/*
 * Michael Turnbull
 * 20190729
 * Handles UART interfacing/messaging
 */

#include "uart_base.h"


/*
 * Determine whether or not a serial device exists.
 *
 * Arguments:
 *  char* device: the device name (e.g. "ttyS0")
 *
 * Returns 1 if exists, 0 if not.  -1 if error
 */
int does_exist(char* device) {
  char* filepath = malloc(sizeof(char) * (strlen(DEVICE_DIR) + strlen(device) + 3));
  strcpy(filepath, DEVICE_DIR);
  strcat(filepath, device);

  if (access(filepath, F_OK) != -1) {
    // file exists
    return 1;
  } else {
    // file does not exist
    return 0;
  }
}
