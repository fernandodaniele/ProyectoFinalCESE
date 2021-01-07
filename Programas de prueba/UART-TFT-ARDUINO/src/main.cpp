/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/08/12
 *===========================================================================*/

#include "Arduino.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include "../include/menu.h"
#include "../include/pantalla.h"

/*=====[Definition macros of private constants]==============================*/

#define TIEMPO_MEF_MENU  100
#define TIEMPO_MEF_PULSADOR  100


/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

int pulsador=0, ultimoBoton = 0; //variable para guardar el ultimo pulsador que permite el cambio de estado


volatile char datos [100];
volatile char datosRecibidos [100];
volatile byte indice=0;
volatile byte cantidad=0;
volatile bool recepcionTerminada=false;

unsigned long mefMenu;


void setup()
{
   UBRR1 = 8; // 8 for configuring baud rate of 115200bps, 103 for 9600
   UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10); 
// Use 8-bit character sizes
   UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);  
// Turn on the transmission, reception, and Receive interrupt     
  Serial.begin(115200); 
     indice = 0;
  recepcionTerminada = false;
  sei();// enable global interrupt

  // ----- Configuración -----------------------------------
   mefMenu = millis ();
   iniciarMEF();
   iniciarPantalla();

}

void loop()
{ 
  
  if (recepcionTerminada)
  {
    
    recepcionTerminada = false;
    cantidad = 0;
  }

  //---Se actualiza MEF del menu cada 100 mS
  if( millis () > mefMenu + TIEMPO_MEF_MENU)
  {
    actualizarMEF();
    ultimoBoton = 0;
  }
  //---Se actualiza la MEF de los botones cada 50 mS
  /*if( delayRead(&mefPulsadores) ) {
  actualizarTeclas();
  pulsador = consultarTeclas();
  }
  else{
  pulsador = 0;
  }
  if (pulsador != 0){
  ultimoBoton = pulsador;
  }*/
}

ISR(USART1_RX_vect)
{ 
  char datoActual = UDR1; // Lee el rado recibido desde el puerto serie
  
  if(datoActual == '\n'){
    recepcionTerminada = true;
    for(int i =0; i<indice; i++){
          datosRecibidos[i] = datos [i];
    }
    cantidad = indice;
    indice=0;
  }
  else
  {
    datos[indice] = datoActual;
    indice++;
  }
}