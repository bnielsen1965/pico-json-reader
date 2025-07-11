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



### JSON, parser and tokens

Your project will need a JSON string to be parsed, a jsmn_parser structure, 
and jsmntok_t token array to store the tokens. Your program must include the 
`pico-json-reader.h` header, declare the jsmn objects and provide a JSON string.


```c

#include "pico-json-reader.h"

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

int main() {
  jsmn_parser parser;
	jsmntok_t *tokens;

  ...

}
```


Use the json_parse_tokens method to parse the JSON string and store the tokens in the token array.


```c

   ...

  json_parse_tokens(&parser, (char*)TEST_JSON, &tokens);

  ...

```


After the JSON is parsed and tokenized the json methods can be used to extract values based on keys.


```c

  ...

  int err;
  char *test_s = NULL;
  char *sub_title_s = NULL;
  int first_i = 0;
  if ((err = json_get_value_s("test", &test_s, (char*)TEST_JSON, tokens, 0)) != JSON_ERR_NONE) {
    printf("Error: %s\n", json_error_string(err));
    return 1;
  }

  if ((err = json_get_value_s("sub.title", &sub_title_s, (char*)TEST_JSON, tokens, 0)) != JSON_ERR_NONE) {
    printf("Error: %s\n", json_error_string(err));
    return 1;
  }

  if ((err = json_get_value_i("first", &first_i, (char*)TEST_JSON, tokens, 0)) != JSON_ERR_NONE) {
    printf("Error: %s\n", json_error_string(err));
    return 1;
  }

  printf("Restuls: test = %s, sub.title = %s, first = %d\n", test_s, sub_title_s, first_i);

  ...

```

Complete example:
```c


#include "pico-json-reader.h"

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

int main() {
  jsmn_parser parser;
	jsmntok_t *tokens;

  json_parse_tokens(&parser, (char*)TEST_JSON, &tokens);

  int err;
  char *test_s = NULL;
  char *sub_title_s = NULL;
  int first_i = 0;
  if ((err = json_get_value_s("test", &test_s, (char*)TEST_JSON, tokens, 0)) != JSON_ERR_NONE) {
    printf("Error: %s\n", json_error_string(err));
    return 1;
  }

  if ((err = json_get_value_s("sub.title", &sub_title_s, (char*)TEST_JSON, tokens, 0)) != JSON_ERR_NONE) {
    printf("Error: %s\n", json_error_string(err));
    return 1;
  }

  if ((err = json_get_value_i("first", &first_i, (char*)TEST_JSON, tokens, 0)) != JSON_ERR_NONE) {
    printf("Error: %s\n", json_error_string(err));
    return 1;
  }

  printf("Results: test = %s, sub.title = %s, first = %d\n", test_s, sub_title_s, first_i);

  return 0;
}

```


## User Methods

The following are the user methods that are commonly used. There are additional methods 
that may be of use in certain scenarios.



int json_parse_tokens (jsmn_parser *parser, char *json, jsmntok_t **tokens)

Parse the provided JSON string and allocate tokens into the provided tokens pointer.
NOTE: The caller is responsible for freeing the allocated memory for the tokens array.

Returns the number of tokens allocated into the tokens pointer, or JSONErrorCode on failure.



int json_get_value_s (char *key, char **value, const char *json, jsmntok_t *tokens, int start_token)

Get the string value for the given key from the provided JSON string.
NOTE: The caller is responsible for freeing the allocated memory.

Returns JSON_ERR_NONE on success or JSONErrorCode on failure.



int json_get_value_i (char *key, int *value, const char *json, jsmntok_t *tokens, int start_token)

Retrieve an integer value from a JSON object at the given key.
Return JSON_ERR_NONE on success, JSONErrorCode on failure.



int json_get_value_d (char *key, double *value, const char *json, jsmntok_t *tokens, int start_token)

Retrieve an double value from a JSON object at the given key.

Return JSON_ERR_NONE on success, JSONErrorCode on failure.



int json_get_value_b (char *key, bool *value, const char *json, jsmntok_t *tokens, int start_token)

Retrieve an boolean value from a JSON object at the given key.

Return JSON_ERR_NONE on success, JSONErrorCode on failure.




const char * json_error_string (JSONErrorCode result)

Converts a JSONErrorCode to a human-readable string.

Returns a pointer to a static string representing the error code.
