#include "aeon_model_manifest.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static int aeon_manifest_dtype(const char *text, aeon_model_dtype_t *out)
{
    if (strcmp(text, "F32") == 0) *out = AEON_MODEL_DTYPE_F32;
    else if (strcmp(text, "BF16") == 0) *out = AEON_MODEL_DTYPE_BF16;
    else if (strcmp(text, "F16") == 0) *out = AEON_MODEL_DTYPE_F16;
    else if (strcmp(text, "FP8_E4M3") == 0) *out = AEON_MODEL_DTYPE_FP8_E4M3;
    else if (strcmp(text, "I8") == 0) *out = AEON_MODEL_DTYPE_I8;
    else return -1;
    return 0;
}

static char *aeon_manifest_trim(char *text)
{
    char *end;
    while (isspace((unsigned char)*text)) ++text;
    end = text + strlen(text);
    while (end > text && isspace((unsigned char)end[-1])) --end;
    *end = '\0';
    return text;
}

static int aeon_manifest_u64(const char *text, uint64_t *value)
{
    char *end;
    unsigned long long parsed;
    if (text == NULL || *text == '\0') return -1;
    errno = 0;
    parsed = strtoull(text, &end, 10);
    if (errno != 0 || *aeon_manifest_trim(end) != '\0') return -1;
    *value = (uint64_t)parsed;
    return 0;
}

int aeon_model_manifest_parse(aeon_model_index_t *index, const char *text,
                              size_t length, uint64_t model_bytes)
{
    char *copy;
    size_t pos = 0u;
    size_t records = 0u;
    int rc = 0;

    if (index == NULL || text == NULL || length == 0u || model_bytes == 0u ||
        length > SIZE_MAX - 1u) return -1;
    copy = malloc(length + 1u);
    if (copy == NULL) return -1;
    memcpy(copy, text, length);
    copy[length] = '\0';
    if (aeon_model_index_init(index) != 0) { free(copy); return -1; }

    while (pos < length && rc == 0) {
        char *line = copy + pos;
        char *fields[6] = {0};
        char *cursor;
        size_t field_count = 0u;
        size_t i;
        uint64_t offset = 0u;
        uint64_t bytes = 0u;
        uint64_t rank64 = 0u;
        uint64_t shape[AEON_MODEL_INDEX_MAX_DIMS] = {0};
        aeon_model_dtype_t dtype;

        while (pos < length && copy[pos] != '\n') ++pos;
        if (pos < length) { copy[pos] = '\0'; ++pos; }
        line = aeon_manifest_trim(line);
        if (*line == '\0' || *line == '#') continue;

        cursor = line;
        while (field_count < 6u) {
            char *separator = strchr(cursor, '|');
            fields[field_count++] = aeon_manifest_trim(cursor);
            if (separator == NULL) { cursor = NULL; break; }
            *separator = '\0';
            cursor = separator + 1;
        }
        if (cursor != NULL || field_count != 6u) { rc = -2; break; }
        if (aeon_manifest_u64(fields[1], &offset) != 0 ||
            aeon_manifest_u64(fields[2], &bytes) != 0 ||
            aeon_manifest_u64(fields[4], &rank64) != 0 || rank64 == 0u ||
            rank64 > AEON_MODEL_INDEX_MAX_DIMS ||
            aeon_manifest_dtype(fields[3], &dtype) != 0) { rc = -3; break; }

        cursor = fields[5];
        for (i = 0u; i < (size_t)rank64; ++i) {
            char *separator = strchr(cursor, ',');
            if (separator != NULL) *separator = '\0';
            if (aeon_manifest_u64(aeon_manifest_trim(cursor), &shape[i]) != 0 ||
                shape[i] == 0u) { rc = -4; break; }
            if (separator == NULL) { cursor = NULL; break; }
            cursor = separator + 1;
        }
        if (rc != 0 || cursor != NULL) { rc = -4; break; }
        if (aeon_model_index_add(index, fields[0], offset, bytes, dtype,
                                 (uint8_t)rank64, shape) != 0) { rc = -5; break; }
        ++records;
    }
    if (rc == 0 && records == 0u) rc = -6;
    if (rc == 0) rc = aeon_model_index_validate(index, model_bytes);
    free(copy);
    return rc;
}

int aeon_model_manifest_load_tensor(aeon_model_stream_t *stream,
                                    const aeon_model_index_t *index,
                                    const char *name, void *buffer,
                                    size_t buffer_bytes)
{
    const aeon_model_tensor_meta_t *tensor;
    if (stream == NULL || index == NULL || name == NULL || buffer == NULL) return -1;
    tensor = aeon_model_index_find(index, name);
    if (tensor == NULL || tensor->bytes > (uint64_t)SIZE_MAX ||
        tensor->offset > (uint64_t)SIZE_MAX || tensor->bytes > buffer_bytes) return -1;
    if (aeon_model_stream_seek(stream, (size_t)tensor->offset) != 0) return -1;
    return aeon_model_stream_read_bound(stream, buffer, (size_t)tensor->bytes);
}
