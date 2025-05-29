#include <string.h>
#include "display_oled.h"

// Declaro o objeto que representa o display OLED
ssd1306_t display;

// Função para inicializar o display OLED via I2C
void init_display() {
    // Inicializo o barramento I2C com frequência de 400kHz
    i2c_init(I2C_PORT, 400 * 1000);
    
    // Configuro os pinos SDA e SCL para função I2C
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // Ativo pull-up nos pinos SDA e SCL (recomendado para I2C)
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Inicializo o display com as configurações básicas
    ssd1306_init_bm(&display, OLED_WIDTH, OLED_HEIGHT, false, ssd1306_i2c_address, I2C_PORT);
    ssd1306_config(&display);

    // Faço a inicialização final do display
    ssd1306_init();
}

// Função para exibir até 3 linhas de texto no display
void exibir_texto_multilinha(const char *linha1, const char *linha2, const char *linha3) {
    // Limpo o buffer de vídeo, exceto o primeiro byte (provavelmente reservado para comando/control byte)
    memset(display.ram_buffer + 1, 0, display.bufsize - 1);

    // Escrevo cada linha de texto se ela não for nula
    if (linha1) ssd1306_draw_string(display.ram_buffer + 1, 0, 8, (char *)linha1);
    if (linha2) ssd1306_draw_string(display.ram_buffer + 1, 0, 24, (char *)linha2);
    if (linha3) ssd1306_draw_string(display.ram_buffer + 1, 0, 40, (char *)linha3);

    // Envio comandos para definir a área da tela que vai ser atualizada
    ssd1306_command(&display, ssd1306_set_column_address);
    ssd1306_command(&display, 0);
    ssd1306_command(&display, OLED_WIDTH - 1);

    ssd1306_command(&display, ssd1306_set_page_address);
    ssd1306_command(&display, 0);
    ssd1306_command(&display, (OLED_HEIGHT / 8) - 1);

    // Envio o conteúdo do buffer para o display
    ssd1306_send_data(&display);
}

// Função para exibir um texto centralizado no display
void exibir_texto_centralizado(const char *texto) {
    // Calculo a largura do texto (assumindo 8 pixels por caractere)
    int largura = strlen(texto) * 8;
    
    // Calculo as posições X e Y para centralizar o texto
    int x = (OLED_WIDTH - largura) / 2;
    int y = (OLED_HEIGHT - 8) / 2;

    // Limpo o buffer de vídeo
    memset(display.ram_buffer + 1, 0, display.bufsize - 1);

    // Desenho o texto no buffer centralizado
    ssd1306_draw_string(display.ram_buffer + 1, x, y, (char *)texto);

    // Envio comandos para definir a área de atualização do display
    ssd1306_command(&display, ssd1306_set_column_address);
    ssd1306_command(&display, 0);
    ssd1306_command(&display, OLED_WIDTH - 1);

    ssd1306_command(&display, ssd1306_set_page_address);
    ssd1306_command(&display, 0);
    ssd1306_command(&display, (OLED_HEIGHT / 8) - 1);

    // Envio o buffer atualizado para o display
    ssd1306_send_data(&display);
}
