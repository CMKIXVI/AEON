#include "aeon_identity.h"
#include <string.h>

int aeon_did_registry_init(aeon_did_registry_t *r,
                           aeon_did_record_t *records, size_t capacity)
{
    if (!r || !records || capacity == 0) return -1;
    r->records = records;
    r->count = 0;
    r->capacity = capacity;
    return 0;
}

int aeon_did_registry_add(aeon_did_registry_t *r,
                          const aeon_did_record_t *rec)
{
    if (!r || !rec || !rec->active || !rec->human_verified ||
        rec->public_key_len != 32 || rec->did[0] == '\0') return -1;
    if (r->count >= r->capacity) return -2;
    if (aeon_did_registry_find(r, rec->did) != NULL) return -3;
    r->records[r->count++] = *rec;
    return 0;
}

const aeon_did_record_t *aeon_did_registry_find(
    const aeon_did_registry_t *r, const char *did)
{
    if (!r || !did) return NULL;
    for (size_t i = 0; i < r->count; ++i)
        if (strncmp(r->records[i].did, did, AEON_DID_MAX) == 0)
            return &r->records[i];
    return NULL;
}

int aeon_did_auth_matches_registry(
    const aeon_did_registry_t *r, const aeon_did_auth_payload_t *auth)
{
    const aeon_did_record_t *rec;
    if (!r || !auth) return 0;
    rec = aeon_did_registry_find(r, auth->did);
    if (!rec || !rec->active || !rec->human_verified ||
        !auth->human_verified) {
        return 0;
    }
    return memcmp(rec->public_key, auth->public_key, 32) == 0;
}
