#include "electrodo.h"
#include <driver/adc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include <stdio.h>
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#define DUTY_US     500
#define PWM_FREQ    1000

int valorAdc;
int motorBomba = 0;
int volumenActual = 0;
extern int volumenCorte;
portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

void electrodo_task (void *arg)
{
    //Configuración
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_MAX);
    
    //Bucle infinito
    while(1)
    {
        taskENTER_CRITICAL(&myMutex);
        valorAdc = adc1_get_raw(ADC1_CHANNEL_0);
        taskEXIT_CRITICAL(&myMutex);

        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void tareaEjemploPWM(void *arg)
{
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 21);    //Set GPIO 18 as PWM0A, to which servo is connected

    mcpwm_config_t pwm_config;
    pwm_config.frequency = PWM_FREQ;
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings
    while (1) 
    {
        if(motorBomba == 1)
        {
            mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, DUTY_US);
            motorBomba =3;
        }
        else if ((motorBomba == 0)||(volumenActual == volumenCorte))
        {
            mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0);
            volumenActual = 0;
            motorBomba = 0;
        }
        volumenActual++; //acá habria que leer el volumen desde el encoder
        vTaskDelay(10); 
    }
}