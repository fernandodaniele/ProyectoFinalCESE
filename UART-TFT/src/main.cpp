/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/12/21
 *===========================================================================*/
/***************************************************************************************************************
 * Este programa está desarrollado para un atmega328 y el framework de Arduino
 * El mismo controla un módulo de pantalla TFT 2.4" táctil (resistivo) Modelo MCUFRIEND
 * El programa debe ser capaz de medir la tensión entregada por un módulo de medición
 * de pH 4502c.
 * Se comunica por puerto serie con un módulo ESP32
 ***************************************************************************************************************/
#include "main.h"
#include "menu.h"
#include "uart.h"
/*
 * Función setup
 */
void setup(void){
  iniciarUart ();
  iniciarMEF();
  Serial.begin(9600);
}
/*
 * Función loop
 */
void loop(void){
  actualizarMEF();
}