/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/12/21
 *===========================================================================*/
/***************************************************************************************************************
 
 ***************************************************************************************************************/
#include "main.h"
#include "uart.h"
#include "Arduino.h"
/*
 * Función setup
 */
void setup(void){
  iniciarUart ();
  Serial.begin(9600);

}
/*
 * Función loop
 */
String lectura;
void loop(void){
  grabarBufferA("04");
  grabarBufferB("07");
  grabarBufferC("10");
  delay(1000);
  lectura = leerBufferA();
  Serial.print ("El valor del buffer A es ");
  Serial.println(lectura);
  lectura = leerBufferB();
  Serial.print ("El valor del buffer B es ");
  Serial.println(lectura);
  lectura = leerBufferC();
  Serial.print ("El valor del buffer C es ");
  Serial.println(lectura);
  delay(1000);

  lectura = leerElectrodo ();
  Serial.print ("El valor del electrodo es ");
  Serial.println(lectura);

  if(calibrarBufferA()){
    Serial.println("Se calibró bufferA");
  }
  if(calibrarBufferB()){
    Serial.println("Se calibró bufferA");
  }
  if(calibrarBufferC()){
    Serial.println("Se calibró bufferA");
  }

  if(iniciarTitulacion()){
    Serial.println("Se inicio titulacion");
  }
  if(finalizarTitulacion()){
    Serial.println("Se finalizo titulacion");
  }

  if(iniciarLimpieza()){
    Serial.println("Se inicio limpieza");
  }
  if(finalizarLimpieza()){
    Serial.println("Se finalizo limpieza");
  }

  if(habilitarAgitador()){
    Serial.println("Se inicio agitador");
  }
  if(deshabilitarAgitador()){
    Serial.println("Se finalizo agitador");
  }


}