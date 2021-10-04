#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"

typedef void (*MenuSetFunction)(uint8_t value);
typedef uint8_t (*MenuGetFunction)();

enum MenuType {NUMBER, TOGGLE, EXIT};

const typedef struct MenuItem_t {
	char text[18];
	MenuType type;
	MenuSetFunction funcSet;
	MenuGetFunction funcGet;
	uint8_t min;
	uint8_t max;
} MenuItem;

#endif