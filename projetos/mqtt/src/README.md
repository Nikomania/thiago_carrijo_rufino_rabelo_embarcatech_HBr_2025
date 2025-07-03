# IoT Security Lab com Raspberry Pi Pico W (BitDogLab)

Este projeto implementa um laboratório de segurança IoT utilizando uma placa Raspberry Pi Pico W em uma plataforma BitDogLab. O foco é demonstrar a detecção de replay attacks em mensagens MQTT, utilizando criptografia XOR básica para ofuscação e timestamp para validar a ordem das mensagens. Dois dispositivos BitDogLab podem interagir entre si, publicando e assinando tópicos MQTT, permitindo observar a detecção de replays e a comunicação simultânea.

## Objetivo

- Mostrar como detectar replay attacks em um cenário MQTT simples, usando timestamps.
- Demonstrar o uso de conectividade Wi-Fi no Raspberry Pi Pico W e interação via MQTT.
- Praticar a ofuscação/decifração de mensagens com cifra XOR.
- Explorar a comunicação entre duas BitDogLabs publicando e assinando dados em um broker MQTT.
- Capturar e analisar pacotes de rede com Wireshark para verificar o comportamento das publicações.

## Lista de materiais

| Componente                           | Conexão / Observações                              |
| ------------------------------------ | -------------------------------------------------- |
| BitDogLab (RP2040)                   | —                                                  |
| Raspberry Pi Pico W                  | Inserido na BitDogLab via conector padrão          |
| Broker MQTT (Mosquitto, por exemplo) | Servidor local ou remoto acessível à rede          |
| Rede Wi-Fi                           | SSID e senha configuráveis em `iot_security_lab.c` |
| USB para energia e depuração serial  | Usado para carregamento e logs via `stdio`         |

> Não há botões ou sensores externos; todas as interações se dão via rede e console serial.

## Execução

1. Abra o projeto no VS Code com suporte ao SDK do Raspberry Pi Pico (CMake + ARM GCC).
2. Configure o SSID e a senha da sua rede Wi-Fi em `iot_security_lab.c` (constantes `WIFI_SSID` e `WIFI_PASSWORD`).
3. Ajuste, se necessário, o IP do broker MQTT, porta, usuário e senha (constantes `IP`, `PORT`, `MOSQUITTO_USER` e `MOSQUITTO_PASSWORD` em `iot_security_lab.c`).
4. Compile o projeto (Ctrl+Shift+B ou via terminal: `mkdir build && cd build && cmake .. && make`).
5. Conecte a BitDogLab com a Pico W em modo boot (pressione BOOTSEL ao conectar via USB).
6. Copie o arquivo `.uf2` gerado para a unidade `RPI-RP2`. A placa reiniciará e o programa começará a executar.

## Funcionamento

1. **Conexão Wi-Fi**

   - A função `connect_to_wifi()` (em `wifi_conn.c`) inicializa o chip CYW43 e tenta conectar à rede configurada. Caso não consiga, fica em loop imprimindo erros no console serial.

2. **Configuração MQTT**

   - Em `iot_security_lab.c`, após estabilização do `stdio`, chama-se `mqtt_setup()` (em `mqtt_comm.c`) com ID do cliente, IP do broker, porta, usuário e senha.
   - O callback `mqtt_connection_cb` atualiza a flag `is_connected` e imprime sucesso ou falha.

3. **Cifra XOR e Formato de Mensagem**

   - Sempre que se envia uma mensagem, ela pode ser cifrada com a função `xor_encrypt()` (em `xor_cipher.c`), usando a chave única definida em `XOR_CIPHER_H` (valor `42`).
   - O formato do payload deve ser JSON simples:
     ```json
     {"valor":<float>,"ts":<timestamp>}
     ```
     - `valor`: dado numérico a ser transmitido (por exemplo, temperatura).
     - `ts`: timestamp (em _unsigned long_) indicando o momento de geração da mensagem.

4. **Publicação (Publisher)**

   - Se a macro `SUBSCRIBER` não estiver definida em `iot_security_lab.c`, o código entra no modo publisher:
     - Gera um payload `"26.5"` (ou qualquer valor desejado).
     - Aplica `xor_encrypt()` e publica em `escola/sala1/temperatura` a cada 5 segundos.
     - Imprime no console serial o status da publicação via callback `mqtt_pub_request_cb()`.

5. **Assinatura e Detecção de Replay (Subscriber)**

   - Com `#define SUBSCRIBER` ativo, o dispositivo se inscreve em `escola/sala1/temperatura` usando `mqtt_comm_subscribe()`.
   - Quando chega uma mensagem, `mqtt_on_incoming_publish()` é chamado, salvando o tópico. Em seguida, `mqtt_on_message()` recebe o payload cifrado:
     - Aplica `xor_encrypt()` para decifrar.
     - Faz `sscanf(msg_decrypted, "{\"valor\":%f,\"ts\":%lu}", &value, &new_timestamp)`.
     - Se `new_timestamp > last_timestamp_received`, considera mensagem legítima, atualiza `last_timestamp_received` e marca `has_new_value`.
     - Caso contrário, imprime `"Replay detectado (ts: <novo> <= <antigo>)"` e descarta o valor.

6. **Interação entre duas BitDogLabs**
   - Dois dispositivos podem ser programados, um no modo publisher e outro no modo subscriber, ou ambos alternarem papéis.
   - Em `sub_pub_double_bitdoglab.png` observa-se uma demonstração de publicação e assinatura simultânea entre duas BitDogLabs, permitindo avaliar latência e detecção de replay em ambiente onde ambos enviam dados.

## Testes Realizados

- **Cenário padrão** (sem ataque de replay):

  - Um dispositivo publica a cada 5 segundos um JSON válido com timestamp crescente.
  - O subscriber imprime no console:
    ```
    Novo tópico recebido: escola/sala1/temperatura
    Novo valor recebido: 26.50 (timestamp: 171XXYYYY)
    ```
  - Não há mensagens de replay.
  - Captura no Wireshark confirma que os pacotes RAW (cifrados) trafegam na porta 1883. :contentReference[oaicite:0]{index=0}

- **Ataque de replay**:

  - Simulou-se publicação de uma mensagem com timestamp antigo (por exemplo, reenviando manualmente via `mosquitto_pub -m "{\"valor\":26.5,\"ts\":1600000000}\"`).
  - O subscriber detectou:
    ```
    Replay detectado (ts: 1600000000 <= 171XXYYYY)
    ```
  - A imagem `replay.png` mostra a saída no console com a detecção do replay.

- **Interação de duas BitDogLabs**:

  - Dois dispositivos configurados ambos como publisher/subscriber, alternando envios e recepções.
  - Validou-se que ambos imprimem corretamente os valores e timestamps, sem replays falsos.
  - A figura `sub_pub_double_bitdoglab.png` ilustra a troca de mensagens entre os dois dispositivos.

- **Capturas no Wireshark**:
  - `wireshark.png` mostra as publicações periódicas cifradas na rede (payloads aparentes como bytes aleatórios).
  - `wireshark_double_bitdoglab.png` exibe pacotes de dois clientes MQTT diferentes publicando quase simultaneamente no broker.

## Estrutura de arquivos

- `iot_security_lab.c`  
  Código principal: inicializa Wi-Fi, configura MQTT, alterna entre modo publisher e subscriber, processa mensagens e detecta replay.

- `mqtt_comm.c` / `mqtt_comm.h`  
  Implementa as funções de conexão, publicação e assinatura MQTT, callbacks de confirmação e recebimento de mensagens.

- `wifi_conn.c` / `wifi_conn.h`  
  Função `connect_to_wifi()`: inicializa e conecta o chip CYW43 à rede Wi-Fi.

- `xor_cipher.c` / `xor_cipher.h`  
  Função `xor_encrypt()`: aplica cifra XOR simples de 1 byte para ofuscar/decifrar mensagens.

- `lwipopts.h`  
  Configurações do LWIP para suporte a sockets TCP/IP e MQTT no Pico W.

- Pasta `assets/`
  - `replay.png`
  - `sub_pub_double_bitdoglab.png`
  - `wireshark.png`
  - `wireshark_double_bitdoglab.png`  
    → Imagens ilustrativas dos testes e capturas de rede.

## 🖼️ Imagens

| Replay detectado    | Sub/Pub dupla BitDogLab               | Publicações na rede    | Captura dupla no Wireshark              |
| ------------------- | ------------------------------------- | ---------------------- | --------------------------------------- |
| `assets/replay.png` | `assets/sub_pub_double_bitdoglab.png` | `assets/wireshark.png` | `assets/wireshark_double_bitdoglab.png` |

- **replay.png**: Mostra no console serial a detecção de uma mensagem com timestamp atrasado (replay).
- **sub_pub_double_bitdoglab.png**: Ilustra a interação simultânea entre duas BitDogLabs publicando e assinando tópicos MQTT.
- **wireshark.png**: Captura de pacotes MQTT cifrados transitando na porta 1883, mostrando payloads e cabeçalhos MQTT.
- **wireshark_double_bitdoglab.png**: Exemplo de captura de rede em um cenário com dois clientes MQTT enviando quase simultaneamente.

## 📜 Licença

```text
MIT License

Copyright (c) 2025
```
