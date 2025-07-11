# example pico-json-reader implementation

This is an example implementation of the `pico-json-reader` library for the Raspberry Pi Pico. It demonstrates how to use the library to read values from JSON.


## Example

To use this example, you will need to have the Raspberry Pi Pico SDK installed.

Edit the setup.sh script to set the `PICO_SDK_PATH` and `PICO_BOARD` environment variables. Then run *source setup.sh* to apply the environment variables in the current shell.

Create a build directory and navigate into it: mkdir build && cd build

Run CMake to configure the project: cmake ..

Build the project: make

Connect the Raspberry Pi Pico to your computer via USB while holding the BootSel button to place the Pico into the mass storage mode.

Copy the generated binary file (example.uf2) from the build directory to the mounted Pico drive.

i.e. cp example.uf2 /run/media/$USER/RPI-RP2/

The Pico will automatically reset and load the new firmware. Once it has booted, you should connect to the USB or UART interface with a terminal program (e.g., minicom, screen, or PuTTY).

i.e. screen /dev/ttyUSB0 115200

The pico will print JSON test results for the JSON in the source code each time the Pi Pico is reset.

i.e.
```shell
Test JSON:
{
  "first": 11,
  "test": "value",
  "sub": {
    "index": 23,
    "title": "blah"
  },
  "array": [1,2,3],
  "bool": true,
  "float": 1.23,
  "end": [3,2,1]
}
Testing pico-json-reader methods...
Testing json_length...
json_length test passed
Testing json_token_count...
json_token_count test passed
Testing json_parse_tokens...
json_parse_tokens test passed
Testing json_get_value_s...
json_get_value_s test passed
Testing json_get_value_i...
json_get_value_i test passed
Testing json_get_value_d...
json_get_value_d test passed
Testing json_get_value_b...
json_get_value_b test passed
Testing json_key_index...
json_key_index test passed
Testing json_root_key_index...
json_root_key_index test passed
Testing json_root_object_indicies...
json_root_object_indicies test passed
Testing json_root_array_indicies...
json_root_array_indicies test passed

*** PANIC ***

Testing complete.
```


## Usage

See the example to see how to include the library in your Pi Pico CMake project.

The [jsmn](https://github.com/zserge/jsmn) library is used for JSON parsing and 
some familiarity with the jsmn library and how it tokenizes the JSON string can 
be helpful.


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
