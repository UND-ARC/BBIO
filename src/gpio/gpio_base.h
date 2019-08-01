/*
 * Michael Turnbull
 * 20190327
 * GPIO pin usage headers
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>  // for sleep()

#ifndef GPIO_PIN_DIR
	#define GPIO_PIN_DIR "/sys/class/gpio/"
	#define GPIO_PIN_DIRECTION "/direction"
	#define GPIO_PIN_VALUE "/value"
#endif

int get_hard_pin_number(char* pin_name);

int does_gpio_exist(char* pin);
int create_gpio(char* pin);
int delete_gpio(char* pin);
int get_pin_direction(char* pin);
int set_pin_direction(char* pin, int direction);
int get_pin_value(char* pin);
int set_pin_value(char* pin, int value);
