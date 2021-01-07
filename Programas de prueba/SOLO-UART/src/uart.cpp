/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/12/21
 *===========================================================================*/
//#include <avr/interrupt.h>
//#include <avr/io.h>
#include "Arduino.h"
#include "uart.h"

String leerBufferA(){
    String val;
    Serial1.write("RA");    //envia comando para leer el valor de buffer A
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    val = Serial1.readString();
    return(val);
}
String leerBufferB(){
    String val;
    Serial1.write("RB");    //envia comando para leer el valor de buffer A
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    val = Serial1.readString();
    return(val);
}
String leerBufferC(){
    String val;
    Serial1.write("RC");    //envia comando para leer el valor de buffer A
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    val = Serial1.readString();
    return(val);
}

void iniciarUart ()
{
    Serial1.begin(115200);
}

int grabarBufferA(char * val)
{
    Serial1.write("WA");
    Serial1.write(val);
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int grabarBufferB(char * val)
{
    Serial1.write("WB");
    Serial1.write(val);
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int grabarBufferC(char * val)
{
    Serial1.write("WC");
    Serial1.write(val);
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

String leerElectrodo()
{
    Serial1.write("RE");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String val = Serial1.readString();
    return val;
}

int calibrarBufferA()
{
    Serial1.write("CA");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int calibrarBufferB()
{
    Serial1.write("CB");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int calibrarBufferC()
{
    Serial1.write("CC");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int iniciarTitulacion()
{
    Serial1.write("TI");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int finalizarTitulacion()
{
    Serial1.write("TF");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int iniciarLimpieza()
{
    Serial1.write("LI");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int finalizarLimpieza()
{
    Serial1.write("LF");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int habilitarAgitador()
{
    Serial1.write("AI");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

int deshabilitarAgitador()
{
    Serial1.write("AF");
    while(Serial1.available() == 0);    //se bloquea hasta que haya dato
    String ack = Serial1.readString();
    if (ack == "OK")
    {
        return 1;   //se escribió correctamente
    }
    else
    {
        return 0;   //hubo error
    }    
}

/* //Esto es para usar interrupciones
void iniciarUart (){
    UBRR1 = 8; // 8 for configuring baud rate of 115200bps, 103 for 9600
    UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10); 
    // Use 8-bit character sizes
    UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);  
    // Turn on the transmission, reception, and Receive interrupt     
    sei();// enable global interrupt
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
*/