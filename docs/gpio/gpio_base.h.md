# gpio_base.h

## `get_pin_direction`

This function checks whether a desired GPIO pin is currently set for input or output.

Signature: `int get_pin_direction(char* pin)`

Arguments: 
* `char* pin`: The name of the GPIO pin to check.  Example: `"gpio66"`.

Returns:
* `int`: 1 if pin is output, 0 if input, -1 if error/unable to determine.

## `set_pin_direction`

This function sets the direction (input or output) of a GPIO pin.

Signature: `int set_pin_direction(char* pin, int direction)`

Arguments:
* `char* pin`: The name of the GPIO pin to edit.  Example: `"gpio66"`.
* `int direction`: 0 for input, 1 for output.

Returns:
* `int`: 1 if successful, 0 if failed.

Notes:
* Will not change value if the pin is already in the desired direction.
* If the function is unable to determine the pin direction (`get_pin_direction` returns -1), the direction will be set to the desired value.

## `get_pin_value`

This function gets the value from a desired GPIO pin, either high or low (1 or 0).

Signature: `int get_pin_value(char* pin)`

Arguments:
* `char* pin`: The name of the GPIO pin to check.  Example: `"gpio66"`.

Returns:
* `int`: 1 if the pin is high, 0 if the pin is low, -1 if error.

## `set_pin_value`

This function sets the value for a desired GPIO pin.  Note that before attempting this operation, the pin should be set to output mode via `set_pin_direction`.

Signature: `int set_pin_value(char* pin, int value)`

Arguments:
* `char* pin`: The name of the GPIO pin to edit.  Example: `"gpio66"`.
* `int value`: The value to set.  1 for high, 0 for low.  Arguments other than 0 or 1 will result in an error message.

Returns:
* `int`: 1 for success, otherwise error: 0 if the pin is in input mode, and -1 for unspecified error.

Notes:
* Will not change the value if the pin is already outputting the desired value.
* If the pin is in input mode, and the preexisting value is equal to the value being set, the function will _still_ return an error message (specifically `0`).
* If the function cannot determine the pin value (`get_pin_value` returns -1), the value will be set to the desired value.


