#include "aeon_semantic.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string.h>

int aeon_semantic_canonicalize(
    const aeon_semantic_triple_t *t, uint8_t *out, size_t cap, size_t *used)
{
    int n;
    if (!t || !out || !used) return -1;
    n = snprintf((char *)out, cap, "S=%s\nP=%s\nO=%s\n",
                 t->subject, t->predicate, t->object);
    if (n < 0 || (size_t)n >= cap) return -2;
    *used = (size_t)n;
    return 0;
}

int aeon_semantic_hash(
    const aeon_semantic_triple_t *t, uint8_t digest[32])
{
    uint8_t buf[AEON_SEMANTIC_FIELD_MAX * 3u + 16u];
    size_t used;
    if (aeon_semantic_canonicalize(t, buf, sizeof(buf), &used) != 0)
        return -1;
    if (!SHA256(buf, used, digest)) return -2;
    return 0;
}

static int verify_signature(const uint8_t pub[32], const uint8_t sig[64],
                            const uint8_t digest[32])
{
    EVP_PKEY *pkey = NULL;
    EVP_MD_CTX *ctx = NULL;
    int ok = 0;

    pkey = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, NULL, pub, 32);
    ctx = EVP_MD_CTX_new();
    if (!pkey || !ctx) goto done;
    if (EVP_DigestVerifyInit(ctx, NULL, NULL, NULL, pkey) != 1) goto done;
    ok = EVP_DigestVerify(ctx, sig, 64, digest, 32) == 1;

done:
    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(pkey);
    return ok;
}

int aeon_semantic_verify_transmission(
    const aeon_did_registry_t *registry,
    const aeon_semantic_transmission_t *tx)
{
    uint8_t digest[32];

    if (!registry || !tx || !tx->friction_verified ||
        !tx->auth.human_verified) return -1;
    if (!aeon_did_auth_matches_registry(registry, &tx->auth)) return -2;
    if (aeon_semantic_hash(&tx->triple, digest) != 0) return -3;
    if (memcmp(digest, tx->digest, 32) != 0) return -4;
    if (!verify_signature(tx->auth.public_key, tx->auth.signature, digest))
        return -5;
    if (tx->ipfs_cid[0] == '\0') return -6;
    return 0;
}
