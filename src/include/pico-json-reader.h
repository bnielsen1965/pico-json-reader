
#include <stdbool.h>
#include "pico/stdlib.h"
#define JSMN_HEADER
#include "jsmn.h"

#ifndef PICO_JSON_READER_H
#define PICO_JSON_READER_H

typedef enum {
    JSON_ERR_NONE = 0,         // No error
    JSON_ERR_INVALID = -1,
    JSON_ERR_MEMORY = -2,
    JSON_ERR_KEY_INVALID = -3,
    JSON_ERR_INDEX_INVALID = -4,


} JSONErrorCode;

#define JSON_KEY_MATCH 0
#define JSON_KEY_NO_MATCH -1

#ifndef MAX_JSON_INPUT_LENGTH
#define MAX_JSON_INPUT_LENGTH 4096
#endif

int json_length (char *json);
int json_token_count (jsmn_parser *parser, char *json);
int json_parse_tokens (jsmn_parser *parser, char *json, jsmntok_t **tokens);

int json_get_value_s(char *key, char **value, const char *json, jsmntok_t *tokens, int start_token);
int json_get_index_s (int index, char **value, const char *json, jsmntok_t *tokens);
int json_get_value_i (char *key, int *value, const char *json, jsmntok_t *tokens, int start_token);
int json_get_index_i (int index, int *value, const char *json, jsmntok_t *tokens);
int json_get_value_d (char *key, double *value, const char *json, jsmntok_t *tokens, int start_token);
int json_get_index_d (int index, double *value, const char *json, jsmntok_t *tokens);
int json_get_value_b (char *key, bool *value, const char *json, jsmntok_t *tokens, int start_token);
int json_get_index_b (int index, bool *value, const char *json, jsmntok_t *tokens);

int json_key_strcmp (const char *s, const char *json, jsmntok_t *tok);

int json_last_object_token_index (jsmntok_t *tokens, int start_token);
int json_last_array_token_index (jsmntok_t *tokens, int start_token);
int json_root_object_indicies (jsmntok_t *tokens, int start_token, int **root_tokens);
int json_root_array_indicies (jsmntok_t *tokens, int start_token, int **root_tokens);
int json_root_key_index (jsmntok_t *tokens, int start_token, char *key, char *json);
char * json_get_key_dot (const char *key, int start_chr);
int json_key_index (jsmntok_t *tokens, int start_token, char *key, char *json);

const char * json_error_string (JSONErrorCode result);

#endif
