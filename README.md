# Óculos Assistivo com Feedback Vibratório para Deficientes Visuais

## Introdução

Este projeto visa desenvolver um dispositivo assistivo para pessoas com deficiência visual, utilizando óculos equipados com um sensor ultrassônico e motor de vibração. O objetivo do dispositivo é fornecer feedback tátil para alertar o usuário sobre a presença de obstáculos, ajudando na navegação e prevenção de colisões.

O sistema é baseado em um sensor ultrassônico HC-SR04 que mede a distância até os obstáculos, acionando o motor vibratório de acordo com a proximidade desses obstáculos. A ideia é proporcionar uma solução de baixo custo e fácil implementação para promover maior independência a pessoas com deficiência visual.

## Requisitos

Antes de rodar o projeto, certifique-se de ter os seguintes componentes e softwares:

### Hardware:
- Arduino Nano (ou similar)
- Sensor Ultrassônico HC-SR04
- Motor de Vibração (como os usados em dispositivos móveis)
- Botão para alternância de modos
- Fonte de alimentação (bateria 9V)
- Fios e protoboard (para prototipagem)

### Software:
- Arduino IDE (para programação do Arduino)
- Bibliotecas necessárias: `NewPing` (opcional, se preferir uma biblioteca para o sensor ultrassônico)

## Instalação

1. **Clone este repositório:**
   ```bash
   git clone https://github.com/seu_usuario/oculos-assistivo.git
