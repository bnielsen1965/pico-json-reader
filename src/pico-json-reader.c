
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "pico-json-reader.h"


/**
 * Calculates the length of a JSON string.
 *
 * @param json The input JSON string.
 * @return The length of the JSON string, or JSONErrorCode if the input is NULL.
 */
int json_length (char *json) {
  if (!json) return JSON_ERR_INVALID;
  return strnlen(json, MAX_JSON_INPUT_LENGTH);
}


/**
 * Counts the number of tokens in a parsed JSON object.
 *
 * @param parser The jsmn JSON parser object.
 * @param json The input JSON string to be parsed.
 * @return The total number of tokens in the parsed JSON string.
 */
int json_token_count (jsmn_parser *parser, char *json) {
  jsmn_init(parser);
  return jsmn_parse(parser, json, json_length(json), NULL, 0);
}


/**
 * Parse the provided JSON string and allocate tokens into the provided tokens pointer.
 * NOTE: The caller is responsible for freeing the allocated memory for the tokens array.
 *
 * @param parser The initialized JSON parser object.
 * @param json The input JSON string to be parsed.
 * @param tokens A pointer to a pointer to an array of jsmntok_t structs, where each element represents a token in the JSON object. If NULL is passed, this function will allocate memory for the tokens array and store it in the provided pointer.
 *
 * @return The number of tokens allocated into the tokens pointer, or JSONErrorCode on failure.
 */
int json_parse_tokens (jsmn_parser *parser, char *json, jsmntok_t **tokens) {
  int token_count = json_token_count(parser, json);
  if (token_count <= 0) return JSON_ERR_INVALID;

  // allocate memory for tokens
  *tokens = malloc(sizeof(jsmntok_t) * token_count);
  if (*tokens == NULL) return JSON_ERR_MEMORY;

  // parse tokens
  jsmn_init(parser);
  token_count = jsmn_parse(parser, json, strlen(json), *tokens, token_count);
  return token_count;
}


/**
 * Get the string value for the given key from the provided JSON string.
 * Returns the JSON_ERR_NONE on success or JSONErrorCode on failure.
 * NOTE: The caller is responsible for freeing the allocated memory.
 *
 * @param key The key for which the value should be retrieved.
 * @param value A pointer to a char pointer that will be set to the value.
 * @param json The JSON string from which the value should be retrieved.
 * @param tokens The parsed JSON tokens.
 * @param start_token The index of the token from which the search should start.
 *
 * @return int JSON_ERR_NONE on success, JSONErrorCode on failure.
*/
int json_get_value_s (char *key, char **value, const char *json, jsmntok_t *tokens, int start_token) {
  int key_index = json_key_index(tokens, start_token, key, (char*)json);
  if (key_index < 0 || tokens[key_index].size != 1) {
    return JSON_ERR_KEY_INVALID;
  }
  return json_get_index_s(key_index + 1, value, json, tokens);
}


/**
 * Get the string value from the JSON string using the given token index.
 * Returns JSON_ERR_NONE on success, JSONErrorCode on failure.
 * NOTE: The caller is responsible for freeing the allocated memory.
 *
 * @param index The token index of the string value.
 * @param value A pointer to a char pointer that will be set to the allocated string value.
 * @param json The JSON string from which the string value will be extracted.
 * @param tokens The parsed JSON tokens.
 *
 * @return int JSON_ERR_NONE on success, JSONErrorCode on failure.
 */
int json_get_index_s (int index, char **value, const char *json, jsmntok_t *tokens) {
  *value = calloc(tokens[index].end - tokens[index].start + 1, sizeof(char));
  if (!*value) return JSON_ERR_MEMORY; // failed to allocate memory
  memcpy(*value, json + tokens[index].start, tokens[index].end - tokens[index].start);
  return JSON_ERR_NONE;
}


/**
 * Retrieve an integer value from a JSON object at the given key.
 *
 * @param key The key to search for in the JSON object.
 * @param value A pointer to an integer to store the retrieved value.
 * @param json The JSON string to search.
 * @param tokens The parsed JSON tokens.
 * @param start_token The index of the token to start searching from.
 * @return JSON_ERR_NONE on success, JSONErrorCode on failure.
 */
int json_get_value_i (char *key, int *value, const char *json, jsmntok_t *tokens, int start_token) {
  int key_index = json_key_index(tokens, start_token, key, (char*)json);
  if (key_index < 0 || tokens[key_index].size != 1) {
    return JSON_ERR_KEY_INVALID;
  }
  return json_get_index_i(key_index + 1, value, json, tokens);
}


/**
 * Get the integer value from the JSON string using the given token index.
 * Returns JSON_ERR_NONE on success, JSONErrorCode on failure.
 *
 * @param index The token index of the integer value.
 * @param value A pointer to an integer to store the retrieved value.
 * @param json The JSON string from which the string value will be extracted.
 * @param tokens The parsed JSON tokens.
 *
 * @return int JSON_ERR_NONE on success, JSONErrorCode on failure.
 */
int json_get_index_i (int index, int *value, const char *json, jsmntok_t *tokens) {
  *value = atoi(json + tokens[index].start);
  return JSON_ERR_NONE;
}


/**
 * Retrieve an double value from a JSON object at the given key.
 *
 * @param key The key to search for in the JSON object.
 * @param value A pointer to an double to store the retrieved value.
 * @param json The JSON string to search.
 * @param tokens The parsed JSON tokens.
 * @param start_token The index of the token to start searching from.
 * @return JSON_ERR_NONE on success, JSONErrorCode on failure.
 */
int json_get_value_d (char *key, double *value, const char *json, jsmntok_t *tokens, int start_token) {
  int key_index = json_key_index(tokens, start_token, key, (char*)json);
  if (key_index < 0 || tokens[key_index].size != 1) {
    return JSON_ERR_KEY_INVALID;
  }
  return json_get_index_d(key_index + 1, value, json, tokens);
}


/**
 * Get the double value from the JSON string using the given token index.
 * Returns JSON_ERR_NONE on success, JSONErrorCode on failure.
 *
 * @param index The token index of the double value.
 * @param value A pointer to an double to store the retrieved value.
 * @param json The JSON string from which the string value will be extracted.
 * @param tokens The parsed JSON tokens.
 *
 * @return int JSON_ERR_NONE on success, JSONErrorCode on failure.
 */
int json_get_index_d (int index, double *value, const char *json, jsmntok_t *tokens) {
  *value = atof(json + tokens[index].start);
  return JSON_ERR_NONE;
}


/**
 * Retrieve an boolean value from a JSON object at the given key.
 *
 * @param key The key to search for in the JSON object.
 * @param value A pointer to an boolean to store the retrieved value.
 * @param json The JSON string to search.
 * @param tokens The parsed JSON tokens.
 * @param start_token The index of the token to start searching from.
 * @return JSON_ERR_NONE on success, JSONErrorCode on failure.
 */
int json_get_value_b (char *key, bool *value, const char *json, jsmntok_t *tokens, int start_token) {
  int key_index = json_key_index(tokens, start_token, key, (char*)json);
  if (key_index < 0 || tokens[key_index].size != 1) {
    return JSON_ERR_KEY_INVALID;
  }
  return json_get_index_b(key_index + 1, value, json, tokens);
}


/**
 * Get the boolean value from the JSON string using the given token index.
 * Returns JSON_ERR_NONE on success, JSONErrorCode on failure.
 *
 * @param index The token index of the boolean value.
 * @param value A pointer to an boolean to store the retrieved value.
 * @param json The JSON string from which the string value will be extracted.
 * @param tokens The parsed JSON tokens.
 *
 * @return int JSON_ERR_NONE on success, JSONErrorCode on failure.
 */
int json_get_index_b (int index, bool *value, const char *json, jsmntok_t *tokens) {
  jsmntok_t *tok = &tokens[index];
  if (strncmp(json + tok->start, "true", tok->end - tok->start) == 0) {
    *value = true;
    return JSON_ERR_NONE;
  } 
  else if (strncmp(json + tok->start, "false", tok->end - tok->start) == 0) {
    *value = false;
    return JSON_ERR_NONE;
  }
  return JSON_ERR_INVALID;
}


// find the last token index in an object and return the index of the last token in the object or -1 on error
/**
 * Find the last token index in an object and return the index of the last token in the object or -1 on error.
 * @param tokens The parsed JSON tokens.
 * @param start_token The index of the object token.
 * @return The index of the last token in the object or JSONErrorCode on error.
*/
int json_last_object_token_index (jsmntok_t *tokens, int start_token) {
  // start token must be an object to have root tokens
  if (tokens[start_token].type != JSMN_OBJECT) {
    return JSON_ERR_INDEX_INVALID;
  }
  // the number of root tokens in this object is the token child size
  int token_count = tokens[start_token].size;
  bool is_key = true; // the next token will naturally be a key
  // process each root token in the object
  while (token_count > 0 || !is_key) {
    start_token += 1;
    if (is_key) {
      token_count -= 1;
      is_key = false; // next token will naturally be a value for this key
    }
    else {
      if (tokens[start_token].type == JSMN_ARRAY) {
        // get the last token in the array
        start_token = json_last_array_token_index(tokens, start_token);
        if (start_token < 0) return start_token;
      }
      else if (tokens[start_token].type == JSMN_OBJECT) {
        // get last token in the object
        start_token = json_last_object_token_index(tokens, start_token);
        if (start_token < 0) return start_token;
      }
      is_key = true; // next token will be a key
    }
  }
  return start_token;
}


/**
 * Find the last token index in an array and return the index of the last token in the array or -1 on error.
 * @param tokens The jsmntok_t array of tokens.
 * @param start_token The index of the array token.
 * @return The index of the last token in the array or JSONErrorCode on error.
*/
int json_last_array_token_index (jsmntok_t *tokens, int start_token) {
  // start token must be an array
  if (tokens[start_token].type != JSMN_ARRAY) {
    return JSON_ERR_INDEX_INVALID;
  }
  // the number of root tokens in this array is the token child size
  int token_count = tokens[start_token].size;
  // loop through all the tokens in the array
  while (token_count > 0) {
    start_token += 1;
    // if token has size then this array value is an array or an object
    if (tokens[start_token].size) {
      if (tokens[start_token].type == JSMN_ARRAY) {
        start_token = json_last_array_token_index(tokens, start_token);
        if (start_token < 0) return start_token;
      }
      else if (tokens[start_token].type == JSMN_OBJECT) {
        start_token = json_last_object_token_index(tokens, start_token);
        if (start_token < 0) return start_token;
      }
      else {
        return JSON_ERR_INVALID;
      }
    }
    token_count -= 1;
  }
  return start_token;
}


/**
 * Create an array of token indices that are root keys of the json object at start_token.
 * NOTE: The caller must free the returned array.
 * NOTE: The root_tokens argument should be NULL or there will be undefined behaviour.
 *
 * @param tokens The array of jsmntok_t to parse.
 * @param start_token The index of the jsmntok_t object token to parse.
 * @param root_tokens A pointer to an array of integers to store the root token indices.
 * @return The number of root tokens found or JSONErrorCode on error.
*/
int json_root_object_indicies (jsmntok_t *tokens, int start_token, int **root_tokens) {
  // start token must be an object to have root tokens
  if (tokens[start_token].type != JSMN_OBJECT) {
    return JSON_ERR_INDEX_INVALID;;
  }
  // the number of root tokens in this object is the token child size
  int token_count = tokens[start_token].size;
  int key_count = 0;
  bool is_key = true;
  // process each root token in the object
  while (token_count > key_count) {
    start_token += 1;
    if (is_key) {
      key_count += 1;
      // allocate memory for the key index array
      int *tmp = *root_tokens;
      if (*root_tokens == NULL) {
        // allocate memory for the key index array
        *root_tokens = malloc(sizeof(int) * key_count);
      }
      else {
        // reallocate with more memory for more keys
        *root_tokens = realloc(*root_tokens, sizeof(int) * key_count);
      }
      // check if memory allocation was successful
      if (NULL == *root_tokens) {
        if (tmp != NULL) free(tmp);
        return JSON_ERR_MEMORY;
      }
      // store the index of this root token in the array
      (*root_tokens)[key_count - 1] = start_token;
      is_key = false;
    }
    else {
      // skip over all values under this key
      if (tokens[start_token].type == JSMN_ARRAY) {
        // get the last token in the array
        start_token = json_last_array_token_index(tokens, start_token);
      }
      else if (tokens[start_token].type == JSMN_OBJECT) {
        // get last token in the object
        start_token = json_last_object_token_index(tokens, start_token);
      }
      is_key = true; // next token will be a key
    }
  }

  return key_count;
}


/**
 * Create an array of token indicieds taht are root elements of the json array at start_token.
 * NOTE: The caller is responsible for freeing the returned array.
 * NOTE: The root_tokens argument should be initialized to NULL.
 * @param tokens: The jsmn tokens to search.
 * @param start_token: The starting token to search from.
 * @param root_tokens: A pointer to an array of integers to store the root token indices.
 * @return: The number of root tokens found, or JSONErrorCode if an error occurred.
*/
int json_root_array_indicies (jsmntok_t *tokens, int start_token, int **root_tokens) {
  // start token must be an object to have root tokens
  if (tokens[start_token].type != JSMN_ARRAY) {
    return JSON_ERR_INDEX_INVALID;
  }
  // the number of root tokens in this array is the token child size
  int token_count = tokens[start_token].size;
  int key_count = 0;
  // process each root token in the array
  while (token_count > key_count) {
    start_token += 1;
    key_count += 1;
    // allocate memory for the key index array
    int *tmp = *root_tokens;
    if (*root_tokens == NULL) {
      // allocate memory for the key index array
      *root_tokens = malloc(sizeof(int) * key_count);
    }
    else {
      // reallocate with more memory for more keys
      *root_tokens = realloc(*root_tokens, sizeof(int) * key_count);
    }
    // store the index of this root token in the array
    (*root_tokens)[key_count - 1] = start_token;
    // if token has size then skip over all tokens in this token object/array
    if (tokens[start_token].size) {
      // skip over all values under this key
      if (tokens[start_token].type == JSMN_ARRAY) {
        // get the last token in the array
        start_token = json_last_array_token_index(tokens, start_token);
      }
      else if (tokens[start_token].type == JSMN_OBJECT) {
        // get last token in the object
        start_token = json_last_object_token_index(tokens, start_token);
      }
    }
  }

  return key_count;
}


/**
 * Get the token index for the given root key name in the JSON object starting at the given token index.
 *
 * @param tokens The array of jsmntok_t tokens
 * @param start_token The starting token index
 * @param key The root key name to search for
 * @param json The JSON string being parsed
 * @return The token index for the given root key name or JSONErrorCode if not found
*/
int json_root_key_index (jsmntok_t *tokens, int start_token, char *key, char *json) {
  // get root key indicies to search
  int *indicies = NULL;
  int key_count = json_root_object_indicies(tokens, start_token, &indicies);
  if (key_count <= 0) {
    if (indicies != NULL) free(indicies);
    return JSON_ERR_KEY_INVALID;
  }
  for (int i = 0; i < key_count; i++) {
    if (json_key_strcmp(key, json, &tokens[indicies[i]]) == JSON_KEY_MATCH) {
      int result = indicies[i];
      free(indicies);
      return result;
    }
  }
  free(indicies);
  return JSON_ERR_KEY_INVALID;
}


/**
 * Get the index of a key in a JSON object. The key may be the name or a dot delimited name path.
 *
 * @param tokens The array of jsmntok_t tokens.
 * @param start_token The index of the starting token.
 * @param key The key to search for.
 * @param json The JSON string.
 * @return The index of the key if found, otherwise JSONErrorCode.
*/
int json_key_index (jsmntok_t *tokens, int start_token, char *key, char *json) {
  int key_dot_index = 0; // token index for the key_dot key name
  int dot_index = 0; // key string index of the dot delimiter
  char *key_dot;
  // loop until the end of the key string
  do {
    key_dot = json_get_key_dot(key, dot_index);
    if (key_dot == NULL) {
      // failed to extract the key name preceding the dot delimiter.
      return JSON_ERR_KEY_INVALID;
    }
    dot_index += strlen(key_dot) + 1;
    key_dot_index = json_root_key_index(tokens, key_dot_index, key_dot, json);
    if (key_dot_index < 0) {
      // failed to find a token index for the key_dot key name.
      free(key_dot);
      return JSON_ERR_KEY_INVALID;
    }
    // if not at end of key then increment key_dot_index by 1 for next iteration
    if (dot_index < strlen(key)) key_dot_index += 1;
    free(key_dot);
  } while (dot_index < strlen(key));
  return key_dot_index;
}


/**
 * Get the key string value preceding any dot delimiter.
 * NOTE: The caller is responsible for freeing the allocated memory.
 *
 * @param key The full key string.
 * @param start_chr The starting character index.
 * @return The key string preceding the dot delimiter.
*/
char * json_get_key_dot (const char *key, int start_chr) {
  char *keydot;
  char *c = ".";
  const char *dot = strnstr(key + start_chr, c, strlen(key));
  if (dot != NULL) {
    keydot = strndup(key + start_chr, dot - key + start_chr);
    return keydot;
  }
  else {
    keydot = strndup(key + start_chr, strlen(key + start_chr));
    return keydot;
  }
  return NULL;
}


/*
 * Compare a key string to a jsmn token. If they match, return 0; otherwise, return -1.
 * @param key The string to compare.
 * @param json The JSON string.
 * @param tok The jsmn token
 * @return JSON_KEY_MATCH if the keys match; otherwise, JSON_KEY_NO_MATCH.
 */
int json_key_strcmp (const char *key, const char *json, jsmntok_t *tok) {
  char *token_key = calloc(tok->end - tok->start + 1, sizeof(char));
  strncpy(token_key, &json[tok->start], tok->end - tok->start);
	if (
    tok->type == JSMN_STRING && 
    (int) strlen(key) == tok->end - tok->start &&
    strncmp(json + tok->start, key, tok->end - tok->start) == 0
  ) {
    return JSON_KEY_MATCH;
  }
	
	return JSON_KEY_NO_MATCH;
}



const char * json_error_string (JSONErrorCode result) {
    switch (result) {
        case JSON_ERR_NONE:
            return "Operation successful.";

        case JSON_ERR_INVALID:
            return "Invalid JSON input.";

        case JSON_ERR_MEMORY:
            return "Memory allocation failed.";

        case JSON_ERR_KEY_INVALID:
            return "Invalid key provided.";

        case JSON_ERR_INDEX_INVALID:
            return "Invalid token index provided.";

        default:
            return "Unknown error code.";
    }
}

