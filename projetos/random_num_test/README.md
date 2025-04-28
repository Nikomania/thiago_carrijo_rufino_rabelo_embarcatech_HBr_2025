# Test de Aleatoriedade com Display OLED

Este projeto simples verifica a imparcialidade da função `rand_0_1()` (baseada em `get_rand_32() & 1`) em um microcontrolador RP2040 (BitDogLab) usando um display OLED SSD1306. A cada ciclo de tempo, o programa gera um valor randômico (0 ou 1), exibe-o no OLED e atualiza contadores separados para zeros e uns.

## Objetivo

- Testar a credibilidade e uniformidade da função random bit (`rand_0_1()`).
- Exibir em tempo real o valor gerado e o total de ocorrências de 0 e 1 no display OLED.

## Componentes e Conexões

| Componente         | Conexão na BitDogLab      |
| ------------------ | ------------------------- |
| BitDogLab (RP2040) | —                         |
| Display OLED I2C   | SDA: GPIO14 / SCL: GPIO15 |

## Arquivos do Projeto

- `main.c` — Código principal que gera bits aleatórios, atualiza contadores e desenha no OLED.
- `oled.c` / `oled.h` — Rotinas de inicialização, desenho de texto e renderização no display SSD1306.

## Como Executar

1. Abra o projeto no VS Code configurado para Raspberry Pi Pico (CMake + compilador ARM).
2. Compile (Ctrl+Shift+B ou via terminal com `cmake` + `make`).
3. Conecte o BitDogLab via USB em modo BOOTSEL.
4. Copie o `.uf2` gerado para a unidade `RPI-RP2`.
5. O Pico reiniciará e iniciará a simulação.

## Funcionamento

1. Inicializa OLED e limpa a tela.
2. Entra em loop infinito que, a cada `tick_ms` milissegundos:
   - Chama `rand_0_1()` para obter 0 ou 1.
   - Incrementa `count_0` ou `count_1` conforme resultado.
   - Converte números em strings alinhadas ao centro (funções `get_c_string_from_int` e `center_c_str`).
   - Monta um array de linhas de texto e chama `print_lines_OLed(...)`.
   - Renderiza e limpa o display para o próximo ciclo.

## Funções Principais

- `int rand_0_1()` — Retorna 0 ou 1 extraindo o bit menos significativo de `get_rand_32()`.
- `void center_c_str(char* str, int length)` — Centraliza uma string em um buffer de até 16 caracteres.
- `void get_c_string_from_int(int n, char* str)` — Converte inteiro em string sem terminação explícita.
- `int count_digits_10(int n)` — Calcula dígitos decimais de um inteiro.

## Personalização

- Ajuste `tick_ms` para alterar a frequência de geração de bits.
- Altere dimensões `MAX_CHAR` em `oled.h` para suportar outros tamanhos de texto.

## Licença

MIT License — use livremente, modifique e distribua.
