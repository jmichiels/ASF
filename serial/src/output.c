/*
 * output.c
 *
 * Created: 2/18/2018 2:25:27 PM
 *  Author: Jacques
 */ 
#include "asf.h"
#include "output.h"

void output_init(struct output* output_instance, uint8_t gpio_pin)
{
	// Get the port for the specified pin.
	output_instance->port = port_get_group_from_gpio_pin(gpio_pin);
	
	// Get the mask for the pin.
	output_instance->mask =  (1UL << (gpio_pin % 32));
	
	// Configure pin as an output.
	output_instance->port->DIRSET.reg = output_instance->mask;
}