#include "parse_file.h"

#define NUM_PINS 16
#define PIN_MAX (NUM_PINS - 1)
#define PIN_MIN 0
#define DELAY_MAX 65535
#define DELAY_MIN 1

static struct instr_t parse_instr(struct fline_t line);

static uint16_t parse_check_args(struct fline_t line);
static uint16_t parse_set_args(struct fline_t line);
static uint16_t parse_vin_args(struct fline_t line);
static uint16_t parse_gnd_args(struct fline_t line);
static uint16_t parse_delay_args(struct fline_t line);

/* Parses the arguments for pin i/o, e.g. SET or CHECK. */
static uint16_t parse_pin_io(struct fline_t line, uint16_t pins, int *met_pins);

/* Parses the arguments for power config, e.g. GND or VIN. */
static uint16_t parse_power_config(struct fline_t line, int (*pred)(int pin));

/* Sets all pins in 'pins' which have not been met yet to 'val'. */
static int handle_rest(uint16_t *pins, int val, int *met_pins);

/* Sets a correspondig bit in 'pins' to 'val'. Also updates 'met_pins'. */
static int set_single_pin(uint16_t *pins, int pin, int val, int *met_pins);

/* Returns 'arg' converted to pin number, or -1 on failure to do so. */
static int arg_to_num(const char* arg, int *success);

static size_t count_actual_lines(struct fline_t *lines, size_t len);

static inline int is_vin_pin(int pin)
{
	return (pin == 15 || pin == 14 || pin == 13 || pin == 4);
}
static inline int is_gnd_pin(int pin)
{
	return (pin == 7 || pin == 11);
}
static inline int is_pin(int pin)
{
	return (pin >= PIN_MIN && pin <= PIN_MAX);
}

static int met_vin;
static int met_gnd;

struct instr_t *parse_file(struct fline_t *lines, size_t *len)
{
	struct instr_t *out = malloc(sizeof(struct instr_t) *
		count_actual_lines(lines, *len));
	size_t out_len = 0;
	unsigned int i;
	char crnt_line[MAX_LINE_DIGITS]; /* Should be enough digits... */

	met_vin = 0;
	met_gnd = 0;

	for (i = 0; i < (*len); i++){
		if (lines[i].len == 0){
			continue;
		}
		if (snprintf(crnt_line, MAX_LINE_DIGITS, "%i", i + 1) >=
				MAX_LINE_DIGITS){
			report_fatal("too many lines");
		}
		push_location(crnt_line);
		out[out_len] = parse_instr(lines[i]);
		out_len++;
		pop_location();
	}
	(*len) = out_len;
	return out;
}

static struct instr_t parse_instr(struct fline_t line)
{
	struct instr_t out = {0, 0};

	out.code = instr_name_to_code(line.words[0]);
	switch (out.code){
	case CODE_CHECK:
		out.arg = parse_check_args(line);
		break;
	case CODE_SET:
		out.arg = parse_set_args(line);
		break;
	case CODE_VIN:
		out.arg = parse_vin_args(line);
		break;
	case CODE_GND:
		out.arg = parse_gnd_args(line);
		break;
	case CODE_DELAY:
		out.arg = parse_delay_args(line);
		break;
	case CODE_INVALID:
		report_error("invalid instruction '%s'", line.words[0]);
		break;
	}
	return out;
}

static uint16_t parse_check_args(struct fline_t line)
{
	int met_pins[NUM_PINS] = {0};
	uint16_t out;

	push_location("check");
	out = parse_pin_io(line, 0, met_pins);
	/* Check that all pins were given a value */
	int i;
	for (i = 0; i < NUM_PINS; i++){
		if (!met_pins[i]){
			report_error("all pins must be given a value");
			out = 0;
			goto exit;
		}
	}
exit:
	pop_location();
	return out; 
}

static uint16_t parse_set_args(struct fline_t line)
{
	static uint16_t out = 0;
	int met_pins[NUM_PINS] = {0};
	push_location("set");
	out = parse_pin_io(line, out, met_pins);
	pop_location();
	return out;
}

static uint16_t parse_vin_args(struct fline_t line)
{
	uint16_t out;
	met_vin = 1;
	push_location("vin");
	out = parse_power_config(line, is_vin_pin);
	pop_location();
	return out;
}

static uint16_t parse_gnd_args(struct fline_t line)
{
	uint16_t out;
	met_gnd = 1;
	push_location("gnd");
	out = parse_power_config(line, is_gnd_pin);
	pop_location();
	return out;
}

static uint16_t parse_delay_args(struct fline_t line)
{
	int was_num;
	long int out = 0;
	push_location("delay");
	if (line.len != 2){
		report_error("exactly 1 argument must be given");
		out = 0;
		goto exit;
	}
	out = arg_to_num(line.words[1], &was_num);
	if (!was_num){
		report_error("argument must be a number");
		out = 0;
	} else if (out < DELAY_MIN || out > DELAY_MAX){
		report_error("argument must be between %i and %i", 
			DELAY_MIN, DELAY_MAX);
		out = 0;
	}
exit:
	pop_location();
	return out;
}

static uint16_t parse_pin_io(struct fline_t line, uint16_t pins, int *met_pins)
{
	uint16_t out = pins;
	int i;
	int pin;
	int was_num;
	int crnt_mod = -1;

	if (!met_vin || !met_gnd){
		report_warning("manipulationg pins without specifying ground and/or vin");
	}
	if (line.len < 3){
		report_error("at least 2 arguments must be given");
		return 0;
	}
	for (i = 1; i < line.len; i++){
		if (!strcmp(line.words[i], KW_ON)){
			crnt_mod = 1;
		} else if (!strcmp(line.words[i], KW_OFF)){
			crnt_mod = 0;
		} else if (!strcmp(line.words[i], KW_REST)){
			handle_rest(&out, crnt_mod, met_pins);
		} else {
			pin = arg_to_num(line.words[i], &was_num) - 1;
			if (!was_num || !is_pin(pin)){
				report_error("argument must be either %s, %s, %s or a pin number (%i - %i)",
					KW_ON, KW_OFF, KW_REST, PIN_MIN + 1, 
					PIN_MAX + 1);
				return 0;
			}
			if (set_single_pin(&out, pin, crnt_mod, met_pins)){
				return 0;
			}
		}
	}
	return out;
}

static uint16_t parse_power_config(struct fline_t line, int (*pred)(int pin))
{
	uint16_t out = 0;
	int i;
	int pin;
	int was_num;
	int met_pins[NUM_PINS] = {0};
	if (line.len < 2){
		report_error("at least 1 argument must be given");
	}
	for (i = 1; i < line.len; i++){
		pin = arg_to_num(line.words[i], &was_num) - 1;
		if (!was_num){
			report_error("argument must be a number");
			return 0;
		} else if (!pred(pin)){
			report_error("the number '%s' is not a valid pin number",
				line.words[i]);
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
		report_error("pins may only be given a value once");
		return -1;
	} else if (val == -1){
		report_error("a value must be given before any pin numbers");
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

static int arg_to_num(const char *arg, int *success)
{
	char *endptr;
	int out = strtol(arg, &endptr, 10);
	if ((*arg) == '\0' || (*endptr) != '\0'){
		if (success){
			(*success) = 0;
		}
		out = -1;
	} else {
		if (success){
			(*success) = 1;
		}
	}
	return out;
}

static size_t count_actual_lines(struct fline_t *lines, size_t len)
{
	size_t i;
	size_t out = 0;
	for (i = 0; i < len; i++){
		if (lines[i].len != 0){
			out++;
		}
	}
	return out;
}