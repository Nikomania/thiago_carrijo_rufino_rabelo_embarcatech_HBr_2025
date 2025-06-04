#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "oled.h"
#include "gpio.h"

// Pino e canal do microfone no ADC.
#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)

#define BUZZER_PIN 21

#define SHOW_TIME_US 1000000

// Parâmetros e macros do ADC.
#define SAMPLE_RATE 44100
#define ADC_CLOCK_DIV 48000000 / SAMPLE_RATE
#define SAMPLES SAMPLE_RATE / 2 // Número de amostras que serão feitas do ADC.
#define ADC_ADJUST(x) (x * 3.3f / (1 << 8u)) // Ajuste do valor do ADC para Volts.
#define ADC_MAX 3.3f
#define ADC_STEP (3.3f/5.f) // Intervalos de volume do microfone.
#define TOTAL_SECONDS_RECORD 2
#define TOTAL_SAMPLES_RECORD (SAMPLE_RATE * TOTAL_SECONDS_RECORD) // Total de amostras que serão feitas do ADC.

#define GAP_OLED_AUDIO 0
#define TOTAL_SECTIONS TOTAL_SAMPLES_RECORD / (ssd1306_width / (GAP_OLED_AUDIO + 1))

// Pino e número de LEDs da matriz de LEDs.
// #define LED_PIN 7
// #define LED_COUNT 25

#define abs(x) ((x < 0) ? (-x) : (x))

// Canal e configurações do DMA
uint8_t dma_channel;
dma_channel_config dma_cfg;

// Buffer de amostras do ADC.
uint8_t adc_buffer[TOTAL_SAMPLES_RECORD];
uint32_t pwm_top;

const float coef[5] = {0.2, 0.2, 0.2, 0.2, 0.2}; // Exemplo simples

void sample_mic();
void pwm_init_buzzer(uint pin);
void buzzer_set(uint pin, uint8_t sample);
void apply_low_pass(uint8_t *input, uint8_t *output, size_t len);
void reproduce_samples(uint8_t *samples, size_t len);
void draw_wave(uint8_t *samples, size_t len, int section, int gap);

int main() {
  stdio_init_all();

  // Delay para o usuário abrir o monitor serial...
  sleep_ms(5000);

  init_gpio();

  init_OLed();
  clear_OLed();
  render_OLed();

  pwm_init_buzzer(BUZZER_PIN);

  // Preparação do ADC.
  printf("Preparando ADC...\n");

  adc_gpio_init(MIC_PIN);
  adc_init();
  adc_select_input(MIC_CHANNEL);

  adc_fifo_setup(
    true, // Habilitar FIFO
    true, // Habilitar request de dados do DMA
    1, // Threshold para ativar request DMA é 1 leitura do ADC
    false, // Não usar bit de erro
    true // Não fazer downscale das amostras para 8-bits, manter 12-bits.
  );

  adc_set_clkdiv(ADC_CLOCK_DIV);

  printf("ADC Configurado!\n\n");

  printf("Preparando DMA...");

  // Tomando posse de canal do DMA.
  dma_channel = dma_claim_unused_channel(true);

  // Configurações do DMA.
  dma_cfg = dma_channel_get_default_config(dma_channel);

  channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8); // Tamanho da transferência é 16-bits (usamos uint16_t para armazenar valores do ADC)
  channel_config_set_read_increment(&dma_cfg, false); // Desabilita incremento do ponteiro de leitura (lemos de um único registrador)
  channel_config_set_write_increment(&dma_cfg, true); // Habilita incremento do ponteiro de escrita (escrevemos em um array/buffer)
  
  channel_config_set_dreq(&dma_cfg, DREQ_ADC); // Usamos a requisição de dados do ADC

  // Amostragem de teste.
  printf("Amostragem de teste...\n");
  sample_mic();

  printf("Config done!\n");
  printf("Press button A to start recording samples!\n");
  while (!get_button_state(&btn_A_state)) {
    sleep_ms(200); // Espera o botão ser pressionado.
  }

  printf("\n----\nInitializing loop...\n----\n");
  while (true) {
    // printf("\n----SAMPLES----\n");
    
    // Realiza uma amostragem do microfone.

    gpio_put(RED_LED_PIN, 1); // Liga o LED vermelho.
    
    sample_mic();

    // for (int i = 0; i < TOTAL_SAMPLES_RECORD; i++) {
    //   printf("%u ", adc_buffer[i]);
    // }
    // printf("\n");

    gpio_put(RED_LED_PIN, 0); // Desliga o LED vermelho.

    printf("Samples recorded!\n");
    printf("Processing samples...\n");

    uint8_t filtered_buffer[TOTAL_SAMPLES_RECORD];
    apply_low_pass(adc_buffer, filtered_buffer, TOTAL_SAMPLES_RECORD);
    
    printf("Samples complete!\n");
    printf("Press again to record another!\n---------------------------------------\n");

    bool btn_a_pressed = false;
    bool btn_b_pressed = false;
    int section = 0;
    // bool test = false;
    uint64_t prev_time = to_us_since_boot(get_absolute_time());
    while (!btn_a_pressed) {
      btn_a_pressed = get_button_state(&btn_A_state);
      btn_b_pressed = get_button_state(&btn_B_state);

      if (btn_b_pressed) {
        gpio_put(GREEN_LED_PIN, 1); // Liga o LED verde.
        reproduce_samples(filtered_buffer, TOTAL_SAMPLES_RECORD);
        gpio_put(GREEN_LED_PIN, 0); // Desliga o LED verde.
        get_button_state(&btn_B_state); // Reseta o estado do botão B.
      }

      if (absolute_time_diff_us(prev_time, get_absolute_time()) > 200) {
        prev_time = to_us_since_boot(get_absolute_time());
        clear_OLed();
        // test = !test; // Alterna entre os testes.
        // draw_line_OLed(48, 48, 52, 52, test); // Desenha uma linha diagonal.
        char section_str[17];
        sprintf(section_str, "%3d %3d         ", section, TOTAL_SECTIONS);
        char* texts[] = {
          section_str,
        };
        print_lines_OLed(texts, 1, 0, 0);
        draw_wave(adc_buffer, TOTAL_SAMPLES_RECORD, section, GAP_OLED_AUDIO);
        render_OLed();
        section++;
        if (section >= TOTAL_SECTIONS) {
          section = 0; // Reseta a seção para desenhar novamente.
        }
      }
    }
  }
}

void draw_wave(uint8_t *samples, size_t len, int section, int gap) {
  int x0 = 0;
  int y0 = ssd1306_height - 1;
  int x1 = 0;

  for (int i = section * ssd1306_width / (gap + 1); i < (section + 1) * ssd1306_width / (gap + 1); i++) {
    if (i >= len) break;
    draw_line_OLed(x0, y0, x1, (((samples[i] + 1) * (ssd1306_height - 1)) >> 8u), true);
    x0 += (gap + 1);
    x1 += (gap + 1);
  }
}

/**
 * Realiza as leituras do ADC e armazena os valores no buffer.
 */
void sample_mic() {
  // uint32_t saved_interrupts = save_and_disable_interrupts();

  adc_fifo_drain(); // Limpa o FIFO do ADC.
  adc_run(false); // Desliga o ADC (se estiver ligado) para configurar o DMA.

  dma_channel_configure(dma_channel, &dma_cfg,
    adc_buffer, // Escreve no buffer.
    &(adc_hw->fifo), // Lê do ADC.
    TOTAL_SAMPLES_RECORD, // Faz SAMPLES amostras.
    true // Liga o DMA.
  );

  // Liga o ADC e espera acabar a leitura.
  adc_run(true);
  dma_channel_wait_for_finish_blocking(dma_channel);
  
  // Acabou a leitura, desliga o ADC de novo.
  adc_run(false);

  // restore_interrupts(saved_interrupts);
}

void pwm_init_buzzer(uint pin) {
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(pin);
  pwm_config config = pwm_get_default_config();
  
  float div = 1.0f;
  pwm_config_set_clkdiv(&config, div);
  pwm_top = clock_get_hz(clk_sys) / (div * SAMPLE_RATE) - 1;
  pwm_config_set_wrap(&config, pwm_top);
  
  pwm_init(slice_num, &config, true);
  pwm_set_gpio_level(pin, 0);
}

void buzzer_set(uint pin, uint8_t sample) {
  uint32_t level = (sample * pwm_top) / (1 << 8u); 
  pwm_set_gpio_level(pin, level);
}

void apply_low_pass(uint8_t *input, uint8_t *output, size_t len) {
    for (int i = 2; i < len - 2; i++) {
        float sum = 0;
        for (int j = 0; j < 5; j++) {
            sum += input[i + j - 2] * coef[j];
        }
        output[i] = (uint8_t)sum;
    }
}

void reproduce_samples(uint8_t *samples, size_t len) {
    absolute_time_t next_time = get_absolute_time();
    for (int i = 0; i < len; i++) {
        buzzer_set(BUZZER_PIN, samples[i]);
        next_time = delayed_by_us(next_time, 1000000 / SAMPLE_RATE);
        busy_wait_until(next_time);
    }
    pwm_set_gpio_level(BUZZER_PIN, 0);
}
