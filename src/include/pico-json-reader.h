
#include <stdbool.h>
#include "pico/stdlib.h"
#define JSMN_HEADER
#include "jsmn.h"

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
