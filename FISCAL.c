#include "ti_msp_dl_config.h"

// Definição de ciclos para atraso de 1ms baseado no clock padrão de 32MHz
// 32000 ciclos @ 32MHz = ~1ms
#define DELAY_1MS (32000)

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
    
    // Contador para o LED 2 (Pisca-Pisca)
    uint32_t contadorPisca = 0;

    while (1) {
        
        // --- MÁQUINA DE ESTADOS (LED 1, LED 2 E BOTÃO PINO 14) ---
        switch (estadoAtual) {
            
            case ESTADO_CONTAGEM_15S:
                // 1. LÓGICA DO PISCA-PISCA DO LED 2 (Ativa apenas neste estado)
                contadorPisca++;
                if (contadorPisca >= 300) { // 300 iterações * 1ms = 300ms (0.3s)
                    DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
                    contadorPisca = 0; 
                }

                // 2. LÓGICA DO BOTÃO E CONTAGEM DO LED 1
                // Se o botão no pino 14 (USER_INPUT) for pressionado, reinicia o temporizador
                if ((DL_GPIO_readPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_INPUT_PIN) & GPIO_LEDS_USER_INPUT_PIN) != 0) {
                    contadorTempoLED1 = 0; 
                } else {
                    contadorTempoLED1++; // Incrementa se o botão NÃO for pressionado
                }

                // Se atingir 15.000 iterações (15 segundos) sem interrupção do botão
                if (contadorTempoLED1 >= 15000) {
                    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN); // Apaga o LED 1
                    
                    // FORÇA O LED 2 A PARAR E APAGAR
                    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
                    contadorPisca = 0; // Reseta o contador do pisca para começar do zero depois
                    
                    contadorTempoLED1 = 0;         // Reseta o contador principal
                    estadoAtual = ESTADO_APAGADO_25S; // Transiciona o estado
                }
                break;

            case ESTADO_APAGADO_25S:
                // O LED 2 NÃO pisca aqui, pois a lógica dele ficou presa dentro do caso anterior.
                
                contadorTempoLED1++; // Incrementa o tempo que o LED 1 passa apagado

                // Se atingir 25.000 iterações (25 segundos)
                if (contadorTempoLED1 >= 25000) {
                    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);  // Liga o LED 1 novamente
                    
                    contadorTempoLED1 = 0;            // Reseta o contador
                    estadoAtual = ESTADO_CONTAGEM_15S; // Retorna, fazendo o LED 2 voltar a piscar
                }
                break;
        }

        // Base de tempo: espera 1 milissegundo antes de rodar o loop novamente
        delay_cycles(DELAY_1MS);
    }
}
