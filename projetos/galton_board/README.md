# Simulação de Galton Board com Display OLED

Este projeto implementa uma simulação de uma Galton Board (ou Quincunce) usando um microcontrolador RP2040 (BitDogLab) e um display OLED SSD1306. Os pinos são desenhados como pixels brancos fixos no display, e as bolas também são pixels brancos que descem, colidem nos pinos e desviam para a esquerda ou direita com probabilidade de 50% a cada impacto. Após todas as bolas atingirem a base, é gerado um histograma que mostra quantas bolas caíram em cada "gap", evidenciando a distribuição normal esperada.

## Objetivo

- Demonstrar a formação da distribuição normal a partir de um processo binomial simples.
- Praticar manipulação de pixels em display OLED e simulação de eventos aleatórios em microcontrolador.
- Avaliar como a remoção de pinos afeta a curva de distribuição.

## Lista de materiais

| Componente         | Conexão na BitDogLab      |
| ------------------ | ------------------------- |
| BitDogLab (RP2040) | —                         |
| Display OLED I2C   | SDA: GPIO14 / SCL: GPIO15 |

> Não são utilizados botões neste projeto; a simulação inicia automaticamente.

## Execução

1. Abra o projeto no VS Code com suporte ao SDK do Raspberry Pi Pico (CMake + ARM GCC).
2. Compile (Ctrl+Shift+B ou `cmake` + `make`).
3. Conecte a BitDogLab via USB em modo boot (pressione BOOTSEL ao conectar).
4. Copie o `.uf2` gerado para a unidade `RPI-RP2`.
5. A placa reiniciará e a simulação começará:
   - Os pinos são desenhados no display.
   - As bolas iniciam no topo e caem, batendo nos pinos.
   - Ao final, é exibido o histograma de quedas.

## Funcionamento

1. **Desenho do mapa**: a função `draw_map()` posiciona os pinos em linhas espaçadas de forma triangular.
2. **Inicialização das bolas**: `balls_init()` prepara cada bola com um atraso (`TICKS_BETWEEN_BALLS`).
3. **Atualização**: a cada _tick_ (`tick_ms`), cada bola é movida:
   - Ao encontrar um pino, decide-se aleatoriamente (50/50) mover para esquerda ou direita.
   - Quando chega à linha final, incrementa o contador do gap correspondente.
4. **Histograma**: após todas as bolas finalizarem, a tela apresenta barras verticais representando o número de bolas em cada gap. Caso deseje visualizar em porcentagem, defina a constante SHOW_PERCENTAGE em `main.c`.

## Testes Realizados

- **Configuração padrão**: todos os pinos presentes — obtida curva aproximadamente normal.
- **Remoção de pinos**: imagens em `assets/tests/` mostram simulações com pinos faltando em determinadas linhas, evidenciando uma concentração em um ponto mais à esquerda (onde havia mais pinos faltantes). Caso deseje retirar pinos, basta alterar o vetor `pins_removed` em `map.c`.

## Estrutura de arquivos

- `main.c` : loop principal, estado de simulação e renderização.
- `map.c` / `map.h`: definição e desenho dos pinos.
- `ball.c` / `ball.h`: lógica de movimento e colisão das bolas.
- `oled.c` / `oled.h`: funções de desenho no SSD1306.
- `vec2.c` / `vec2.h`: operações com vetores 2D.

## 🖼️ Imagens

| Cenário padrão                     | Pinos removidos                            |
| ---------------------------------- | ------------------------------------------ |
| `assets/histogram.jpeg`            | `assets/tests/missing_pins_histogram.jpeg` |
| `assets/galton_board.jpeg`         | `assets/tests/missing_pins.jpeg`           |
| `assets/histogram_percentage.jpeg` |                                            |

## 📜 Licença

MIT License
