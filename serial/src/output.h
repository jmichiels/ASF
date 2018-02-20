/*
 * output.h
 *
 * Created: 2/18/2018 2:25:13 PM
 *  Author: Jacques
 */ 


#ifndef OUTPUT_H_
#define OUTPUT_H_

struct output_module {
	PortGroup* port;
	uint32_t mask;
};

void output_init(struct output_module* output_instance, uint8_t gpio_pin);

inline void output_set(struct output_module* output_instance) {
	output_instance->port->OUTSET.reg = output_instance->mask;
}

inline void output_clear(struct output_module* output_instance) {
	output_instance->port->OUTCLR.reg = output_instance->mask;
}

inline void output_toggle(struct output_module* output_instance) {
	output_instance->port->OUTTGL.reg = output_instance->mask;
}

#endif /* OUTPUT_H_ */