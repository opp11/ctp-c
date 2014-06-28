#include "instr.h"

const char *KW_OFF = "off";
const char *KW_ON = "on";
const char *KW_REST = "rest";

const char *NAME_CHECK = "check";
const char *NAME_SET = "set";
const char *NAME_VIN = "vin";
const char *NAME_GND = "gnd";
const char *NAME_DELAY = "delay";

const struct instr_t HEADER_INSTR = {'P', 'M' | ('1'<<8)};
const struct instr_t END_INSTR = {CODE_END, 'N' | ('D'<<8)};

int instr_name_to_code(char* name)
{
	if (!strcmp(name, NAME_CHECK)){
		return CODE_CHECK;
	} else if (!strcmp(name, NAME_SET)){
		return CODE_SET;
	} else if (!strcmp(name, NAME_VIN)){
		return CODE_VIN;
	} else if (!strcmp(name, NAME_GND)){
		return CODE_GND;
	} else if (!strcmp(name, NAME_DELAY)){
		return CODE_DELAY;
	} else {
		return CODE_INVALID;
	}
}

const char *instr_code_to_name(int code)
{
	switch(code){
	case CODE_CHECK:
		return NAME_CHECK;
	case CODE_SET:
		return NAME_SET;
	case CODE_VIN:
		return NAME_VIN;
	case CODE_GND:
		return NAME_GND;
	case CODE_DELAY:
		return NAME_DELAY;
	default:
		return NULL;
	}
}