#include <stdio.h>
#include "pico/stdlib.h"

const uint8_t led_red_pino = 11;
const uint8_t led_blue_pino = 12;
const uint8_t led_green_pino = 13;

static bool estado_red = false;
static bool estado_blue = false;
static bool estado_green = false;

void iniciar_pinos();

int main()
{
    stdio_init_all();
    iniciar_pinos();

    struct repeating_timer timer;

    add_repeating_timer_ms(-3000, alterna_led_red, NULL, &timer);
    add_repeating_timer_ms(-6000, alterna_led_red, NULL, &timer);
    add_repeating_timer_ms(-9000, alterna_led_red, NULL, &timer);

    while (true) {
        if((estado_red == true) || (estado_blue == true) || (estado_green == true)){

        }
        else{
            printf("Passou 1 segundo.\n");
        }
    }
}

void iniciar_pinos(){
    gpio_init(led_red_pino);
    gpio_init(led_blue_pino);
    gpio_init(led_red_pino);
    gpio_set_dir(led_red_pino, GPIO_OUT);
    gpio_set_dir(led_blue_pino, GPIO_OUT);
    gpio_set_dir(led_green_pino, GPIO_OUT);
}

void alterna_led_red(){
    if((estado_red == false) || (estado_blue == false) || (estado_green == false)){

    }

    return true;
}

void alterna_led_blue(){
    
    return true;
}

void alterna_led_green(){
    
    return true;
}