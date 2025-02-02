# Projeto de Interrupções e Controle de LEDs com RP2040 e BitDogLab

## Descrição do Projeto

Este projeto foi desenvolvido para consolidar a compreensão do uso de interrupções em microcontroladores, especificamente no RP2040, e explorar as funcionalidades da placa de desenvolvimento BitDogLab. O sistema combina hardware e software para criar uma aplicação interativa utilizando LEDs e botões.

## Objetivos

- Compreender o funcionamento e a aplicação de interrupções em microcontroladores.
- Implementar a correção do fenômeno de bouncing em botões através de debouncing via software.
- Controlar LEDs comuns e LEDs endereçáveis WS2812.
- Utilizar resistores de pull-up internos em botões de acionamento.
- Desenvolver um projeto funcional que integra hardware e software.

## Componentes Utilizados

- **Matriz 5x5 de LEDs (endereçáveis) WS2812**: Conectada à GPIO 7.
- **LED RGB**: Com os pinos conectados às GPIOs 11, 12 e 13.
- **Botão A**: Conectado à GPIO 5.
- **Botão B**: Conectado à GPIO 6.

## Funcionalidades

1. **Piscar do LED Vermelho**: O LED vermelho do LED RGB pisca continuamente a uma taxa de 5 vezes por segundo.
2. **Incremento com Botão A**: O botão A, ao ser pressionado, incrementa o número exibido na matriz de LEDs.
3. **Decremento com Botão B**: O botão B, ao ser pressionado, decrementar o número exibido na matriz de LEDs.
4. **Efeitos Visuais com LEDs WS2812**: Os LEDs WS2812 são utilizados para criar efeitos visuais que representam números de 0 a 9. Cada número é exibido na matriz em um formato fixo, utilizando caracteres em estilo digital ou um estilo criativo, desde que o número seja claramente identificável.

## Requisitos do Projeto

- **Uso de Interrupções**: Todas as funcionalidades relacionadas aos botões foram implementadas utilizando rotinas de interrupção (IRQ).
- **Debouncing**: O tratamento do bouncing dos botões foi implementado via software.
- **Controle de LEDs**: O projeto demonstra o controle de diferentes tipos de LEDs, incluindo LEDs comuns e endereçáveis.
- **Organização do Código**: O código está estruturado e comentado para facilitar o entendimento.

## Demonstração

Um vídeo de demonstração de até 2 minutos está disponível para mostrar as funcionalidades do projeto em ação. 

## Conclusão

Este projeto não apenas reforçou o entendimento sobre interrupções e debouncing, mas também mostrou a versatilidade da placa BitDogLab na criação de aplicações interativas e visuais com LEDs.