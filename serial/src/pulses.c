/*
 * pulses.c
 *
 * Created: 2/19/2018 2:11:08 PM
 *  Author: Jacques
 */ 

#include "asf.h"
#include "pulses.h"

static struct tcc_module tcc_instance;

static uint32_t half_period;
static uint32_t cycles_target = 0;
static uint32_t cycles_current = 0;

inline static void noop(uint32_t cycles) {};
	
pulses_callback_t user_callback = noop;

static void pulses_callback(struct tcc_module *const module)
{
	if((++cycles_current) >= cycles_target) {
		// Reached the target number of cycles.
		tcc_stop_counter(&tcc_instance);
		// Do nothing else before complete stop.
		while (tcc_is_syncing(&tcc_instance) || tcc_is_running(&tcc_instance));
	}
	user_callback(cycles_current);
}

bool pulses_init(Tcc* const tcc, uint8_t pin, uint32_t frequency)
{
	struct tcc_config config;
	tcc_get_config_defaults(&config, tcc);
	
	// Compute half period in clock cycles.
	half_period = system_gclk_gen_get_hz(config.counter.clock_source) / (2*frequency);
	
	if (2*half_period > 0xffff) {
		// Only 16 bits counter is supported.
		return false;
	}
	
	// Set the counter period.
	config.counter.period = 2*half_period;
	
	// Wave generation: count from zero to period.
	config.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	
	// Set match value. 
	config.compare.match[0] = half_period;
	
	// Enable the PWM output.
	config.pins.enable_wave_out_pin[0] = true;
	config.pins.wave_out_pin_mux[0] = MUX_PA10E_TCC1_WO0;
	config.pins.wave_out_pin[0]	= PIN_PA10E_TCC1_WO0;
	
	if (tcc_init(&tcc_instance, tcc, &config) != STATUS_OK) {
		// An error occurred while initializing the TCC module.
		return false;
	}
	
	// Register the callback.
	tcc_register_callback(&tcc_instance, pulses_callback, TCC_CALLBACK_CHANNEL_0);
	
	// Enable the callback.
	tcc_enable_callback(&tcc_instance, TCC_CALLBACK_CHANNEL_0);
	
	// Enable the timer.
	tcc_enable(&tcc_instance);
	
	return true;
}

void pulses_register_callback(pulses_callback_t callback_func)
{
	user_callback = callback_func;
}

void pulses_run(uint32_t cycles)
{
	// Reset counter.
	cycles_current = 0;
	// Set target.
	cycles_target = cycles;
	
	/// Restart the counter.
	tcc_restart_counter(&tcc_instance);
	
	// Wait to have reached the target number of cycles.
	while (tcc_is_syncing(&tcc_instance) || tcc_is_running(&tcc_instance));
}