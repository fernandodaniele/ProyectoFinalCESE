/*
 * pantalla.h
 *
 *  Created on: 18 ago. 2020
 *      Author: fernando
 */

#ifndef PANTALLA_H_
#define PANTALLA_H_

#include <Arduino.h>
#include "menu.h"

void iniciarPantalla();

int muestraPantalla(pantalla_t);

bool Touch_getXY(int *pixel_X,int *pixel_Y);

#endif /*PANTALLA_H_ */
