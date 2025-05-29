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
#include "display_oled.h"  // Arquivo com funções para controle do display OLED

// Define o pino conectado à matriz de LEDs WS2812B
#define LED_PIN 7

// Define o pino do botão A da placa BitDogLab
#define BUTTON_A 5

// Flag global controlada pela interrupção do botão
volatile bool gerar_numeros = false;

// Callback de interrupção do botão A (aciona na borda de descida)
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL)) {
        gerar_numeros = true;  // Sinaliza que o sorteio deve ocorrer
    }
}

// Função de configuração inicial do sistema
void setup() {
    stdio_init_all();         // Inicializa entrada/saída padrão (debug via USB)
    sleep_ms(1000);           // Aguarda 1 segundo (tempo para estabilizar USB)

    // Configura o botão A como entrada com pull-up e interrupção
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    npInit(LED_PIN);          // Inicializa a fita NeoPixel
    init_display();           // Inicializa o display OLED

    srand(time_us_32());      // Semente para geração de números aleatórios

    // Exibe mensagem inicial no display
    exibir_texto_multilinha("Aperte o Botao A", "para sortear um", "numero");
}

// Gera um número aleatório entre os valores fornecidos (inclusive)
int sorteia_entre(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Exibe o número sorteado na matriz de LEDs (em formato gráfico)
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
    setup();  // Configura todos os periféricos
    char texto[32];  // Buffer para converter número sorteado em string

    while (true) {
        if (gerar_numeros) {
            gerar_numeros = false;  // Limpa a flag de sorteio

            int vezes = sorteia_entre(100, 500);  // Define quantas vezes sortear
            printf("Sorteando %d numeros...\n", vezes);

            // Executa o sorteio animado
            for (int i = 0; i < vezes; i++) {
                int n = sorteia_entre(1, 6);  // Gera número entre 1 e 6
                printf("Numero: %d\n", n);

                snprintf(texto, sizeof(texto), "%d", n);  // Converte para texto
                exibir_texto_centralizado(texto);         // Exibe no display OLED

                mostrar_numero_sorteado(n);               // Exibe na matriz NeoPixel
                sleep_ms(10);                             // Pequeno delay visual
            }

            // Exibe novamente a mensagem de instrução no fim do sorteio
            exibir_texto_multilinha("Aperte o Botao A", "para sortear um", "numero");
        }

        sleep_ms(10);  // Pequeno delay no loop principal para reduzir uso de CPU
    }

    return 0;
}
