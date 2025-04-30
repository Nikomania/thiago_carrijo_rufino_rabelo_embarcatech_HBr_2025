# Pisca-pisca Modular para Raspberry Pi Pico W

Este projeto demonstra como reestruturar um simples “blink” do LED embutido da Raspberry Pi Pico W em uma arquitetura modular, separando responsabilidades em camadas de driver, HAL (Hardware Abstraction Layer) e aplicação. A compilação é organizada via CMake.

---

## 📋 Descrição

O código original ligava e desligava o LED embutido usando diretamente a API `cyw43_arch`. Aqui, reorganizamos em:

- **drivers/** – acesso direto ao hardware (API `cyw43_arch`).  
- **hal/** – funções genéricas (toggle) que abstraem o driver.  
- **app/** – lógica principal, usa apenas a HAL.  
- **include/** – cabeçalhos para cada módulo.  
- **CMakeLists.txt** – orquestra build modular.  

---

## 🎯 Objetivos

- Isolar o controle de GPIO do LED no driver `led_embutido.c`.  
- Expor uma função simples `hal_led_toggle()` na HAL.  
- Fazer o `app/main.c` apenas chamar `hal_led_toggle()` em loop.  
- Ter build configurado para compilar cada módulo e gerar um firmware `.uf2`.  

---

## 📂 Estrutura do Projeto

```
app/
├── main.c

drivers/
├── led_embutido.c

hal/
├── hal_led.c

include/
├── led_embutido.h
└── hal_led.h

CMakeLists.txt
```

---

## 📖 Detalhes dos Módulos

### drivers/led_embutido.c  
Código que fala diretamente com a API `cyw43_arch` para ligar/desligar o LED.

### include/led_embutido.h  
Protótipo das funções de driver:
```c
void led_embutido_init(void);
void led_embutido_set(bool on);
```

### hal/hal_led.c  
Abstrai o driver; mantém estado interno e fornece toggle:
```c
void hal_led_init(void);
void hal_led_toggle(void);
```

### include/hal_led.h  
Protótipo da HAL:
```c
void hal_led_init(void);
void hal_led_toggle(void);
```

### app/main.c  
Função `main()` minimalista:
```c
int main() {
    stdio_init_all();
    if (cyw43_arch_init()) return -1;
    hal_led_init();
    while (true) {
        hal_led_toggle();
        sleep_ms(500);
    }
}
```

### CMakeLists.txt  
Configura build em camadas, incluindo diretórios `include/`, compilando `drivers/`, `hal/`, `app/` e linkando tudo em um único firmware.

---

## 🚀 Como Compilar e Rodar

1. Instale o SDK da Pico conforme documentação oficial.  
2. No diretório do projeto, crie pasta de build:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
3. Conecte a Pico W em modo BOOTSEL e copie o arquivo `.uf2` gerado para a unidade RPI-RP2.  
4. A Pico reiniciará e o LED começará a piscar em intervalos de 500 ms.

---

## 📜 Licença

Licença MIT. Veja o arquivo `LICENSE` para detalhes.
