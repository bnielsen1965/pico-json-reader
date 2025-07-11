
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "pico-json-reader.h"

#define SLEEP_MS 30000

#define TEST_JSON_TOKEN_COUNT 25
#define TEST_JSON "" \
  "{\n" \
  "  \"first\": 11,\n" \
  "  \"test\": \"value\",\n" \
  "  \"sub\": {\n" \
  "    \"index\": 23,\n" \
  "    \"title\": \"blah\"\n" \
  "  },\n" \
  "  \"array\": [1,2,3],\n" \
  "  \"bool\": true,\n" \
  "  \"float\": 1.23,\n" \
  "  \"end\": [3,2,1]\n" \
  "}"
#define TEST1_KEY "test"
#define TEST1_VALUE "value"
#define TEST2_KEY "sub.title"
#define TEST2_VALUE "blah"
#define TEST3_KEY "first"
#define TEST3_VALUE 11
#define TEST4_KEY "sub.index"
#define TEST4_VALUE 23
#define TEST5_KEY "array"
#define TEST5_INDEX 11
#define TEST6_KEY "sub.title"
#define TEST6_INDEX 9
#define TEST7_KEY "array"
#define TEST7_INDEX 11
#define TEST8_COUNT 7
#define TEST9_START 6
#define TEST9_COUNT 2
#define TEST10_INDEX 12
#define TEST10_COUNT 3
#define TEST11_KEY "float"
#define TEST11_VALUE 1.23
#define TEST12_KEY "bool"
#define TEST12_VALUE true

const char * JSON = TEST_JSON;


// declare methods
void printToken (jsmntok_t *t, int index, char *json);
int test_json_length (char *json);
int test_json_token_count (jsmn_parser *parser, char *json);
int test_json_get_value_s (jsmntok_t *tokens, char *json);
int test_json_get_value_i (jsmntok_t *tokens, char *json);
int test_json_get_value_d (jsmntok_t *tokens, char *json);
int test_json_get_value_b (jsmntok_t *tokens, char *json);
int test_json_key_index (jsmntok_t *tokens, char *json);
int test_json_root_key_index (jsmntok_t *tokens, char *json);
int test_json_root_object_indicies (jsmntok_t *tokens);
int test_json_root_array_indicies (jsmntok_t *tokens);



// main application entry point
int main() {

  // initialize stdio
  stdio_init_all();

  
  jsmn_parser parser;
	jsmntok_t *tokens;
  int status;
	int token_count;


  printf("Test JSON:\n%s\n", JSON);
  
  ///////////////////////////////////
  // test pico-json-reader methods //
  ///////////////////////////////////

  printf("Testing pico-json-reader methods...\n");

  printf("Testing json_length...\n");
  if (0 != test_json_length((char*)JSON)) {
    panic("json_length test failed");
  }
  printf("json_length test passed\n");


  printf("Testing json_token_count...\n");
  if (0 != test_json_token_count(&parser, (char*)JSON)) {
    panic("json_token_count test failed");
  }
  printf("json_token_count test passed\n");


  printf("Testing json_parse_tokens...\n");
  if (TEST_JSON_TOKEN_COUNT != json_parse_tokens(&parser, (char*)JSON, &tokens)) {
    panic("json_parse_tokens test failed");
  }
  printf("json_parse_tokens test passed\n");
  

  printf("Testing json_get_value_s...\n");
  if (0 != test_json_get_value_s(tokens, (char*)JSON)) {
    panic("json_get_value_s test failed");
  }
  printf("json_get_value_s test passed\n");


  printf("Testing json_get_value_i...\n");
  if (0 != test_json_get_value_i(tokens, (char*)JSON)) {
    panic("json_get_value_i test failed");
  }
  printf("json_get_value_i test passed\n");


  printf("Testing json_get_value_d...\n");
  if (0 != test_json_get_value_d(tokens, (char*)JSON)) {
    panic("json_get_value_d test failed");
  }
  printf("json_get_value_d test passed\n");


  printf("Testing json_get_value_b...\n");
  if (0 != test_json_get_value_b(tokens, (char*)JSON)) {
    panic("json_get_value_b test failed");
  }
  printf("json_get_value_b test passed\n");


  printf("Testing json_key_index...\n");
  if (0 != test_json_key_index(tokens, (char*)JSON)) {
    panic("json_key_index test failed");
  }
  printf("json_key_index test passed\n");
  

  printf("Testing json_root_key_index...\n");
  if (0 != test_json_root_key_index(tokens, (char*)JSON)) {
    panic("json_root_key_index test failed");
  }
  printf("json_root_key_index test passed\n");


  printf("Testing json_root_object_indicies...\n");
  if (0 != test_json_root_object_indicies(tokens)) {
    panic("json_root_object_indicies test failed");
  }
  printf("json_root_object_indicies test passed\n");


  printf("Testing json_root_array_indicies...\n");
  if (0 != test_json_root_array_indicies(tokens)) {
    panic("json_root_array_indicies test failed");
  }
  printf("json_root_array_indicies test passed\n");


  panic("Testing complete.");

}



// print details about a specific jsmn token
void printToken (jsmntok_t *toks, int index, char *json) {
  jsmntok_t *t = &toks[index];
      // printf("I: %d %d %d\n", index, t->start, t->end);
  char *token_key = calloc(t->end - t->start + 1, sizeof(char));
  strncpy(token_key, &json[t->start], t->end - t->start);
  printf("TOKEN: i %d, s %d, e %d, z %d, t %d, v %s\n", index, t->start, t->end, t->size, t->type, token_key);
  free(token_key);
}


int test_json_length (char *json) {
  if (json_length(json) != strlen(json)) return -1;
  if (json_length("{}") != 2) return -1;
  if (json_length("") != 0) return -1;
  if (json_length(NULL) != -1) return -1;
  return 0;
}


int test_json_token_count (jsmn_parser *parser, char *json) {
  if (json_token_count(parser, json) != TEST_JSON_TOKEN_COUNT) return -1;
  if (json_token_count(parser, "{}") != 1) return -1;
  if (json_token_count(parser, "") != 0) return -1;
  if (json_token_count(parser, NULL) != 0) return -1;
  return 0;
}


int test_json_get_value_s (jsmntok_t *tokens, char *json) {
  int err;
  char *value = NULL;
  if ((err = json_get_value_s(TEST1_KEY, &value, json, tokens, 0)) != JSON_ERR_NONE) {
    printf("Get value %s failed, %s\n", TEST1_KEY, json_error_string(err));
    return -1;
  }
  if (strcmp(value, TEST1_VALUE) != 0) {
    printf("Expected value '%s', but got '%s'\n", TEST1_VALUE, value);
    free(value);
    return -1;
  }
  free(value);

  if ((err = json_get_value_s(TEST2_KEY, &value, json, tokens, 0)) != JSON_ERR_NONE) {
    printf("Get value %s failed, %s\n", TEST2_KEY, json_error_string(err));
    return -1;
  }
  if (strcmp(value, TEST2_VALUE) != 0) {
    free(value);
    return -1;
  }
  free(value);

  if ((err = json_get_value_s("nokey", &value, json, tokens, 0)) != JSON_ERR_KEY_INVALID) {
    printf("Expected error '%s', but got '%s'\n", json_error_string(JSON_ERR_KEY_INVALID), json_error_string(err));
    free(value);
    return -1;
  }

  if ((err = json_get_value_s("nokey.dot", &value, json, tokens, 0)) != JSON_ERR_KEY_INVALID) {
    printf("Expected error '%s', but got '%s'\n", json_error_string(JSON_ERR_KEY_INVALID), json_error_string(err));
    free(value);
    return -1;
  }

  return 0;
}


int test_json_get_value_i (jsmntok_t *tokens, char *json) {
  int err;
  int value = 0;
  if ((err = json_get_value_i(TEST3_KEY, &value, json, tokens, 0)) != JSON_ERR_NONE) {
    printf("Get value %s failed, %s\n", TEST3_KEY, json_error_string(err));
    return -1;
  }
  if (value != TEST3_VALUE) {
    printf("Expected '%d', but got '%d'\n", TEST3_VALUE, value);
    return -1;
  }

  if ((err = json_get_value_i(TEST4_KEY, &value, json, tokens, 0)) != JSON_ERR_NONE) {
    printf("Get value %s failed, %s\n", TEST4_KEY, json_error_string(err));
    return -1;
  }
  if (value != TEST4_VALUE) {
    printf("Expected '%d', but got '%d'\n", TEST4_VALUE, value);
    return -1;
  }

  return 0;
}


int test_json_get_value_d (jsmntok_t *tokens, char *json) {
  int err;
  double value = 0;
  if ((err = json_get_value_d(TEST11_KEY, &value, json, tokens, 0)) != JSON_ERR_NONE) {
    printf("Get value %s failed, %s\n", TEST11_KEY, json_error_string(err));
    return -1;
  }
  if (value != TEST11_VALUE) {
    printf("Expected '%f', but got '%f',\n", TEST11_VALUE, value);
    return -1;
  }

  return 0;
}


int test_json_get_value_b (jsmntok_t *tokens, char *json) {
  int err;
  bool value = false;
  if ((err = json_get_value_b(TEST12_KEY, &value, json, tokens, 0)) != JSON_ERR_NONE) {
    printf("Get value %s failed, %s\n", TEST12_KEY, json_error_string(err));
    return -1;
  }
  if (value != TEST12_VALUE) {
    printf("Expected '%d', but got '%d',\n", TEST12_VALUE, value);
    return -1;
  }

  return 0;
}


int test_json_key_index (jsmntok_t *tokens, char *json) {
  int result;
  if ((result = json_key_index(tokens, 0, TEST5_KEY, json)) < 0) {
    printf("Key index failed, %s\n", json_error_string(result));
    return -1;
  }

  if ((result = json_key_index(tokens, 0, TEST6_KEY, json)) < 0) {
    printf("Key index failed, %s\n", json_error_string(result));
    return -1;
  }

  return 0;
}


int test_json_root_key_index (jsmntok_t *tokens, char *json) {
  int index = json_root_key_index(tokens, 0, TEST7_KEY, json);
  if (index < 0) {
    printf("Root key index failed, %s\n", json_error_string(index));
    return -1;
  }
  if (TEST7_INDEX != index) {
    printf("Root object index failed, expected %d but got %d\n", TEST7_INDEX, index);
    return -1;
  }
  return 0;
}


int test_json_root_object_indicies (jsmntok_t *tokens) {
  int *indicies = NULL;
  int key_count = json_root_object_indicies(tokens, 0, &indicies);
  if (key_count < 0) {
    printf("Root object indicies failed, %s\n", json_error_string(key_count));
    return -1;
  }
  if (TEST8_COUNT != key_count) {
    printf("Root object indicies failed, expected %d key count, but got %d\n", TEST8_COUNT, key_count);
    free(indicies);
    return -1;
  }
  free(indicies);
  indicies = NULL;

  key_count = json_root_object_indicies(tokens, TEST9_START, &indicies);
  if (key_count < 0) {
    printf("Root object indicies failed, %s\n", json_error_string(key_count));
    return -1;
  }
  if (TEST9_COUNT != key_count) {
    printf("Root object indicies failed, expected %d key count, but got %d\n", TEST9_COUNT, key_count);
    free(indicies);
    return -1;
  }
  free(indicies);

  return 0;
}


int test_json_root_array_indicies (jsmntok_t *tokens) {
  int *indicies = NULL;
  int key_count = json_root_array_indicies(tokens, TEST10_INDEX, &indicies);
  if (key_count < 0) {
    printf("Root array indicies failed, %s\n", json_error_string(key_count));
    return -1;
  }
  if (TEST10_COUNT != key_count) {
    printf("Root array indicies failed, expected %d key count, but got %d\n", TEST10_COUNT, key_count);
    free(indicies);
    return -1;
  }
  free(indicies);

  return 0;
}
