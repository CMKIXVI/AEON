#ifndef AEON_SEMANTIC_WIRE_H
#define AEON_SEMANTIC_WIRE_H
#include <stddef.h>
#include <stdint.h>
#include "aeon_semantic_frame.h"
#define AEON_SEMANTIC_WIRE_MAGIC 0x41455731u
#define AEON_SEMANTIC_WIRE_VERSION 1u
int aeon_semantic_wire_encode(const aeon_semantic_frame_t *, uint8_t *, size_t, size_t *);
int aeon_semantic_wire_decode(const uint8_t *, size_t, aeon_semantic_frame_t *);
#endif
