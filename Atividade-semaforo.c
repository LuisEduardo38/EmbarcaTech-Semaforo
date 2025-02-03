#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/timer.h"

const uint8_t led_red_pino = 13;
const uint8_t led_blue_pino = 12;
const uint8_t led_green_pino = 11;

volatile bool led_red_status = 0;
volatile bool led_blue_status = 0;
volatile bool led_green_status = 0;

const uint8_t btn_a = 5;

struct repeating_timer timer_red, timer_blue, timer_green;

bool alternar_red();
bool alternar_blue();
void iniciar_pinos();
bool alternar_green();
static void gpio_irq_handler(uint gpio, uint32_t events);

int main() {
    stdio_init_all();
    iniciar_pinos();

    gpio_set_irq_enabled_with_callback(btn_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    add_repeating_timer_ms(3000, alternar_red, NULL, &timer_red);

    while (true) {
        printf("Status do Semáforo\n\n");
        if((led_red_status == 1) && (led_green_status == 1)){
            printf("Vermelho: 0\n");
            printf("Amarelo: 1\n");
            printf("Verde: 0\n");
        }
        else{
            printf("Vermelho: %d\n", led_red_status);
            printf("Amarelo: 0\n");
            printf("Verde: %d\n", led_green_status);
        }
        sleep_ms(1000);
    }

    return 0;
}

void iniciar_pinos(){
    //Iniciando leds
    gpio_init(led_red_pino);
    gpio_init(led_blue_pino);
    gpio_init(led_green_pino);
    gpio_set_dir(led_red_pino, GPIO_OUT);
    gpio_set_dir(led_blue_pino, GPIO_OUT);
    gpio_set_dir(led_green_pino, GPIO_OUT);

    //Iniciando botão
    gpio_init(btn_a);
    gpio_set_dir(btn_a, GPIO_IN);
    gpio_pull_up(btn_a);
}

bool alternar_red(){
    led_red_status = 1;
    led_blue_status = 0;
    led_green_status = 0;

    gpio_put(led_red_pino, led_red_status);
    gpio_put(led_blue_pino, led_blue_status);
    gpio_put(led_green_pino, led_green_status);

    add_repeating_timer_ms(3000, alternar_blue, NULL, &timer_blue);

    return false;
}

bool alternar_blue(){
    led_red_status = 1;
    led_blue_status = 0;
    led_green_status = 1;

    gpio_put(led_red_pino, led_red_status);
    gpio_put(led_blue_pino, led_blue_status);
    gpio_put(led_green_pino, led_green_status);
    
    add_repeating_timer_ms(3000, alternar_green, NULL, &timer_green);
    
    return false;
} 

bool alternar_green(){
    led_red_status = 0;
    led_blue_status = 0;
    led_green_status = 1;

    gpio_put(led_red_pino, led_red_status);
    gpio_put(led_blue_pino, led_blue_status);
    gpio_put(led_green_pino, led_green_status);
    
    add_repeating_timer_ms(3000, alternar_red, NULL, &timer_red);
    
    return false;
}

static void gpio_irq_handler(uint gpio, uint32_t events){
    printf("BTN_A\n");
    reset_usb_boot(0, 0);
}