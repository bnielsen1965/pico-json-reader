# pico-json-reader

A JSON reader library for the Raspberry Pi Pico using C and the jsmn JSON parser. 

This library provides methods to read values from JSON strings by leveraging the 
jsmn JSON parser which tokenizes the JSON string.


## Usage

See the example directory for an example of how to use the pico-json-reader submodule.


### git submodule

Use git to add pico-json-reader as a submodule to your project.

> git submodule add https://github.com/bnielsen1965/pico-json-reader.git

> git submodule update --init --recursive


### CMakeLists.txt

Add the following to your project's CMakeLists.txt file:

add_subdirectory(pico-json-reader build)


    target_link_libraries(my_project 
        pico_stdlib 
        pico-json-reader
    )


### Include pico-json-reader header file

In your source code, include the pico-json-reader header file:

    #include "pico-json-reader.h"


## Methods


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
