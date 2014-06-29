#ifndef __INSTR_H__
#define __INSTR_H__

#include <string.h>
#include <stdint.h>

/* Keywords */
extern const char *KW_OFF;
extern const char *KW_ON;
extern const char *KW_REST;

/* The text name of the instructions */
extern const char *NAME_CHECK;
extern const char *NAME_SET;
extern const char *NAME_VIN;
extern const char *NAME_GND;
extern const char *NAME_DELAY;

/* The binary code of the instructions */
enum {
	CODE_CHECK = 1,
	CODE_SET = 2,
	CODE_VIN = 3,
	CODE_GND = 4,
	CODE_DELAY = 5,
	CODE_END = 'E',
	CODE_INVALID
};

/*
 * Structure for containing the binary representation of an instruction.
 *
 * members:
 *	code - The 8-bit instruction code.
 * 	arg  - The 16-bit instruction argument.
 */
struct instr_t {
	uint8_t code;
	uint16_t arg;
};
/* Special instruction for the 3 byte file header */
extern const struct instr_t HEADER_INSTR;
/* Special instruction for the 3 byte file end code */
extern const struct instr_t END_INSTR;

/*
 * Converts a text name of an instruction to the corresponding binary code.
 * 
 * params:
 * 	name - Text name of the instruction.
 *
 * returns:
 * 	int  - The binary code corresponding to 'name' or CODE_INVALID if no
 *	       suitable code was found.
 */
int instr_name_to_code(char* name);

/*
 * Converts a binary instruction code to the corresponding text name.
 * 
 * params:
 *	code - The instruction code.
 *
 * returns:
 *	const char* - The text name of the instruction.
 */
const char *instr_code_to_name(int code);

#endif /* __INSTR_H__ */