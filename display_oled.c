#include <string.h>
#include "display_oled.h"

ssd1306_t display;

void init_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    ssd1306_init_bm(&display, OLED_WIDTH, OLED_HEIGHT, false, ssd1306_i2c_address, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_init();
}

void exibir_texto_multilinha(const char *linha1, const char *linha2, const char *linha3) {
    memset(display.ram_buffer + 1, 0, display.bufsize - 1);
    if (linha1) ssd1306_draw_string(display.ram_buffer + 1, 0, 8, (char *)linha1);
    if (linha2) ssd1306_draw_string(display.ram_buffer + 1, 0, 24, (char *)linha2);
    if (linha3) ssd1306_draw_string(display.ram_buffer + 1, 0, 40, (char *)linha3);

    ssd1306_command(&display, ssd1306_set_column_address);
    ssd1306_command(&display, 0);
    ssd1306_command(&display, OLED_WIDTH - 1);
    ssd1306_command(&display, ssd1306_set_page_address);
    ssd1306_command(&display, 0);
    ssd1306_command(&display, (OLED_HEIGHT / 8) - 1);
    ssd1306_send_data(&display);
}

void exibir_texto_centralizado(const char *texto) {
    int largura = strlen(texto) * 8;
    int x = (OLED_WIDTH - largura) / 2;
    int y = (OLED_HEIGHT - 8) / 2;

    memset(display.ram_buffer + 1, 0, display.bufsize - 1);
    ssd1306_draw_string(display.ram_buffer + 1, x, y, (char *)texto);

    ssd1306_command(&display, ssd1306_set_column_address);
    ssd1306_command(&display, 0);
    ssd1306_command(&display, OLED_WIDTH - 1);
    ssd1306_command(&display, ssd1306_set_page_address);
    ssd1306_command(&display, 0);
    ssd1306_command(&display, (OLED_HEIGHT / 8) - 1);
    ssd1306_send_data(&display);
}
