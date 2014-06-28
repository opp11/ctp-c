#include "parse_file.h"

#define NUM_PINS 16
#define PIN_MAX (NUM_PINS - 1)
#define PIN_MIN 0
#define DELAY_MAX 65535
#define DELAY_MIN 1

static struct instr_t parse_instr(struct fline_t line);

static uint16_t parse_check_args(struct fline_t line);
static uint16_t parse_set_args(struct fline_t line);
static uint16_t parse_delay_args(struct fline_t line);

static uint16_t parse_pin_io(struct fline_t line, uint16_t pins, int *met_pins);
static uint16_t parse_power_config(struct fline_t line, int (*pred)(int pin));

static int handle_rest(uint16_t *pins, int val, int *met_pins);
static int set_single_pin(uint16_t *pins, int pin, int val, int *met_pins);
static int arg_to_num(const char* arg);

static inline int is_vin_pin(int pin)
{
	return (pin == 15 || pin == 14);
}
static inline int is_gnd_pin(int pin)
{
	return (pin == 7);
}
static inline int is_pin(int pin)
{
	return (pin >= PIN_MIN && pin <= PIN_MAX);
}

static int met_vin;
static int met_gnd;

struct instr_t *parse_file(struct fline_t *lines, size_t *len)
{
	size_t out_len = len ? (*len) + 2 : 2;
	struct instr_t *out = malloc(sizeof(struct instr_t) * out_len);
	int i;

	met_vin = 0;
	met_gnd = 0;
	out[0] = HEADER_INSTR;

	for (i = 1; i < out_len - 1; i++){
		out[i] = parse_instr(lines[i]);
	}
	out[out_len - 1] = END_INSTR;
	if (len){
		(*len) = out_len;
	}
	return out;
}

static struct instr_t parse_instr(struct fline_t line)
{
	struct instr_t out = {0, 0};
	if (line.len < 2){
		/* TODO: Error message */
		return out;
	}

	out.code = instr_name_to_code(line.words[0]);
	switch (out.code){
	case CODE_CHECK:
		out.arg = parse_check_args(line);
		break;
	case CODE_SET:
		out.arg = parse_set_args(line);
		break;
	case CODE_VIN:
		met_vin = 1;
		out.arg = parse_power_config(line, is_vin_pin);
		break;
	case CODE_GND:
		met_gnd = 1;
		out.arg = parse_power_config(line, is_gnd_pin);
		break;
	case CODE_DELAY:
		out.arg = parse_delay_args(line);
		break;
	case CODE_INVALID:
		/* TODO: Error message */
		break;
	}
	return out;
}

static uint16_t parse_check_args(struct fline_t line)
{
	int met_pins[NUM_PINS] = {0};
	uint16_t out = parse_pin_io(line, 0, met_pins);
	int i;
	for (i = 0; i < NUM_PINS; i++){
		if (!met_pins[i]){
			/* TODO: Error message */
			return 0;
		}
	}
	return out; 
}

static uint16_t parse_set_args(struct fline_t line)
{
	static uint16_t out = 0;
	int met_pins[NUM_PINS] = {0};
	out = parse_pin_io(line, out, met_pins);
	return out;
}

static uint16_t parse_delay_args(struct fline_t line)
{
	long int out = 0;
	if (line.len > 2){
		/* TODO: Error message */
		return 0;
	}
	out = arg_to_num(line.words[1]);
	if (out == -1){
		/* TODO: Error message */
		return 0;
	} else if (out < DELAY_MIN || out > DELAY_MAX){
		/* TODO: Error message */
		return 0;
	}
	return out;
}

static uint16_t parse_pin_io(struct fline_t line, uint16_t pins, int *met_pins)
{
	uint16_t out = pins;
	int i;
	int pin;
	int crnt_mod = -1;
	for (i = 1; i < line.len; i++){
		if (!strcmp(line.words[i], KW_ON)){
			crnt_mod = 1;
		} else if (!strcmp(line.words[i], KW_OFF)){
			crnt_mod = 0;
		} else if (!strcmp(line.words[i], KW_REST)){
			handle_rest(&out, crnt_mod, met_pins);
		} else {
			pin = arg_to_num(line.words[i]) - 1;
			if (pin == -1 || !is_pin(pin)){
				/* TODO: Error message */
				return 0;
			}
			set_single_pin(&out, pin, crnt_mod, met_pins);
		}
	}
	return out;
}

static uint16_t parse_power_config(struct fline_t line, int (*pred)(int pin))
{
	uint16_t out = 0;
	int i;
	int pin;
	int met_pins[NUM_PINS] = {0};
	for (i = 1; i < line.len; i++){
		pin = arg_to_num(line.words[i]) - 1;
		if (pin == -1 || !pred(pin)){
			/* TODO: Error message */
			return 0;
		}
		set_single_pin(&out, pin, 1, met_pins);
	}
	return out;
}

static int handle_rest(uint16_t *pins, int val, int *met_pins){
	int i;
	for (i = 0; i < NUM_PINS; i++){
		if (!met_pins[i]){
			set_single_pin(pins, i, val, met_pins);
		}
	}
	return 0;
}

static int set_single_pin(uint16_t *pins, int pin, int val, int *met_pins)
{
	if (met_pins[pin]){
		return -1;
		/* TODO: Error message */
	} else if (val == -1){
		/* TODO: Error message */
		return -1;
	} else {
		met_pins[pin] = 1;
		if (val){
			(*pins) |= 1 << pin;
		} else {
			(*pins) &= ~(1 << pin);
		}
		return 0;
	}
}

static int arg_to_num(const char *arg)
{
	char *endptr;
	int out = strtol(arg, &endptr, 10);
	if ((*arg) == '\0' || (*endptr) != '\0'){
		out = -1;
	}
	return out;
}