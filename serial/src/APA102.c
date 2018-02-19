/*
* APA102.c
*
* Created: 2/17/2018 5:00:49 PM
*  Author: Jacques
*/
#include "asf.h"
#include "APA102.h"

static struct spi_module spi_master_instance;

static void APA102_configure_spi_master(void)
{
	struct spi_config config_spi_master;
	
	// Configure, initialize and enable SERCOM SPI module.
	spi_get_config_defaults(&config_spi_master);
	
	// Configure pins (MOSI on pad0, SCK on pad1).
	config_spi_master.mux_setting = SPI_SIGNAL_MUX_SETTING_A;
	config_spi_master.pinmux_pad0 = PINMUX_PA00D_SERCOM1_PAD0;
	config_spi_master.pinmux_pad1 = PINMUX_PA01D_SERCOM1_PAD1;
	config_spi_master.pinmux_pad2 = PINMUX_UNUSED;
	config_spi_master.pinmux_pad3 = PINMUX_UNUSED;
	
	spi_init(&spi_master_instance, SERCOM1, &config_spi_master);
	spi_enable(&spi_master_instance);
}

void APA102_init(void)
{
	APA102_configure_spi_master();
}

static uint8_t buffer[] = {
	0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11
};

#define APA102_GLOBAL_OFFSET 4

#define APA102_COLOR_R_OFFSET 7
#define APA102_COLOR_G_OFFSET 6
#define APA102_COLOR_B_OFFSET 5

void APA102_set_color_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t global)
{
	// Set brightness (0-31, 5 bits).
	buffer[APA102_GLOBAL_OFFSET] = 0xe0 | (global & 0x1f);
	
	// Set color.
	buffer[APA102_COLOR_R_OFFSET] = r;
	buffer[APA102_COLOR_G_OFFSET] = g;
	buffer[APA102_COLOR_B_OFFSET] = b;
	
	spi_write_buffer_wait(&spi_master_instance, buffer, sizeof(buffer));
}

static void hsv_to_rgb(float h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b) {
	
	float s_f = s / 255.0f; // 0.0-1.0
	float v_f = v / 255.0f; // 0.0-1.0

	float r_f=0, g_f=0, b_f=0; // 0.0-1.0

	int   hi = (int)(h / 60.0f) % 6;
	float f  = (h / 60.0f) - hi;
	float p  = v_f * (1.0f - s_f);
	float q  = v_f * (1.0f - s_f * f);
	float t  = v_f * (1.0f - s_f * (1.0f - f));

	switch(hi) {
		case 0: r_f = v_f, g_f = t, b_f = p; break;
		case 1: r_f = q, g_f = v_f, b_f = p; break;
		case 2: r_f = p, g_f = v_f, b_f = t; break;
		case 3: r_f = p, g_f = q, b_f = v_f; break;
		case 4: r_f = t, g_f = p, b_f = v_f; break;
		case 5: r_f = v_f, g_f = p, b_f = q; break;
	}

	*r = (uint8_t)(r_f * 255); // 0-255
	*g = (uint8_t)(g_f * 255); // 0-255
	*b = (uint8_t)(b_f * 255); // 0-255
}

void APA102_set_color_hsv(float h, uint8_t s, uint8_t v, uint8_t global)
{
	uint8_t r, g, b;
	hsv_to_rgb(h,s,v,&r,&g,&b);
	APA102_set_color_rgb(r, g, b, global);
}