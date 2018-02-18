/*
 * APA102.h
 *
 * Created: 2/17/2018 6:08:18 PM
 *  Author: Jacques
 */ 

#ifndef APA102_H_
#define APA102_H_

#include "asf.h"

#define APA102_GLOBAL_OFFSET 4

#define APA102_COLOR_R_OFFSET 7
#define APA102_COLOR_G_OFFSET 6
#define APA102_COLOR_B_OFFSET 5

void APA102_init(void);
void APA102_set_color_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t global);
void APA102_set_color_hsv(float h, uint8_t s, uint8_t v, uint8_t global);

#endif /* APA102_H_ */