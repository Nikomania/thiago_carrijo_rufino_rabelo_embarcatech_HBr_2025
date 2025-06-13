# FreeRTOS Raspberry Pi Pico W em BitDogLab

Este projeto demonstra o uso de FreeRTOS no microcontrolador RP2040 (BitDogLab) com tarefas de LED RGB, buzzer e leitura de botões, estruturado em build, lib e src.

## 📁 Estrutura de Pastas

```
.
├── build/                 # Arquivos gerados pela compilação
├── lib/                   # Bibliotecas externas (ex: FreeRTOS)
├── src/                   # Código-fonte do projeto
│   ├── button.c
│   ├── button.h
│   ├── buzzer.c
│   ├── buzzer.h
│   ├── led_rgb.c
│   ├── led_rgb.h
│   ├── FreeRTOSConfig.h
│   ├── CMakeLists.txt
│   └── main.c
├── CMakeLists.txt         # Configuração de build na raiz
└── pico_sdk_import.cmake
```

## 🔧 Materiais

| Componente         | Conexão / Definição |
| ------------------ | ------------------- |
| BitDogLab (RP2040) | —                   |
| LED RGB            | RED_PIN: GPIO13     |
|                    | GREEN_PIN: GPIO11   |
|                    | BLUE_PIN: GPIO12    |
| Buzzer             | BUZZER_PIN: GPIO21  |
| Botões             | BTN_A: GPIO5        |
|                    | BTN_B: GPIO6        |

## 🚀 Execução

1. Crie e acesse a pasta de build:
   ```sh
   mkdir -p build && cd build
   ```
2. Geração dos arquivos de build com CMake:
   ```sh
   cmake ..
   ```
3. Compile:
   ```sh
   make
   ```
4. Conecte a BitDogLab em modo boot (BOOTSEL pressionado) via USB.
5. Copie o arquivo `.uf2` gerado para a unidade `RPI-RP2`.
6. A placa reiniciará e as tarefas iniciarão automaticamente.

## ⚙️ Funcionamento

- **vTaskLED** (prioridade **2**): alterna as cores do LED RGB a cada 500 ms.
- **vTaskBuzzer** (prioridade **1**): emite um _beep_ de 100 ms a cada 1000 ms.
- **vTaskButton** (prioridade **3**): executada para cada botão em polling a cada 100 ms, com debounce de 50 ms; ao pressionar ou soltar, suspende ou retoma a tarefa associada (LED ou buzzer).

## 🧪 Testes Realizados

- **Sem interação de botões**: LED e buzzer operam simultaneamente conforme prioridades.
- **Pressionar BTN_A**: suspende a tarefa LED; buzzer continua.
- **Pressionar BTN_B**: suspende a tarefa buzzer; LED continua.
- **Debounce**: alterações rápidas de estado não geram múltiplos eventos.

## ❓ Perguntas e Respostas

1. **O que acontece se todas as tarefas tiverem a mesma prioridade?**  
   Todas as tarefas seriam escalonadas de forma round-robin, recebendo **fatias de tempo iguais**, sem preferência de execução.

2. **Qual tarefa consome mais tempo da CPU?**  
   A tarefa de **botão** (vTaskButton) consome mais, pois faz _polling_ de GPIO a cada 100 ms em um loop infinito, além de lógica de debounce.

3. **Quais seriam os riscos de usar polling sem prioridades?**
   - **Desperdício de CPU**: polling frequente ocupa tempo de processamento mesmo sem eventos.
   - **Latência**: tarefas críticas podem sofrer atraso se o polling não ceder CPU.
   - **Ineficiência energética**: maior consumo de energia pelo MCU.

## 📜 Licença

Este projeto está sob a licença MIT.
