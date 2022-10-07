#include "driver/uart.h"
#include "esp_ao.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define UART_NUM (UART_NUM_2)
#define TXD_PIN 17
#define RXD_PIN 16

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)

static const char *TAG = "UART_TASK";

static QueueHandle_t uart_queue;

static void process_command(uint8_t *data, int start, int end) {
    size_t length = end - start + 1;

    /*
        Faz um switch aqui e,
        Active_post(AO_ihmManager) com o evento correto
    */
}

static void uart_task(void *pvParameters) {
    Active *AO_ihmManager = (Active *)pvParameters;

    uart_event_t uart_event;

    for (;;) {
        if (xQueueReceive(uart_queue, (void *)&uart_event, portMAX_DELAY)) {
            size_t buffer_size = uart_event.size;
            uint8_t data[128];

            /* Checa se o tipo de evento é do tipo 0 (DATA), se não for, analisar pq pode ser fonte de bugs */
            if (uart_event.type == 0) {
                uart_read_bytes(UART_NUM, data, buffer_size, portMAX_DELAY);

                int command_start = 2;
                int command_end = 2;
                while (command_start < buffer_size) {
                    for (int i = command_start; i < buffer_size; i++) {
                        if (data[i] == 0x54) {
                            if (i < buffer_size && data[i + 1] == 0xA5) {
                                command_end = data[i - 1];
                                break;
                            }
                        }
                    }

                    if (command_end == 2) {
                        command_end = buffer_size - 1;
                    }

                    process_command(data, command_start, command_end);
                    command_start = command_end + 1;
                }
            } else {
                ESP_LOGE(TAG, "Uart Event Type returned: %d", uart_event.type);
            }
        }
    }
}

void uart_task_init(Active *AO_ihmManager) {
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart_queue, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    xTaskCreate(uart_task, "UART_TASK", 2400, (void *)AO_ihmManager, 4, NULL);
}