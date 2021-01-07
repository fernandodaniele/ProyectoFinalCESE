/* 
    Prueba de SPI con Arduino Mega como esclavo
    Para más informacion visitar https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi_master.html
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22
#define NOT_USED    -1
#define MAX_TRANSF_SZ 10
#define DMA_CHAN    2
#define ARDUINO_HOST    HSPI_HOST


//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
void spiPreTransferCallback(spi_transaction_t *t)
{
    int dc=(int)t->user;
    //gpio_set_level(PIN_NUM_DC, dc);
}

/* Send data. Uses spi_device_polling_transmit, which waits until the
 * transfer is complete.
 *
 * Since data transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void sendData(spi_device_handle_t spi, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len==0) return;             //no need to send anything
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=len*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=data;               //Data
    ret=spi_device_polling_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

void app_main(void)
{
    esp_err_t ret; // Variable para detectar errores
    spi_device_handle_t spiHandle; // Handle for a device on a SPI bus.

    const spi_bus_config_t busConfig = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = NOT_USED,
        .quadhd_io_num = NOT_USED,
        .max_transfer_sz= MAX_TRANSF_SZ //Maximum transfer size, in bytes. Defaults to 4094 if 0.
    };

    spi_device_interface_config_t devConfig ={
        .clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
        .mode=0,                                //SPI mode 0 -- ver que es esto
        .spics_io_num=PIN_NUM_CS,               //CS pin
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        .pre_cb=spiPreTransferCallback,  //Callback to be called before a transmission is started.
    };

    ret = spi_bus_initialize(ARDUINO_HOST, &busConfig, DMA_CHAN); // Inicializa el bus spi
    ESP_ERROR_CHECK(ret); // Verifica que no haya errores

    ret = spi_bus_add_device(ARDUINO_HOST, &devConfig, &spiHandle); // Aloja un dispositivo en el bus SPI
    ESP_ERROR_CHECK(ret); // Verifica que no haya errores

    while (true)
    {
        printf("Hello world!\n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }    
}
