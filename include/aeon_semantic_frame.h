#ifndef AEON_SEMANTIC_FRAME_H
#define AEON_SEMANTIC_FRAME_H
#include <stdint.h>
#include "aeon_semantic.h"
#include "aeon_friction.h"
#define AEON_SEMANTIC_FRAME_MAGIC 0x41455350u
#define AEON_SEMANTIC_FRAME_VERSION 1u
typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    uint32_t total_size;
    uint32_t subject_len;
    uint32_t predicate_len;
    uint32_t object_len;
    uint8_t digest[32];
} aeon_semantic_frame_header_t;
typedef struct {
    aeon_semantic_frame_header_t header;
    char subject[AEON_SEMANTIC_FIELD_MAX];
    char predicate[AEON_SEMANTIC_FIELD_MAX];
    char object[AEON_SEMANTIC_FIELD_MAX];
    aeon_did_auth_payload_t auth;
    aeon_friction_challenge_t friction;
    aeon_human_attestation_t attestation;
    char ipfs_cid[AEON_IPFS_CID_MAX];
} aeon_semantic_frame_t;
int aeon_semantic_frame_validate(const aeon_semantic_frame_t *);
int aeon_semantic_frame_extract(const aeon_semantic_frame_t *, aeon_semantic_transmission_t *);
#endif
