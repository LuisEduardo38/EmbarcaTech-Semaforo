#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/timer.h"

//Declaração das variáveis para os pinos dos leds
const uint8_t led_red_pino = 13;
const uint8_t led_blue_pino = 12;
const uint8_t led_green_pino = 11;

//Declaração das variáveis para monitoramento do semáforo
volatile bool led_red_status = 0;
volatile bool led_blue_status = 0;
volatile bool led_green_status = 0;

//Declaração da variável para o pino do botão A da placa
const uint8_t btn_a = 5;

//Declaração da variável do tipo repeating_timer para uso na função de alarme
struct repeating_timer timer_red, timer_blue, timer_green;

//Declaração dos protótipos das funções
bool alternar_red();
bool alternar_blue();
void iniciar_pinos();
bool alternar_green();
static void gpio_irq_handler(uint gpio, uint32_t events);

int main() {
    //Inicializando a biblioteca e os pinos do código
    stdio_init_all();
    iniciar_pinos();

    //Declaração da interrupção para entra no modo bootsel
    gpio_set_irq_enabled_with_callback(btn_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    //FUNCIONAMENTO DO CÓDIGO
    /*O código funciona por meio de declaração de novos alarmes. Quando um alarme é acionado após a
    sua rotina ser executada, é declarado um novo alarme para realizar a alternação dos estados do
    led com esse novo alarme, tendo seu tempo de 3000 milissegundos, com o loop inifito da troca de
    led se dando pela declaração de novos alarmes.*/

    //Declaração do primeiro alarme do código
    add_repeating_timer_ms(3000, alternar_red, NULL, &timer_red);

    //Loop para exibir o estado do semáforo
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

//Função para configurar os pinos da placa
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

//Função callback que realiza alteração do led vermelho
bool alternar_red(){
    //Alteração das variáveis de estado dos leds
    led_red_status = 1;
    led_blue_status = 0;
    led_green_status = 0;

    //Ligando e desligando os leds via variável.
    gpio_put(led_red_pino, led_red_status);
    gpio_put(led_blue_pino, led_blue_status);
    gpio_put(led_green_pino, led_green_status);

    //Declaração de um novo alarme que irá alterar o estado do led
    add_repeating_timer_ms(3000, alternar_blue, NULL, &timer_blue);

    /*Passando o parâmetro, false para que esse alarme não se repita,
    somente com a nova declaração que irá ocorrer mais para frente no código.*/
    return false;
}

//Função callback que realiza alteração do led azul (amarelo)
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

//Função callback que realiza alteração do led verde
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

//Função para interrupção do código
static void gpio_irq_handler(uint gpio, uint32_t events){
    printf("BTN_A\n");
    reset_usb_boot(0, 0);
}