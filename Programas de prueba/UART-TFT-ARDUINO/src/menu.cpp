/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/08/12
 *===========================================================================*/
#include "../include/menu.h"
#include "../include/pantalla.h"

// Variable para el estado actual
pantalla_t pantalla;

// Función para controlar errores de la MEF (Error handler)
void errorMEF( void )
{
   pantalla = PANTALLA_INICIAL;
   muestraPantalla(pantalla);
}

// Función para iniciar la MEF
void iniciarMEF( void )
{
   pantalla = PANTALLA_INICIAL;   // Set initial state
   muestraPantalla(pantalla);
}
int opcion =1;
// Función para actualizar la MEF
void actualizarMEF( )
{
   switch( pantalla ){
      case PANTALLA_INICIAL:
    	  switch (opcion){
             case 1:
            	 pantalla = MENU_CALIBRAR;
            	 break;
             case 2:
            	 pantalla = PANTALLA_TITULACION;
                //prender motor
                 break;
             case 3:
            	 pantalla = MENU_CONFIGURACION;
                 break;
             case 4:
            	 pantalla = PANTALLA_CONEXION;
                 break;
             default:
            	 pantalla = PANTALLA_INICIAL;
            	 break;
    	  }
        opcion = muestraPantalla(pantalla);
    	  break;

      case MENU_CALIBRAR:
         switch (opcion){
            case 1:
               pantalla = MENU_CALIBRAR1;
               break;
            case 2:
               pantalla = MENU_CALIBRAR2;
               break;
            case 3:
               pantalla = MENU_CALIBRAR3;
               break;
            case 4:
               pantalla = PANTALLA_INICIAL;
               break;
            default:
               pantalla = MENU_CALIBRAR;
               break;
    	   }
         muestraPantalla(pantalla);
    	   break;

      case MENU_CALIBRAR1:
         if(opcion==4){
            pantalla = MENU_CALIBRAR;
    	   }
         muestraPantalla(pantalla); 
    	   break;

      case MENU_CALIBRAR2:
    	   if(opcion==4){
            pantalla = MENU_CALIBRAR;
    	   }
         muestraPantalla(pantalla); 
    	   break;

      case MENU_CALIBRAR3:
    	   if(opcion==4){
            pantalla = MENU_CALIBRAR;
    	   }
         muestraPantalla(pantalla);
    	   break;

      case PANTALLA_TITULACION:
         
    	   if(opcion==4){
    	      pantalla = PANTALLA_INICIAL;
    		   //apaga motor
    	   }
         muestraPantalla(pantalla);
    	   break;

      case MENU_CONFIGURACION:
         switch (opcion){
            case 1:
               pantalla = MENU_CONFIGURACION1;
               break;
            case 2:
               pantalla = MENU_CONFIGURACION2;
               break;
            case 3:
               pantalla = MENU_CONFIGURACION3;
               //prende motor
               break;
            case 4:
               pantalla = PANTALLA_INICIAL;
               break;
            default:
               pantalla = MENU_CONFIGURACION;
               break;
    	   }
    	   muestraPantalla(pantalla);
         break;

      case MENU_CONFIGURACION1:
         if(opcion==4){
            pantalla = MENU_CONFIGURACION;
    	   }
         muestraPantalla(pantalla); 
    	   break;

      case MENU_CONFIGURACION2:
    	   if(opcion==4){
            pantalla = MENU_CONFIGURACION;
    	   }
         muestraPantalla(pantalla);
    	   break;

      case MENU_CONFIGURACION3:
    	   if(opcion==4){
            pantalla = MENU_CONFIGURACION;
            //apaga motor
    	   }
         muestraPantalla(pantalla); 
    	   break;

      case PANTALLA_CONEXION:
    	   if(opcion==4){
            pantalla = PANTALLA_INICIAL;
    	   }
    	   muestraPantalla(pantalla);
         break;

      default:
    	 errorMEF();
    	 break;
   }
}