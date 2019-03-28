# gpio_base.h

## `get_pin_mode`

This function checks whether a desired GPIO pin is currently set for input or output.

Signature: `int get_pin_direction(char* pin)`

Arguments: 
* `char* pin`: The name of the GPIO pin to check.  Example: `"gpio66"`.

Returns:
* `int`: 1 if pin is output, 0 if input, -1 if error/unable to determine.

## `set_pin_mode`

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

