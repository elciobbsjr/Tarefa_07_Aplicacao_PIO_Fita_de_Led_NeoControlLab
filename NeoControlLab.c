#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "neopixel_driver.h"
#include "testes_cores.h"
#include "efeitos.h"
#include "efeito_curva_ar.h"
#include "numeros_neopixel.h"
#include "display_oled.h"  // <- novo arquivo com funções do display

// Pino da fita NeoPixel (Matriz WS2812B)
#define LED_PIN 7

// Botão A da BitDogLab
#define BUTTON_A 5

volatile bool gerar_numeros = false;

// Interrupção do botão A
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL)) {
        gerar_numeros = true;
    }
}

// Inicialização geral
void setup() {
    stdio_init_all();
    sleep_ms(1000);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    npInit(LED_PIN);
    init_display();
    srand(time_us_32());

    exibir_texto_multilinha("Aperte o Botao A", "para sortear um", "numero");
}

// Sorteia número entre min e max
int sorteia_entre(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Mostra o número na matriz de LEDs
void mostrar_numero_sorteado(int numero) {
    switch (numero) {
        case 1: mostrar_numero_1(); break;
        case 2: mostrar_numero_2(); break;
        case 3: mostrar_numero_3(); break;
        case 4: mostrar_numero_4(); break;
        case 5: mostrar_numero_5(); break;
        case 6: mostrar_numero_6(); break;
    }
}

int main() {
    setup();
    char texto[32];

    while (true) {
        if (gerar_numeros) {
            gerar_numeros = false;

            int vezes = sorteia_entre(100, 500);
            printf("Sorteando %d numeros...\n", vezes);

            for (int i = 0; i < vezes; i++) {
                int n = sorteia_entre(1, 6);
                printf("Numero: %d\n", n);

                snprintf(texto, sizeof(texto), "%d", n);
                exibir_texto_centralizado(texto);

                mostrar_numero_sorteado(n);
                sleep_ms(10);
            }

            // Volta a mensagem de instrução
            exibir_texto_multilinha("Aperte o Botao A", "para sortear um", "numero");
        }

        sleep_ms(10);
    }

    return 0;
}