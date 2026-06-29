O programa gerencia dois LEDs e um botão de forma simultânea e independente através de uma 
Arquitetura Cooperativa Baseada em Tempo:Base de Tempo Estrita (1ms): 
Através da função delay_cycles, o loop principal é sincronizado para rodar 
exatamente a cada 1 milissegundo. Todas as contagens de tempo do software são 
derivadas dessa batida de 1ms.O LED 2 ("Heartbeat"): Ele funciona como um indicador
visual de que o sistema está vivo e operando. A cada 300 iterações (300ms), o código 
inverte o estado do pino 12, fazendo o LED piscar constantemente, independente do que
aconteça com o resto do sistema.O LED 1 (Controle do Botão): Ele inicia aceso. O programa
monitora continuamente o pino 14.Se o botão for pressionado, o contador de tempo é zerado
imediatamente.Se o botão for solto, o contador acumula milissegundos. Se atingir 15.000 
iterações (15 segundos) sem interrupção, o LED 1 apaga e o sistema muda de estado.No estado
"Apagado", o sistema ignora o botão e espera 25.000 iterações (25 segundos) até acender o 
LED 1 novamente e reiniciar o ciclo.Principais Características TécnicasNão-Bloqueante 
(Multitarefa Simulada): A característica mais marcante. Como não existem funções de atraso longo
(como travar o código por 15 segundos), a CPU consegue processar a lógica do pisca-pisca e a 
lógica do botão quase ao mesmo tempo.Máquina de Estados Finitos (FSM): O comportamento do 
LED 1 é dividido em regras claras (ESTADO_CONTAGEM_15S e ESTADO_APAGADO_25S). Isso impede que
o código execute ações inválidas — como apagar o LED que já está apagado ou aceitar comandos
de botão na hora errada.Determinismo: Como o loop roda com um atraso fixo, você sabe exatamente
quando cada linha de código será avaliada, tornando o comportamento do firmware previsível.
Ajuste de Escala Proporcional: Ao mudar a base de tempo para 1ms, as variáveis de contagem do
seu código precisam ser multiplicadas por 10 em relação ao código anterior para manter os mesmos
tempos em segundos ($15\text{s} = 15000\text{ms}$ e $25\text{s} = 25000\text{ms}$), demonstrando
como essa arquitetura é flexível e fácil de ajustar.
