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
