/*
 * Michael Turnbull
 * 20190327
 * Handles GPIO pin enable/disable stuff
 */

#include "gpio_base.h"

/*
 * Set the (gpio) pin mode to whatever it's supposed to be
 *
 * Arguments:
 *  char* pin : the pin name (e.g. "gpio66")
 *  int mode : 0 for input, 1 for output
 *
 * Returns 1 if successful, 0 if failed.
 */
int set_pin_mode(char* pin, int mode) {
	FILE *fp;
	char* filepath = malloc(sizeof(char) * (strlen(GPIO_PIN_DIR) + strlen(pin) + strlen(GPIO_PIN_DIRECTION)));

	strcpy(filepath, GPIO_PIN_DIR);
	strcat(filepath, pin);
	strcat(filepath, GPIO_PIN_DIRECTION);

	// First, check to see if we're already in the desired mode
	// Read file, check against argument.  Saves disk IO if not needed
	fp = fopen(filepath, "r");
	printf("got file pointer\n");
	if (fp == NULL) {
		printf("[!] failed\n");
		printf("[!] failed to read gpio pin mode, errno=%d, pin=%s\n", errno, pin);
		return 0;
	} else {
		printf("Opened file successfully (for reading)\n");
		// successfully opened file
		char contents[10] = "";
		char buffer[10] = "";
		printf("About to read\n");
		while ( fgets(buffer, sizeof(buffer), fp) ) {
			printf("ch = %s, contents = %s\n", buffer, contents);
			strcat(contents, buffer);
		}
		printf("Read file, contents are %s\n", contents);
		if (mode == 0) {
			if (strcmp(contents, "in") == 0) {
				// Done!  Mode is already input
				return 1;
			} else {
				fclose(fp);
				//free(buffer); free(contents);
			}
		} else if (mode == 1) {
			if (strcmp(contents, "out") == 0) {
				// Done! Mode is already output
				return 1;
			} else {
				fclose(fp);
				//free(buffer); free(contents);
			}
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
	if (mode == 0) {
		strcat(write_out, "in");
	} else if (mode == 1) {
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
 * Tests:
 * Try to set GPIO66 to output, and turn it on
 */
int main(void) {
	set_pin_mode("gpio66", 1);
}
