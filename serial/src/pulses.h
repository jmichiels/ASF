/*
 * pulses.h
 *
 * Created: 2/19/2018 2:11:50 PM
 *  Author: Jacques
 */ 


#ifndef PULSES_H_
#define PULSES_H_

typedef void (*pulses_callback_t)(uint32_t cycle);

bool pulses_init(Tcc* const tcc, uint8_t pin, uint32_t frequency);

void pulses_register_callback(pulses_callback_t callback_func);

void pulses_run(uint32_t cycles);

#endif /* PULSES_H_ */