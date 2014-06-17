#ifndef __INSTR_H__
#define __INSTR_H__

enum {
	CMD_CHECK = 1,
	CMD_SET = 2,
	CMD_VIN = 3,
	CMD_GND = 4,
	CMD_DELAY = 5
};

struct instr_t {
	int code;
	char** argv;
	int argc;
};

#endif /* __INSTR_H__ */