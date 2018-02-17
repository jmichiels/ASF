/**
* \file
*
* \brief Empty user application template
*
*/

/**
* \mainpage User Application template doxygen documentation
*
* \par Empty user application template
*
* Bare minimum empty user application template
*
* \par Content
*
* -# Include the ASF header files (through asf.h)
* -# Minimal main function that starts with a call to system_init()
* -# "Insert application code here" comment
*
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
/*
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/
#include <asf.h>

#define PIN_LED_BUILTIN PIN_PA10

static void main_init_system(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();
	
	// Initialize the sleep manager.
	sleepmgr_init();
	
	// Initialize ASF.
	system_init();
	
	// Initialize delay.
	delay_init();
}

static void main_init_serial(void)
{
	// Start USB.
	stdio_usb_init();
	stdio_usb_enable();
}

static void main_init_gpio(void)
{
	struct port_config config_output;
	port_get_config_defaults(&config_output);
	config_output.direction = PORT_PIN_DIR_OUTPUT;
	
	// Configure output pins.
	port_pin_set_config(PIN_LED_BUILTIN, &config_output);
}

int main(void)
{
	main_init_system();
	main_init_gpio();
	main_init_serial();
	
	uint16_t cnt = 0;
	
	while (true) {
		printf("Hello World %d\n", cnt++);
		port_pin_toggle_output_level(PIN_PA10);
		delay_ms(500);
	}
}


