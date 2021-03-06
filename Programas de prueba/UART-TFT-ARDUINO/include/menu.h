/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/08/12
 *===========================================================================*/

#ifndef MISPROGRAMAS_PDM_TP_FINAL_INC_MENU_H_
#define MISPROGRAMAS_PDM_TP_FINAL_INC_MENU_H_

#include <Arduino.h>
// Funciones de la MEF
void errorMEF( void );
void iniciarMEF( void );
void actualizarMEF();

// Nombres de los estados de la MEF
typedef enum{
   PANTALLA_INICIAL,
   MENU_CALIBRAR,
   MENU_CALIBRAR1,
   MENU_CALIBRAR2,
   MENU_CALIBRAR3,
   PANTALLA_TITULACION,
   MENU_CONFIGURACION,
   MENU_CONFIGURACION1,
   MENU_CONFIGURACION2,
   MENU_CONFIGURACION3,
   PANTALLA_CONEXION,
} pantalla_t;

#endif /* MISPROGRAMAS_PDM_TP_FINAL_INC_MENU_H_ */