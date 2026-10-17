#include "aeon_friction.h"
#include <openssl/evp.h>
#include <string.h>
static int verify_attestation_signature(const aeon_human_attestation_t *a)
{
    EVP_PKEY *key = NULL; EVP_MD_CTX *ctx = NULL; uint8_t message[40]; int ok = 0;
    memcpy(message, &a->challenge, sizeof(a->challenge));
    memcpy(message + sizeof(a->challenge), a->context_hash, 32);
    key = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, NULL, a->verifier_public_key, 32);
    ctx = EVP_MD_CTX_new(); if (!key || !ctx) goto done;
    if (EVP_DigestVerifyInit(ctx, NULL, NULL, NULL, key) != 1) goto done;
    ok = EVP_DigestVerify(ctx, a->verifier_signature, 64, message, sizeof(message)) == 1;
done: EVP_MD_CTX_free(ctx); EVP_PKEY_free(key); return ok;
}
int aeon_friction_issue(aeon_friction_challenge_t *c, uint64_t challenge, const uint8_t context_hash[32])
{ if (!c || !context_hash || challenge == 0) return -1; memset(c,0,sizeof(*c)); c->challenge=challenge; memcpy(c->context_hash,context_hash,32); c->issued=1; return 0; }
int aeon_friction_verify(const aeon_friction_challenge_t *c, const aeon_human_attestation_t *a)
{ if (!c || !a || !c->issued || a->challenge != c->challenge) return -1; if (memcmp(a->context_hash,c->context_hash,32)!=0) return -2; if (!verify_attestation_signature(a)) return -3; return 0; }
int aeon_friction_bind_transmission(aeon_semantic_transmission_t *tx, const aeon_friction_challenge_t *c, const aeon_human_attestation_t *a)
{ if (!tx) return -1; if (aeon_friction_verify(c,a)!=0) return -2; tx->friction_nonce=c->challenge; tx->friction_verified=1; return 0; }
