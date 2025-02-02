#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h" // Biblioteca para PIO WS2812

// Número de LEDs
#define NUM_PIXELS 25

// Pino de saída
#define OUT_PIN 7

// Pino do LED RGB
#define LED_RGB_PIN 13

// Botões de interrupção
const uint button_a = 5; // Botão A
const uint button_b = 6; // Botão B

// Contador
volatile int counter = 0;

// Debouncing
bool button_a_pressed = false;
bool button_b_pressed = false;

// Intensidade do brilho (0.0 a 1.0)
float intensidade_brilho = 0.9; // Brilho máximo por padrão

// Rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events) {
    if (gpio == button_a) {
        if (!button_a_pressed) {
            button_a_pressed = true; // Marcar como pressionado
            counter = (counter + 1) % 10; // Incrementa o contador
            printf("Botão A pressionado: %d\n", counter); // Debug
        }
    } else if (gpio == button_b) {
        if (!button_b_pressed) {
            button_b_pressed = true; // Marcar como pressionado
            counter = (counter - 1 + 10) % 10; // Decrementa o contador
            printf("Botão B pressionado: %d\n", counter); // Debug
        }
    }
}

// Função para definir a cor dos LEDs com controle de intensidade
uint32_t matrix_rgb(double b, double r, double g, float intensidade) {
    unsigned char R, G, B;
    R = r * 255 * intensidade; // Aplica a intensidade ao vermelho
    G = g * 255 * intensidade; // Aplica a intensidade ao verde
    B = b * 255 * intensidade; // Aplica a intensidade ao azul
    return (G << 24) | (R << 16) | (B << 8);
}

// Função para desenhar números na matriz de LEDs (0-9)
void desenhar_numero_na_matriz(PIO pio, uint sm, int numero) {
    int leds[10][NUM_PIXELS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},//numero 0
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},//numero 1
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},//numero 2
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},//numero 3
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1},//numero 4
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},//numero 5
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},//numero 6
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},//numero 7
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},//numero 8
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1} //numero 9
    };
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, leds[numero][i] ? matrix_rgb(1.0, 0.0, 0.0, intensidade_brilho) : matrix_rgb(0.0, 0.0, 0.0, intensidade_brilho));
    }
}

// Função principal
int main() {
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    stdio_init_all();

    // Configuração dos botões
    gpio_init(button_a);
    gpio_set_dir(button_a, GPIO_IN);
    gpio_pull_up(button_a);
    
    gpio_init(button_b);
    gpio_set_dir(button_b, GPIO_IN);
    gpio_pull_up(button_b);

    // Configuração do LED RGB
    gpio_init(LED_RGB_PIN);
    gpio_set_dir(LED_RGB_PIN, GPIO_OUT);

    // Configuração das interrupções
    gpio_set_irq_enabled_with_callback(button_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    desenhar_numero_na_matriz(pio, sm, counter);

    bool led_state = false; // Estado do LED
    while (true) {
        desenhar_numero_na_matriz(pio, sm, counter);
    
        // Piscar LED vermelho
        gpio_put(LED_RGB_PIN, led_state ? 1 : 0);
        led_state = !led_state; // Alterna o estado
        sleep_ms(200); // Controla a frequência de piscar (5 vezes por segundo)

        // Resetar o estado dos botões após debouncing
        if (gpio_get(button_a)) {
            button_a_pressed = false;
        }
        if (gpio_get(button_b)) {
            button_b_pressed = false;
        }
    }

    return 0;
}