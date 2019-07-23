/*
 * Michael Turnbull
 * 20190327
 * Handles GPIO pin enable/disable stuff
 */

#include "gpio_base.h"


/*
 * Get the (gpio) pin direction, in or out.
 *
 * Arguments:
 *  char* pin : the pin name (e.g. "gpio66")
 *
 * Returns 1 if output, 0 if input.  -1 if error
 */
int get_pin_direction(char* pin) {
	FILE *fp;
	char* filepath = malloc(sizeof(char) * (strlen(GPIO_PIN_DIR) + strlen(pin) + strlen(GPIO_PIN_DIRECTION) + 3));

	strcpy(filepath, GPIO_PIN_DIR);
	strcat(filepath, pin);
	strcat(filepath, GPIO_PIN_DIRECTION);

	// read file contents
	fp = fopen(filepath, "r");
	char contents[4];  // mode is only ever "in" or "out"
	if (fgets(contents, sizeof(contents), fp) == NULL) {
		// uh-oh
		printf("[!] failed to read gpio pin mode, errno=%d, pin=%s\n", errno, pin);
		return -1;
	} else {
		// success!
		if ( (strcmp(contents, "in\n") == 0) || (strcmp(contents, "in") == 0) ) {
			return 0;
		} else if ( (strcmp(contents, "out\n") == 0) || (strcmp(contents, "out") == 0) ) {
			return 1;
		}
	}
	printf("[!] how the hell did I get here?? gpio_base.c:get_pin_mode\n");
	return -1;
}


/*
 * Set the (gpio) pin mode to whatever it's supposed to be
 *
 * Arguments:
 *  char* pin : the pin name (e.g. "gpio66")
 *  int mode : 0 for input, 1 for output
 *
 * Returns 1 if successful, 0 if failed.
 */
int set_pin_direction(char* pin, int direction) {
	FILE *fp;
	char* filepath = malloc(sizeof(char) * (strlen(GPIO_PIN_DIR) + strlen(pin) + strlen(GPIO_PIN_DIRECTION) + 3));

	strcpy(filepath, GPIO_PIN_DIR);
	strcat(filepath, pin);
	strcat(filepath, GPIO_PIN_DIRECTION);

	// First, check to see if we're already in the desired mode
	// Read file, check against argument.  Saves disk IO if not needed
	int current_dir = get_pin_direction(pin);
	if (current_dir == -1) {
		printf("[!] unable to determine pin direction, goahead with overwrite\n");
	} else {
		if (direction == current_dir) {
			// no work to be done
			return 1;
		}
	}

	// if we're here, then the mode needs to be set, the file is
	// closed, and char* contents has been free'd

	fp = fopen(filepath, "w");
	if (fp == NULL) {
		printf("[!] failed to open gpio direction file for writing, errno=%d, pin=%s\n", errno, pin);
		return 0;
	}

	// file open, write to it
	// first, decide what we're going to write
	char write_out[3] = "";
	if (direction == 0) {
		strcat(write_out, "in");
	} else if (direction == 1) {
		strcat(write_out, "out");
	}

	// write it
	int res = fputs(write_out, fp);
	if (res == EOF) {
		printf("[!] failed to write to gpio direction file, errno=%d, pin=%s\n", errno, pin);
		return 0;
	}

	// wrote it, done
	return 1;
}

/*
 * Gets the GPIO pin value.
 *
 * Arguments:
 *  char* pin : the pin to get, e.g. "gpio66"
 *
 * Returns:
 *  int : 1 or 0: pin value; -1: error
 */
int get_pin_value(char* pin) {
	FILE* fp;
	char* filepath = malloc(sizeof(char) * ( strlen(GPIO_PIN_DIR) + strlen(pin) + strlen(GPIO_PIN_VALUE) + 3));

	strcpy(filepath, GPIO_PIN_DIR);
	strcat(filepath, pin);
	strcat(filepath, GPIO_PIN_VALUE);

	fp = fopen(filepath, "r");
	if (fp == NULL) {
		printf("[!] failed to open gpio pin value for reading, errno=%d, pin=%s\n", errno, pin);
		return -1;
	}

	char contents[2];  // one character and a newline
	if ( fgets(contents, sizeof(contents), fp) == NULL) {
		// uh-oh
		printf("[!] failed to read gpio pin value, errno=%d, pin=%s\n", errno, pin);
		return -1;
	} else {
		return atoi(contents);
	}

	printf("[!] how the hell did i get here?? gpio_base:get_pin_value\n");
	return -1;
}

/*
 * Sets the GPIO pin value.
 *
 * Arguments:
 *  char* pin : the pin to change, e.g. "gpio66"
 *  int value : the new output value (1 or 0)
 *
 * Returns:
 *  int: 1 if success
 *       0 if pin is in input mode (error)
 *       -1 if other error
 */
int set_pin_value(char* pin, int value) {
	int cur_value = get_pin_value(pin);
	if (get_pin_direction(pin) == 0) {
		// pin is in input mode, return 0
		return 0;
	}
	if (cur_value == value) {
		// no work to be done
		// already checked for pin in input mode
		return 1;
	}

	FILE* fp;
	char* filepath = malloc(sizeof(char) + (strlen(GPIO_PIN_DIR) + strlen(pin) + strlen(GPIO_PIN_VALUE) + 3));

	strcpy(filepath, GPIO_PIN_DIR);
	strcat(filepath, pin);
	strcat(filepath, GPIO_PIN_VALUE);

	fp = fopen(filepath, "w");
	free(filepath);
	if (fp == NULL) {
		// dammit
		printf("[!] failed to open gpio value file for writing, errno=%d, pin=%s\n", errno, pin);
		return -1;
	}

	// value sanity-check
	if ( (value != 0) && (value != 1) ) {
		printf("[!] illegal argument given to set_pin_value; must be 0 or 1; got %d\n", value);
		fclose(fp);
		return -1;
	}

	// if we're here, write to the file
	char contents[2] = "";
	snprintf(contents, sizeof(contents), "%d", value);

	int res = fputs(contents, fp);
	if (res == EOF) {
		printf("[!] failed to write gpio value, errno=%d, pin=%s, value=%d\n", errno, pin, value);
		fclose(fp);
		return -1;
	}

	// if we got here, mission success
	fclose(fp);
	return 1;
}


/*
 * Tests:
 * Try to set GPIO66 to output, and turn it on
 */
int main(void) {
	printf("========== GPIO self-test =========\n");
	printf("Pin: gpio66\n");  char pin[] = "gpio66";
	printf("direction: %d\n", get_pin_direction(pin));
	printf("Setting to out...");
	set_pin_direction(pin, 1);
	printf("Done!  direction: %d\n", get_pin_direction(pin));
	printf("value: %d\n", get_pin_value(pin));
	
	printf("Setting value to 1...\n");
	set_pin_value(pin, 1);
	printf("sleeping 1 second...\n");
	sleep(1);
	printf("Setting value to 0...\n");
	set_pin_value(pin, 0);
	printf("Done! value: %d\n", get_pin_value(pin));
	printf("========== GPIO self-test done ====\n");
}
