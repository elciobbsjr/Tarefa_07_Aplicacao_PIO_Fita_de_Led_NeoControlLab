#ifndef DISPLAY_OLED_H
#define DISPLAY_OLED_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "ssd1306_i2c.h"

// Pinos e configurações do display OLED
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

extern ssd1306_t display;

void init_display();
void exibir_texto_multilinha(const char *linha1, const char *linha2, const char *linha3);
void exibir_texto_centralizado(const char *texto);

#endif
