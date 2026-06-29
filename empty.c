#include "ti_msp_dl_config.h"

// Definição de ciclos para atraso de 10ms baseado no clock padrão
// 320000 ciclos @ 32MHz = ~10ms
#define DELAY_10MS (32000)

// Definição dos estados do sistema para o LED 1
typedef enum {
    ESTADO_CONTAGEM_15S,
    ESTADO_APAGADO_25S
} EstadoSistema;

int main(void){
    SYSCFG_DL_init();
    
    // Inicialização padrão dos LEDs
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_3_PIN | GPIO_LEDS_USER_TEST_PIN);
    
    // O LED 1 (pino 11) começa SEMPRE aceso
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    
    // O LED 2 (pino 12) começa apagado 
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

    EstadoSistema estadoAtual = ESTADO_CONTAGEM_15S;
    uint32_t contadorTempoLED1 = 0;
    
    // Novo contador para o LED 2 (Pisca-Pisca)
    uint32_t contadorPisca = 0;

    while (1) {
        
        // --- NOVO: LÓGICA DO LED 2 (PISCA-PISCA NÃO BLOQUEANTE) ---
        contadorPisca++;
        if (contadorPisca >= 250) { // 30 iterações * 10ms = 300ms (0.3s)
            // Inverte o estado atual do LED 2 (se está aceso, apaga; se está apagado, acende) 
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
            contadorPisca = 0; // Reseta o contador do pisca
        }

        // --- LÓGICA DA MÁQUINA DE ESTADOS (LED 1 E BOTÃO PINO 14) ---
        switch (estadoAtual) {
            
            case ESTADO_CONTAGEM_15S:
                // Se o botão no pino 14 (USER_INPUT) for pressionado, reinicia o temporizador [cite: 11, 12]
                if ((DL_GPIO_readPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_INPUT_PIN) & GPIO_LEDS_USER_INPUT_PIN) != 0) {
                    contadorTempoLED1 = 0; 
                } else {
                    contadorTempoLED1++; // Incrementa o tempo se o botão não for pressionado
                }

                // 15 segundos = 1500 iterações de 10ms
                if (contadorTempoLED1 >= 1500) {
                    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN); // Apaga o LED 1
                    contadorTempoLED1 = 0;                                          // Reseta o contador
                    estadoAtual = ESTADO_APAGADO_25S;                           // Muda o estado
                }
                break;

            case ESTADO_APAGADO_25S:
                contadorTempoLED1++; // Incrementa o tempo que o LED passa apagado

                // 25 segundos = 2500 iterações de 10ms
                if (contadorTempoLED1 >= 2500) {
                    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);  // Liga o LED 1 novamente
                    contadorTempoLED1 = 0;                                          // Reseta o contador
                    estadoAtual = ESTADO_CONTAGEM_15S;                          // Volta para o estado inicial
                }
                break;
        }

        // Base de tempo: espera 10 milissegundos antes de rodar o loop novamente
        delay_cycles(DELAY_10MS);
    }
}