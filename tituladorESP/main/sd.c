#include "sd.h"

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

static const char *TAG = "example";

#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 15
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   13

sdmmc_card_t* card;
esp_err_t ret;
sdmmc_host_t host = SDSPI_HOST_DEFAULT();
sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
//Opciones para montar el sistema de archivos
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,                //Si es true, la SD se formatea en caso de que falle el montaje
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };



void inicializarSD()
{   
    slot_config.gpio_miso = PIN_NUM_MISO;
    slot_config.gpio_mosi = PIN_NUM_MOSI;
    slot_config.gpio_sck  = PIN_NUM_CLK;
    slot_config.gpio_cs   = PIN_NUM_CS;

        
    ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }

    if(escribeSD("Que tal")==0)
    {
        ESP_LOGE(TAG, "Error al abrir el archivo para escribir");
    }
    char dato [100];
    if(leeSD(dato)==0)
    {
        ESP_LOGE(TAG, "Error al abrir el archivo para leer");
    }
    ESP_LOGI(TAG, "Se leyó del archivo: '%s'", dato);
}
    
int escribeSD(char* dato)
{
    FILE* f = fopen("/sdcard/titular.txt", "w");
    if (f == NULL) {
        return 0;
    }
    fprintf(f, "%s\n",dato);
    fclose(f);
    ESP_LOGI(TAG, "File written");
    return 1;
}

int leeSD(char* dato)
{
    // Abre el archivo para lectura
    FILE* f = fopen("/sdcard/titular.txt", "r");
    if (f == NULL) {
        return 0;
    }
    fgets(dato, 10, f);
    fclose(f);
    return 1;
}

void desmontarSD()
{
    // Desmonta particion y deshabilita SPI
    esp_vfs_fat_sdmmc_unmount();
    ESP_LOGI(TAG, "Card unmounted");
}