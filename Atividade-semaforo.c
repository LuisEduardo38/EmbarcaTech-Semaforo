#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

const uint8_t led_red_pino = 13;
const uint8_t led_blue_pino = 12;
const uint8_t led_green_pino = 11;

struct repeating_timer timer_red, timer_blue, timer_green;

bool alternar_red();
bool alternar_blue();
void iniciar_pinos();
bool alternar_green();

int main() {
    stdio_init_all();
    iniciar_pinos();

    add_repeating_timer_ms(3000, alternar_red, NULL, &timer_red);

    while (true) {
        printf("Status do Semáforo\n");
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
}

bool alternar_red(){
    gpio_put(led_red_pino, 1);
    gpio_put(led_blue_pino, 0);
    gpio_put(led_green_pino, 0);

    add_repeating_timer_ms(3000, alternar_blue, NULL, &timer_blue);

    return false;
}

bool alternar_blue(){
    gpio_put(led_red_pino, 1);
    gpio_put(led_blue_pino, 0);
    gpio_put(led_green_pino, 1);
    
    add_repeating_timer_ms(3000, alternar_green, NULL, &timer_green);
    
    return false;
} 

bool alternar_green(){
    gpio_put(led_red_pino, 0);
    gpio_put(led_blue_pino, 0);
    gpio_put(led_green_pino, 1);
    
    add_repeating_timer_ms(3000, alternar_red, NULL, &timer_red);
    
    return false;
}