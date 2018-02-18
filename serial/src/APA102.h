/*
 * APA102.h
 *
 * Created: 2/17/2018 6:08:18 PM
 *  Author: Jacques
 */ 

#ifndef APA102_H_
#define APA102_H_

void APA102_init(void);
void APA102_set_color_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t global);
void APA102_set_color_hsv(float h, uint8_t s, uint8_t v, uint8_t global);

#endif /* APA102_H_ */