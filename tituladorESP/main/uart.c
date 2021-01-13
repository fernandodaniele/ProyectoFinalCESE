#include "uart.h"
#include "flash.h"

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

extern int valorAdc;
extern portMUX_TYPE myMutex;
extern int motorBomba;

int sendData(const char* data);

char bufferA[7];
int16_t bufferAInt = 400;
char bufferB[7];
int16_t bufferBInt = 700;
char bufferC[7];
int16_t bufferCInt = 1000;
char electrodoA[7];
int16_t electrodoAInt = 500;
char electrodoB[7];
int16_t electrodoBInt = 750;
char electrodoC[7];
int16_t electrodoCInt = 1100;
char electrodoStr[7];
int16_t electrodoVal = 800;
char volumenStr[7];
int16_t volumenCorte = 60;

void iniciarUart(void) {
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

void rx_task(void *arg)
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
                sprintf(electrodoStr,"%d",valorAdc);
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
                        if(guardarFlash("BUFFERA", bufferAInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", bufferAInt);
                        }
                        break;
                    case 'B':
                        for(int i = 2; i<rxBytes; i++)
                        {
                            bufferB [i-2] = data [i];
                        }
                        bufferBInt = atoi (bufferB);
                        sendData("OK");
                        if(guardarFlash("BUFFERB", bufferBInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", bufferBInt);
                        }
                        break;
                    case 'C':
                        for(int i = 2; i<rxBytes; i++)
                        {
                            bufferC [i-2] = data [i];
                        }
                        bufferCInt = atoi (bufferC);
                        sendData("OK");
                        if(guardarFlash("BUFFERC", bufferCInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", bufferCInt);
                        }
                        break;
                    case 'V':
                        for(int i = 2; i<rxBytes; i++)
                        {
                            volumenStr [i-2] = data [i];
                        }
                        volumenCorte = atoi (volumenStr);
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
                        if(leerFlash("BUFFERA", &bufferAInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", bufferAInt);
                        }
                        sprintf(bufferA,"%i",bufferAInt);
                        sendData (bufferA);
                        ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", bufferA);
                        break;
                    case 'B':
                        if(leerFlash("BUFFERB", &bufferBInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", bufferBInt);
                        }
                        sprintf(bufferB,"%d",bufferBInt);
                        sendData (bufferB);
                        ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", bufferB);
                        break;
                    case 'C':
                        if(leerFlash("BUFFERC", &bufferCInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", bufferCInt);
                        }
                        sprintf(bufferC,"%d",bufferCInt);
                        sendData (bufferC);
                        ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", bufferC);
                        break;
                    case 'V':
                        sprintf(volumenStr,"%d",volumenCorte);
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
                        if(leerFlash("CALIBREA", &electrodoAInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", electrodoAInt);
                        }
                        sprintf(electrodoA,"%d",electrodoAInt);
                        sendData (electrodoA);
                        //ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", electrodoA);
                        break;
                    case 'B':
                        if(leerFlash("CALIBREB", &electrodoBInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", electrodoBInt);
                        }
                        sprintf(electrodoB,"%d",electrodoBInt);
                        sendData (electrodoB);
                        //ESP_LOGI(RX_TASK_TAG, "Send data: '%s'", electrodoB);
                        break;
                    case 'C':
                        if(leerFlash("CALIBREC", &electrodoCInt))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", electrodoCInt);
                        }
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
                        if(guardarFlash("CALIBREA", valorAdc))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", valorAdc);
                        }
                        sendData("OK");
                        break;
                    case 'B':
                        if(guardarFlash("CALIBREB", valorAdc))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", valorAdc);
                        }
                        sendData("OK");
                        break;
                    case 'C':
                        if(guardarFlash("CALIBREC", valorAdc))
                        {
                            ESP_LOGI(RX_TASK_TAG, "%d Guardado en Flash", valorAdc);
                        }
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
                        motorBomba = 1;
                        break;
                    case 'F':
                        //finalizar titulacion
                        sendData("OK");
                        motorBomba = 0;
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