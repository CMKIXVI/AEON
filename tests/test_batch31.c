#include "aeon_kernel_adapter.h"
#include "aeon_semantic_gate.h"
#include <assert.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>

static void keypair(EVP_PKEY **key, uint8_t pub[32], uint8_t base)
{
    uint8_t seed[32];
    size_t n = sizeof(seed);
    for (unsigned i = 0; i < 32; ++i) seed[i] = (uint8_t)(base + i);
    *key = EVP_PKEY_new_raw_private_key(EVP_PKEY_ED25519, NULL, seed,
                                        sizeof(seed));
    assert(*key);
    assert(EVP_PKEY_get_raw_public_key(*key, pub, &n) == 1);
    assert(n == 32);
}

static void sign_message(EVP_PKEY *key, const uint8_t *msg, size_t len,
                         uint8_t sig[64])
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    size_t n = 64;
    assert(ctx);
    assert(EVP_DigestSignInit(ctx, NULL, NULL, NULL, key) == 1);
    assert(EVP_DigestSign(ctx, sig, &n, msg, len) == 1);
    assert(n == 64);
    EVP_MD_CTX_free(ctx);
}

static void init_frame(aeon_semantic_frame_t *f)
{
    memset(f, 0, sizeof(*f));
    f->header.magic = AEON_SEMANTIC_FRAME_MAGIC;
    f->header.version = AEON_SEMANTIC_FRAME_VERSION;
    snprintf(f->subject, sizeof(f->subject), "did:aeon:human:test31");
    snprintf(f->predicate, sizeof(f->predicate), "aeon:mayTransmit");
    snprintf(f->object, sizeof(f->object), "network:semantic-triple");
    f->header.subject_len = (uint32_t)strlen(f->subject);
    f->header.predicate_len = (uint32_t)strlen(f->predicate);
    f->header.object_len = (uint32_t)strlen(f->object);
    f->header.total_size = (uint32_t)sizeof(f->header) +
        f->header.subject_len + f->header.predicate_len +
        f->header.object_len + 3u +
        (uint32_t)sizeof(f->auth) + (uint32_t)sizeof(f->friction) +
        (uint32_t)sizeof(f->attestation) + (uint32_t)sizeof(f->ipfs_cid);
}

int main(void)
{
    EVP_PKEY *did_key, *verifier_key;
    uint8_t did_pub[32], verifier_pub[32], msg[40], digest[32];
    aeon_did_record_t rec, records[2];
    aeon_did_registry_t registry;
    aeon_semantic_frame_t frame;
    aeon_semantic_transmission_t tx;
    aeon_friction_challenge_t challenge;
    aeon_human_attestation_t att;
    aeon_semantic_gate_reason_t reason;
    aeon_net_event_t event;
    aeon_net_action_t action;

    keypair(&did_key, did_pub, 0x10);
    keypair(&verifier_key, verifier_pub, 0x80);

    memset(&rec, 0, sizeof(rec));
    snprintf(rec.did, sizeof(rec.did), "did:aeon:human:test31");
    memcpy(rec.public_key, did_pub, 32);
    rec.public_key_len = 32;
    rec.human_verified = 1;
    rec.active = 1;
    assert(aeon_did_registry_init(&registry, records, 2) == 0);
    assert(aeon_did_registry_add(&registry, &rec) == 0);

    init_frame(&frame);
    assert(aeon_semantic_frame_extract(&frame, &tx) == 0);
    assert(aeon_semantic_hash(&tx.triple, digest) == 0);
    memcpy(frame.header.digest, digest, sizeof(digest));
    memcpy(frame.auth.public_key, did_pub, 32);
    memcpy(frame.auth.did, rec.did, sizeof(rec.did));
    memcpy(frame.auth.vc_hash, digest, sizeof(digest));
    frame.auth.human_verified = 1;
    snprintf(frame.ipfs_cid, sizeof(frame.ipfs_cid), "bafybeiaeon31proof");
    sign_message(did_key, digest, sizeof(digest), frame.auth.signature);

    assert(aeon_friction_issue(&challenge, 0xAE310001u, digest) == 0);
    frame.friction = challenge;
    memset(&att, 0, sizeof(att));
    att.challenge = challenge.challenge;
    memcpy(att.context_hash, challenge.context_hash, 32);
    memcpy(att.verifier_public_key, verifier_pub, 32);
    memcpy(msg, &att.challenge, 8);
    memcpy(msg + 8, att.context_hash, 32);
    sign_message(verifier_key, msg, sizeof(msg), att.verifier_signature);
    att.valid = 1;
    frame.attestation = att;

    assert(aeon_semantic_frame_validate(&frame) == 0);
    assert(aeon_semantic_gate_check(&frame, &registry, &reason) == 0);
    assert(reason == AEON_SEMANTIC_ALLOW);

    memset(&event, 0, sizeof(event));
    event.process_id = 31;
    event.protocol = 6;
    event.payload_len = frame.header.total_size;
    event.semantic_candidate = 1;
    event.semantic_frame = &frame;
    assert(aeon_net_policy_decide(&event, AEON_NET_HOOK_PRE_TX,
                                  &registry, &action) == 0);
    assert(action == AEON_NET_ACCEPT);

    frame.object[0] = 'X';
    assert(aeon_semantic_gate_check(&frame, &registry, &reason) == 0);
    assert(reason == AEON_SEMANTIC_DENY_DIGEST);
    assert(aeon_net_policy_decide(&event, AEON_NET_HOOK_PRE_TX,
                                  &registry, &action) == 0);
    assert(action == AEON_NET_DROP);

    frame.object[0] = 'n';
    frame.attestation.context_hash[0] ^= 1u;
    assert(aeon_semantic_gate_check(&frame, &registry, &reason) == 0);
    assert(reason == AEON_SEMANTIC_DENY_IDENTITY);
    assert(aeon_net_policy_decide(&event, AEON_NET_HOOK_PRE_TX,
                                  &registry, &action) == 0);
    assert(action == AEON_NET_DROP);

    EVP_PKEY_free(did_key);
    EVP_PKEY_free(verifier_key);
    puts("AEON Batch 31 authenticated semantic transmission test: PASS");
    return 0;
}
