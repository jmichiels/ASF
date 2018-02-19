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
#include "APA102.h"
#include "output.h"

#define PIN_LED_13 PIN_PA10

#define PIN_LED_APA102_DO PIN_PA00 // data out
#define PIN_LED_APA102_CO PIN_PA01 // clock out

static struct output led_13;

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
	
	// Initialize RGB led.
	APA102_init();
	
	// Initialize led "13".
	output_init(&led_13, PIN_LED_13);
}

static void main_init_serial(void)
{
	// Start USB.
	stdio_usb_init();
	stdio_usb_enable();
}

// Periods in seconds.
#define UPDATE_FREQUENCY (100ul);

static float hue = 0;

static void main_callback(struct tc_module *const module)
{
	if (++hue > 360.0f) {
		// Stay in the circle.
		hue -= 360.0f;
	}
	// Update LED hue.	
	APA102_set_color_hsv(hue++, 0xff, 0xff, 0xff);
}

static struct tc_module tc_instance;

static bool main_init_tc(void)
{	
	struct tc_config config;
	tc_get_config_defaults(&config);
	
	// Counter size.
	config.counter_size = TC_COUNTER_SIZE_32BIT;
	
	// Wave generation: count from zero to CC0.
	config.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
	
	// Set CC0 value (for specified frequency).
	config.counter_32_bit.compare_capture_channel[0] = system_gclk_gen_get_hz(0) / UPDATE_FREQUENCY;
	
	if(tc_init(&tc_instance, TC4, &config) != STATUS_OK) {
		return false;
	}
	
	// Register the callback function on CC0.
	tc_register_callback(&tc_instance, main_callback, TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);

	// Start the timer.
	tc_enable(&tc_instance);
	
	return true;
}

int main(void)
{
	main_init_system();
	main_init_serial();
	
	if (!main_init_tc()) {
		// Something went wrong.
		APA102_set_color_rgb(0xff,0x00,0x00, 1);
	}

	while (true) {
		// Enter sleep as everything is done via timers.
		sleepmgr_enter_sleep();
	}
}

