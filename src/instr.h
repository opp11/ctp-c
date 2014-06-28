#ifndef __INSTR_H__
#define __INSTR_H__

#include <string.h>
#include <stdint.h>

extern const char *KW_OFF;
extern const char *KW_ON;
extern const char *KW_REST;

extern const char *NAME_CHECK;
extern const char *NAME_SET;
extern const char *NAME_VIN;
extern const char *NAME_GND;
extern const char *NAME_DELAY;

enum {
	CODE_CHECK = 1,
	CODE_SET = 2,
	CODE_VIN = 3,
	CODE_GND = 4,
	CODE_DELAY = 5,
	CODE_END = 'E',
	CODE_INVALID
};

struct instr_t {
	uint8_t code;
	uint16_t arg;
};
extern const struct instr_t HEADER_INSTR;
extern const struct instr_t END_INSTR;

int instr_name_to_code(char* name);
const char *instr_code_to_name(int code);

#endif /* __INSTR_H__ */