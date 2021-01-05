/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

int sendData(const char* data);

char bufferA[5];
int bufferAInt = 400;
char bufferB[5];
int bufferBInt = 700;
char bufferC[5];
int bufferCInt = 1000;
char electrodoA[5];
int electrodoAInt = 500;
char electrodoB[5];
int electrodoBInt = 750;
char electrodoC[5];
int electrodoCInt = 1100;
char electrodoStr[5];
int electrodoVal = 800;
char volumenStr[5];
int volumenInt = 60;

void init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    return txBytes;
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    
    while (1) {
        int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 500 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
			switch (data[0])
			{
            case 'E':
                sprintf(electrodoStr,"%d",electrodoVal);
                sendData (electrodoStr); //acá seria el valor del electrodo
                break;
			case 'W':
                {
                    switch (data[1])
                    {
                    case 'A':
                        for(int i = 2; i<rxBytes; i++)
                        {
                            bufferA [i-2] = data [i];
                        }
                        bufferAInt = atoi (bufferA);
                        sendData("OK");
                        break;
                    case 'B':
                        for(int i = 2; i<rxBytes; i++)
                        {
                            bufferB [i-2] = data [i];
                        }
                        bufferBInt = atoi (bufferB);
                        sendData("OK");
                        break;
                    case 'C':
                        for(int i = 2; i<rxBytes; i++)
                        {
                            bufferC [i-2] = data [i];
                        }
                        bufferCInt = atoi (bufferC);
                        sendData("OK");
                        break;
                    case 'V':
                        for(int i = 2; i<rxBytes; i++)
                        {
                            volumenStr [i-2] = data [i];
                        }
                        volumenInt = atoi (volumenStr);
                        sendData("OK");
                        break;
                    default:
                        break;
                    }
                }
				break;
			case 'R':
                {
                    switch (data[1])
                    {
                    case 'A':
                        sprintf(bufferA,"%d",bufferAInt);
                        sendData (bufferA);
                        ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", bufferA);
                        break;
                    case 'B':
                        sprintf(bufferB,"%d",bufferBInt);
                        sendData (bufferB);
                        ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", bufferB);
                        break;
                    case 'C':
                        sprintf(bufferC,"%d",bufferCInt);
                        sendData (bufferC);
                        ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", bufferC);
                        break;
                    case 'V':
                        sprintf(volumenStr,"%d",volumenInt);
                        sendData (volumenStr);
                        break;
                    default:
                        break;
                    }
                }
				break;
            case 'V':
                {
                    switch (data[1])
                    {
                    case 'A':
                        sprintf(electrodoA,"%d",electrodoAInt);
                        sendData (electrodoA);
                        //ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", electrodoA);
                        break;
                    case 'B':
                        sprintf(electrodoB,"%d",electrodoBInt);
                        sendData (electrodoB);
                        //ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", electrodoB);
                        break;
                    case 'C':
                        sprintf(electrodoC,"%d",electrodoCInt);
                        sendData (electrodoC);
                        //ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", electrodoC);
                        break;
                    default:
                        break;
                    }
                }
				break;
            case 'C':
                {
                    switch (data[1])
                    {
                    case 'A':
                        //grabar valor de bufferA en memoria
                        electrodoAInt++;
                        sendData("OK");
                        break;
                    case 'B':
                        //grabar valor de bufferB en memoria
                        electrodoBInt++;
                        sendData("OK");
                        break;
                    case 'C':
                        //grabar valor de bufferC en memoria
                        electrodoCInt++;
                        sendData("OK");
                        break;
                    default:
                        break;
                    }
                }
                break;
			
            case 'T':
                {
                    switch (data[1])
                    {
                    case 'I':
                        //iniciar titualacion
                        sendData("OK");
                        break;
                    case 'F':
                        //finalizar titulacion
                        sendData("OK");
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 'L':
                {
                    switch (data[1])
                    {
                    case 'I':
                        //iniciar limpieza
                        sendData("OK");
                        break;
                    case 'F':
                        //finalizar limpieza
                        sendData("OK");
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 'A':
                {
                    switch (data[1])
                    {
                    case 'I':
                        //habilitar agitador
                        sendData("OK");
                        break;
                    case 'F':
                        //deshabilitar agitador
                        sendData("OK");
                        break;
                    default:
                        break;
                    }
                }
                break;
			default:
				break;
			}
        }
    
    }
    free(data);
}

void app_main(void)
{
    init();
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    //xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
}


