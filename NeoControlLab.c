#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "neopixel_driver.h"
#include "testes_cores.h"
#include "efeitos.h"
#include "efeito_curva_ar.h"
#include "numeros_neopixel.h"

#define LED_PIN 7      // Já definido no seu projeto (fita LED)
#define BUTTON_A 5      // GPIO do botão A na BitDogLab

volatile bool gerar_numeros = false;

// Função chamada pela interrupção no botão A
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL)) {
        gerar_numeros = true;
    }
}

// Inicializa o sistema e a matriz NeoPixel
void setup() {
    stdio_init_all();
    sleep_ms(1000);  // Aguarda conexão USB (opcional)

    // Inicializa o botão A com interrupção
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Inicializa NeoPixel e random seed
    npInit(LED_PIN);
    srand(time_us_32());
}

// Sorteia número inteiro entre [min, max]
int sorteia_entre(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Exibe o número sorteado de 1 a 6
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

    while (true) {
        if (gerar_numeros) {
            gerar_numeros = false;  // Reseta flag

            int vezes = sorteia_entre(100, 500);
            printf("Mostrando %d numeros aleatorios...\n", vezes);

            for (int i = 0; i < vezes; i++) {
                int n = sorteia_entre(1, 6);
                printf("Numero sorteado: %d\n", n);
                mostrar_numero_sorteado(n);
                sleep_ms(10);
            }
        }

        sleep_ms(10);  // Evita busy-waiting
    }

    return 0;
}
