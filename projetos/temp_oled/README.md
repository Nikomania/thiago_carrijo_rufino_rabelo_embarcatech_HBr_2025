# Monitor da temperatura interna da MCU

Este projeto foi desenvolvido para a Raspberry Pi Pico W instalada na BitDogLab e tem como objetivo ler a temperatura ambiente através de um sensor de temperatura e exibi-la em tempo real em um display OLED. O código-fonte está presente em `main.c` e demonstra a integração entre leitura analógica e comunicação I2C.

## Objetivo

- Realizar a leitura da temperatura ambiente por meio de um sensor específico.
- Exibir, em tempo real, a temperatura medida no display OLED.
- Demonstrar a utilização dos recursos de entrada analógica (ADC) e exibição gráfica via I2C na Raspberry Pi Pico W.

## Lista de Materiais e Conexões

| Componente                | Conexão na BitDogLab (RP2040 Pico W) |
| ------------------------- | ------------------------------------ |
| BitDogLab (RP2040 Pico W) | -                                    |
| Sensor de Temperatura     | Canal 4 de ADC                       |
| Display OLED I2C          | SDA: GPIO14 / SCL: GPIO15            |

### Execução

1. Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make);
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);
5. A Pico reiniciará automaticamente e começará a executar o código;
6. O display OLED irá mostrar os valores de temperatura do sensor em celsius ou fahrenheit.

## Lógica do Projeto

- **Inicialização:**  
  O código configura os módulos de ADC para a leitura do sensor de temperatura e o I2C para a comunicação com o display OLED.

- **Leitura e Processamento:**  
  O sensor de temperatura é lido através do conversor analógico-digital (ADC).  
  A leitura é convertida para uma escala de temperatura em graus (°C ou °F, conforme a calibração), que pode ser exibida de forma legível no display.

- **Exibição:**  
  Os valores de temperatura são atualizados continuamente no display OLED, permitindo monitoramento em tempo real.

## Arquivos

- `main.c` – Código-fonte principal que contém toda a lógica para leitura do sensor, processamento dos dados e exibição no display OLED.
- `assets/init_state.jpg` - Foto demonstrando o funcionamento do programa

---

## 🖼️ Imagens do Projeto

### Caso base no OLED

![init_state](./assets/init_state.jpg)

---

## 📜 Licença

MIT License - MIT GPL-3.0.
