# Gravador e Reprodutor de Áudio para Raspberry Pi Pico W na BitDogLab

Este projeto implementa um gravador e reprodutor de áudio utilizando o microcontrolador Raspberry Pi Pico W integrado em uma placa BitDogLab, juntamente com um microfone e um buzzer. O áudio é amostrado via ADC, armazenado em um buffer via DMA, filtrado com um filtro passa-baixa e reproduzido pelo buzzer.

## Objetivos

- Capturar áudio do ambiente através de um microfone analógico.
- Armazenar amostras de áudio em buffer utilizando DMA para eficiência.
- Aplicar um filtro passa-baixa simples para suavizar o sinal.
- Reproduzir o áudio gravado em tempo real ou sob demanda via buzzer.
- Exibir a forma de onda em tempo real em um display OLED SSD1306.

## Materiais Necessários

| Componente                  | Conexão na BitDogLab        |
| --------------------------- | --------------------------- |
| BitDogLab (RP2040 / Pico W) | —                           |
| Microfone analógico         | ADC Channel 2 (GPIO 28)     |
| Buzzer (Piezo)              | GPIO 21                     |
| Display OLED SSD1306 (I2C)  | SDA: GPIO 14 / SCL: GPIO 15 |
| Cabos Jumper                | —                           |

> **Observação:** os botões A e B da BitDogLab são usados para controlar gravação e reprodução.

## Estrutura de Arquivos

```
├── CMakeLists.txt
├── main.c
├── inc
│   ├── oled.h
│   ├── gpio.h
│   ├── ssd1306_font.h
│   ├── ssd1306_i2c.h
│   └── ssd1306.h
├── src
│   ├── oled.c
│   ├── ssd1306_i2c.c
│   └── gpio.c
└── README.md
```

- **CMakeLists.txt**: configura o projeto para o Pico SDK.
- **main.c**: lógica principal de inicialização, gravação (ADC + DMA), filtragem, exibição da forma de onda no OLED e loop de controle.
- **gpio.c / gpio.h**: configuração de GPIO, tratamento de botões com debounce.
- **oled.c / oled.h**: funções de desenho e renderização no display SSD1306 via I2C.
- **ssd1306_i2c.c / ssd1306_i2c.h**: driver de baixo nível para comunicação I2C com o SSD1306.

## Instalação e Compilação

1. Instale o [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) e configure as variáveis de ambiente.
2. Conecte a placa BitDogLab em modo BOOTSEL (pressione BOOTSEL ao conectar via USB).
3. No diretório do projeto, execute:

   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

4. Copie o arquivo `.uf2` gerado para a unidade `RPI-RP2`.
5. A placa reiniciará e o firmware será executado automaticamente.

## Uso

1. Após reinicializar, aguarde mensagem de inicialização no monitor serial (115200 baud).
2. Pressione o **Botão A** para iniciar a gravação de 2 segundos de áudio.

   - Durante a gravação, o LED vermelho pisca.

3. Após a gravação, a forma de onda é exibida em seções no display OLED.
4. Pressione o **Botão B** para reproduzir o áudio gravado no buzzer.

   - O LED verde pisca durante a reprodução.

5. Para gravar novamente, pressione o **Botão A** novamente.

## Parâmetros Configuráveis

- `TOTAL_SECONDS_RECORD`: duração da gravação em segundos.
- `SAMPLE_RATE`: frequência de amostragem (padrão 44100 Hz).
- `coef[]`: coeficientes do filtro passa-baixa.
- `GAP_OLED_AUDIO`: espaçamento horizontal entre pixels no OLED.

## Detalhes de Implementação

- **ADC + DMA**: o ADC captura 12 bits de resolução, enviado ao buffer via DMA para não bloquear a CPU.
- **Filtro Passa-Baixa**: média móvel simples de 5 amostras (coeficiente 0.2 cada).
- **Display OLED**: a cada intervalo, uma seção da forma de onda é desenhada usando `draw_line_OLed()`.
- **Buzzer PWM**: o buffer filtrado é reproduzido ajustando o duty cycle de um PWM configurado para `SAMPLE_RATE`.

## Licença

Este projeto está licenciado sob a Licença MIT. Sinta-se à vontade para usar, modificar e distribuir.
