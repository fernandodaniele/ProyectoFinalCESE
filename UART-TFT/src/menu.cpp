/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/12/21
 *===========================================================================*/
#include "uart.h"
#include "menu.h"
#include "main.h"
#include "pantallaTFT.h"


// Variable para el estado actual
pantalla_t pantalla;

// Función para controlar errores de la MEF (Error handler)
void errorMEF( void )
{
   pantalla = PANTALLA_INICIAL;
   pantallaInicial();
}

// Función para iniciar la MEF
void iniciarMEF( void )
{
   pantalla = PANTALLA_INICIAL;   // Set initial state
   inicializarTFT();
}
int opcion =0;
// Función para actualizar la MEF
void actualizarMEF( )
{
   switch( pantalla ){
      case PANTALLA_INICIAL:
         opcion = tactilInicial();
    	   switch (opcion){
            case 1:
               pantalla = MENU_CALIBRAR;
               pantallaElegirBuffer();
               break;
            case 2:
               pantalla = PANTALLA_TITULACION;
               pantallaMedir();
               if(iniciarTitulacion()){   //envia señal para iniciar la titulacion
                  Serial.println("Se inicio titulacion");
               }
               else
               {
                  //acá ver que hacer en caso de error
                  Serial.println("Error en el inicio de titulacion");
               }
               break;
            case 3:
               pantalla = MENU_CONFIGURACION;
               pantallaConfigurar();
               break;
            case 4:
               pantalla = PANTALLA_CONEXION;
               pantallaWIFI ();
               break;
            default:
               pantalla = PANTALLA_INICIAL;
               break;
    	   }
    	   break;

      case MENU_CALIBRAR:
         opcion = tactilElegirBuffer();
         switch (opcion){
            case 1:
               pantalla = MENU_CALIBRAR_A;
               pantallaCalibrar ();
               break;
            case 2:
               pantalla = MENU_CALIBRAR_B;
               pantallaCalibrar ();
               break;
            case 3:
               pantalla = MENU_CALIBRAR_C;
               pantallaCalibrar ();
               break;
            case 4:
               pantalla = PANTALLA_INICIAL;
               pantallaInicial();
               break;
            default:
               pantalla = MENU_CALIBRAR;
               break;
    	   }
    	   break;

      case MENU_CALIBRAR_A:
         opcion = tactilCalibrar();
    	   if(opcion==3){
            pantallaCalibrarA();
            pantalla = MENU_CALIBRAR;
            pantallaElegirBuffer();
    	   }
         else if(opcion==4){
            pantalla = MENU_CALIBRAR;
            pantallaElegirBuffer();
    	   }
    	   break;

      case MENU_CALIBRAR_B:
         opcion =  tactilCalibrar();
    	   if(opcion==3){
            pantallaCalibrarB();
            pantalla = MENU_CALIBRAR;
            pantallaElegirBuffer();
    	   }
         else if(opcion==4){
            pantalla = MENU_CALIBRAR;
            pantallaElegirBuffer();
    	   }
    	   break;

      case MENU_CALIBRAR_C:
         opcion =  tactilCalibrar();
    	   if(opcion==3){
            pantallaCalibrarC();
            pantalla = MENU_CALIBRAR;
            pantallaElegirBuffer();
    	   }
         else if(opcion==4){
            pantalla = MENU_CALIBRAR;
            pantallaElegirBuffer();
    	   }
    	   break;

      case PANTALLA_TITULACION:
         opcion = tactilMedir();
    	   if(opcion==4){
            if(finalizarTitulacion()){
               Serial.println("Se finalizo titulacion");
            }
            else
            {
               Serial.println("Error en finalizar titulacion");
            }
            pantalla = PANTALLA_INICIAL;
            pantallaInicial();
    	   }
    	   break;

      case MENU_CONFIGURACION:
         opcion = tactilConfigurar();
         switch (opcion){   
            case 1:
               pantalla = MENU_CONFIGURACION_BUFFER;
               pantallaConfigurarBuffer();
               break;
            case 2:
               pantalla = MENU_VOLUMEN;
               pantallaVolumenCorte();
               break;
            case 3:
               pantalla = MENU_LIMPIEZA;
               pantallaLimpieza();
               if(iniciarLimpieza()){
                  Serial.println("Se inicio limpieza");
               }
               else
               {
                  Serial.println("Error en inicio limpieza");
               }
               break;
            case 4:
               pantalla = MENU_AGITADOR;
               pantallaAgitador();
               break;
            case 5:
               pantalla = PANTALLA_INICIAL;
               pantallaInicial();
               break;
            default:
               pantalla = MENU_CONFIGURACION;
               break;
    	   }
         break;

      case MENU_CONFIGURACION_BUFFER:
         opcion = tactilConfigurarBuffer();
         if(opcion==4){
            //Enviar al ESP los valores de los buffers configurados
            pantalla = MENU_CONFIGURACION;
            pantallaConfigurar();
         }
    	   break;

      case MENU_VOLUMEN:
         opcion = tactilVolumenCorte();
    	   if(opcion==4){
            //Acá enviar al ESP el valor del volumen de corte
            pantalla = MENU_CONFIGURACION;
            pantallaConfigurar();
    	   }
    	   break;

      case MENU_LIMPIEZA:
         opcion = tactilLimpieza();
    	   if(opcion==4){
            pantalla = MENU_CONFIGURACION;
            if(finalizarLimpieza()){
               Serial.println("Se finalizo limpieza");
            }
            else
            {
               Serial.println("Error en finalizar limpieza");
            }
            pantallaConfigurar();
    	   }
    	   break;

      case MENU_AGITADOR:
         opcion = tactilAgitador();
         switch (opcion)
         {
         case 1:
               if(habilitarAgitador()){
                  habilitoAgitador();
               }
               else
               {
                  errorAgitador();
               }
            pantalla = MENU_CONFIGURACION;
            pantallaConfigurar();
            break;
         case 2:
               if(deshabilitarAgitador()){
                 deshabilitoAgitador();
               }
               else
               {
                  errorAgitador();
               }
            pantalla = MENU_CONFIGURACION;
            pantallaConfigurar();
            break;
         case 4:
            pantalla = MENU_CONFIGURACION;
            pantallaConfigurar();
            break;
         default:
            break;
         }
    	   break;

      case PANTALLA_CONEXION:
         opcion = tactilWIFI();
    	   if(opcion==4){
            pantalla = PANTALLA_INICIAL;
            pantallaInicial();
    	   }
         break;

      default:
    	 errorMEF();
    	 break;
   }
}