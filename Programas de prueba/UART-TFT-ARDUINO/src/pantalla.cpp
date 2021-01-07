/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/08/12
 *===========================================================================*/
#include "../include/pantalla.h"

#include <stdlib.h>
#include <Adafruit_GFX.h>  //librería gráfica para TFT
#include <MCUFRIEND_kbv.h> //librería especifica del módulo
#include <TouchScreen.h>   //librería para la parte táctil del módulo
#define MINPRESSURE 150   //Valor previo: 200- Rango de detección eje z táctil
#define MAXPRESSURE 1000

#define BLACK   0x0000    //Colores predefinidos. Se pueden agregar o modificar
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void pantallaInicial();
int tactilInicial();
void menuCalibrar();

MCUFRIEND_kbv tft;        //Objeto pantalla gráfica

int pantallaPrevia=100; //para imprimir una sola vez la pantalla

uint16_t color;

//const int XP=12,XM=15,YP=33,YM=13; 
//const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341 //Para Arduino UNO
const int XP=8,XM=A2,YP=A3,YM=9; //arduino nano     
//const int TS_LEFT=130,TS_RT=906,TS_TOP=905,TS_BOT=126;  //Valores obtenidos del ejemplo de calibración de la librería. Para arduino UNO usar const int TS_LEFT=66,TS_RT=883,TS_TOP=924,TS_BOT=108;                                                      
const int TS_LEFT=105,TS_RT=899,TS_TOP=889,TS_BOT=115;  //Valores obtenidos del ejemplo de calibración de la librería. Para arduino UNO usar const int TS_LEFT=66,TS_RT=883,TS_TOP=924,TS_BOT=108;  
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);      //Objeto para pantalla táctil ¿porqué 300?

Adafruit_GFX_Button calibrarBtn, medirBtn, configurarBtn, acercaBtn;    //botones pantalla principal
Adafruit_GFX_Button  finalizarBtn, bufferABtn, bufferBBtn, bufferCBtn;  //botones pantalla elegir buffer
Adafruit_GFX_Button guardarBtn, cancelar2Btn;                           //botones pantalla calibrar
Adafruit_GFX_Button  finalizarGBtn;                                     //botones pantalla medir
Adafruit_GFX_Button  incBufferABtn, decBufferABtn, incBufferBBtn, decBufferBBtn,
                     incBufferCBtn, decBufferCBtn, finalizarCBtn;  //botones pantalla configurar

int bufferA, bufferB, bufferC; //variables de calibración en bits formato decimal (0-0V 2047-3.3V)
float bufferAPH=4.00, bufferBPH=7.00, bufferCPH=10.00; //variables de calibración en ph

int botonPresionado;

int pixel_x, pixel_y;  
/*
 * Función para leer la pantalla táctil
 */

bool Touch_getXY(int *pixel_X,int *pixel_Y){           //Touch_getXY() updates global vars
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);          //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);       //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
      *pixel_X = map(p.y, TS_LEFT, TS_RT, 0, 320);
      *pixel_Y = map(p.x, TS_TOP, TS_BOT, 0, 240);
    }
    return pressed;
}

void iniciarPantalla()
{
   	//configurar pantalla
	uint16_t ID = tft.readID();
	if (ID == 0xD3D3)
		ID = 0x9486; // write-only shield
	tft.begin(ID);
	tft.setRotation(1);            //horizontal
	tft.setTextSize(3);
}

int muestraPantalla(int pantalla)
{
   switch( pantalla ){
      case PANTALLA_INICIAL:
	     if(pantalla != pantallaPrevia){
            pantallaInicial();
			pantallaPrevia = pantalla;
	     }
		 else
		 {
			 botonPresionado = tactilInicial();
		 }
		 
	     break;

      case MENU_CALIBRAR:
         if(pantalla != pantallaPrevia){
		    menuCalibrar();
			pantallaPrevia = pantalla;
	     }
	     break;

      case MENU_CALIBRAR1:
	     if(pantalla != pantallaPrevia){
		    
			pantallaPrevia = pantalla;
	     }
	     break;

      case MENU_CALIBRAR2:
	     if(pantalla != pantallaPrevia){
		 
		 	pantallaPrevia = pantalla;
	     }
	     break;

      case MENU_CALIBRAR3:
	     if(pantalla != pantallaPrevia){
			
			pantallaPrevia = pantalla;
	     }
	     break;

      case PANTALLA_TITULACION:
	     if(pantalla != pantallaPrevia){
		    
			pantallaPrevia = pantalla;
	     }
	     break;

      case MENU_CONFIGURACION:
	     if(pantalla != pantallaPrevia){
		    
			pantallaPrevia = pantalla;
	     }
	     break;

      case MENU_CONFIGURACION1:
	     if(pantalla != pantallaPrevia){
		 
		 	pantallaPrevia = pantalla;
	     }
	     break;

      case MENU_CONFIGURACION2:
	     if(pantalla != pantallaPrevia){
		   
			pantallaPrevia = pantalla;
	     }
	     break;

      case MENU_CONFIGURACION3:
	     if(pantalla != pantallaPrevia){
			
			pantallaPrevia = pantalla;
	     }
	     break;

      case PANTALLA_CONEXION:
	     if(pantalla != pantallaPrevia){
		    
			pantallaPrevia = pantalla;
	     }
	     break;
      default:
	     break;
	  }
	  return botonPresionado;
}
 
/*************************************************************************************************
 * Pantalla princial - Frontend
 *************************************************************************************************/
void pantallaInicial(){
  delay(50);
  tft.fillScreen(BLACK);
  calibrarBtn.initButton(&tft,  80, 150, 140, 40, WHITE, CYAN, BLACK, "CALIBRAR", 2); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
  medirBtn.initButton(&tft, 240, 150, 140, 40, WHITE, CYAN, BLACK, "MEDIR PH", 2);
  configurarBtn.initButton(&tft,  80, 210, 140, 40, WHITE, CYAN, BLACK, "BUFFERS", 2); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
  acercaBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "HORA", 2);
  calibrarBtn.drawButton(false);//dibuja boton. false es para intercambiar colores
  medirBtn.drawButton(false);
  configurarBtn.drawButton(false);
  acercaBtn.drawButton(false);
  tft.drawRect(10, 10, 300, 110, WHITE); //(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  tft.setCursor(15,13);
  tft.setTextColor(WHITE);
  tft.print("Calibracion y medicion");
  tft.setCursor(15,38);
  tft.print("de pH para titulador.");
  tft.setCursor(15,61);
  tft.print("Desarrollado por");
  tft.setCursor(15,86);
  tft.print("U.T.N. San Francisco.");
  return;
}

void menuCalibrar(){      //interfaz gráfica
    delay(50);
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,20);
    tft.print("bufferA = ");
    //bufferA = leerBufferA();
    //bufferB = leerBufferB();
    //bufferC = leerBufferC();
    tft.print(bufferA);
    tft.setCursor(10,40);
    tft.print("bufferB = ");
    tft.print(bufferB);
    tft.setCursor(10,60);
    tft.print("bufferC = ");
    tft.print(bufferC);
    
    bufferABtn.initButton(&tft,  80, 150, 140, 40, WHITE, CYAN, BLACK, "BUFFER 4", 2); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
    bufferBBtn.initButton(&tft, 240, 150, 140, 40, WHITE, CYAN, BLACK, "BUFFER 7", 2);
    bufferCBtn.initButton(&tft,  80, 210, 140, 40, WHITE, CYAN, BLACK, "BUFFER 10", 2); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
    finalizarBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "FINALIZAR", 2);
    bufferABtn.drawButton(false);//dibuja boton. false es para intercambiar colores
    bufferBBtn.drawButton(false);
    bufferCBtn.drawButton(false);//dibuja boton. false es para intercambiar colores
    finalizarBtn.drawButton(false);
    return;
}



int tactilInicial(){
  bool down = Touch_getXY(&pixel_x, &pixel_y);
  medirBtn.press(down && medirBtn.contains(pixel_x, pixel_y));
  calibrarBtn.press(down && calibrarBtn.contains(pixel_x, pixel_y));
  configurarBtn.press(down && configurarBtn.contains(pixel_x, pixel_y));
  acercaBtn.press(down && acercaBtn.contains(pixel_x, pixel_y));
  if (calibrarBtn.justReleased()) {
    delay(50); 
    return 1;
  }
  else if (medirBtn.justReleased()) {
    delay(50); 
    return 2; 
  }
  else if (configurarBtn.justReleased()) {
    delay(50);
    return 3;
  }
  else if (acercaBtn.justReleased()) {
    return 4;
  }
  else
  {
	  return 0;
  }
  
}
/*************************************************************************************************
 * Pantalla elegir buffer
 *************************************************************************************************
void elegirBuffer(){
  pantallaElegirBuffer();
  int bandera =0;
  while(bandera == 0){
    bool  ec = Touch_getXY(&pixel_x,&pixel_y);
    bufferABtn.press(ec && bufferABtn.contains(pixel_x, pixel_y));
    bufferBBtn.press(ec && bufferBBtn.contains(pixel_x, pixel_y));
    bufferCBtn.press(ec && bufferCBtn.contains(pixel_x, pixel_y));
    finalizarBtn.press(ec && finalizarBtn.contains(pixel_x, pixel_y));
    if (bufferABtn.justReleased()){
      delay(50); 
      bufferA = calibrar();
      grabarBufferA(bufferA);
      pantallaElegirBuffer();
    }
    if (bufferBBtn.justReleased()){
      delay(50); 
     bufferB= calibrar();
     grabarBufferB(bufferB);
     pantallaElegirBuffer();
    }
    if (bufferCBtn.justReleased()){
      delay(50); 
     bufferC= calibrar();
     grabarBufferC(bufferC);
     pantallaElegirBuffer();
    }
    if (finalizarBtn.justReleased()){
      delay(50); 
      bandera = 1;
    }
  }
  return;
}

/*************************************************************************************************
 * Pantalla calibrar
 *************************************************************************************************
int calibrar (){
  int bufferT;
  delay(50);
  tft.fillScreen(BLACK);
  cancelar2Btn.initButton(&tft,  80, 210, 140, 40, WHITE, CYAN, BLACK, "CANCELAR", 2); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
  guardarBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "GUARDAR", 2);
  cancelar2Btn.drawButton(false);
  guardarBtn.drawButton(false);
  tft.setTextColor(WHITE);
  tft.setCursor(10,10);
  tft.println("Realice la medicion");
  tft.println("Presione guardar cuando el");
  tft.println("valor en mV se estabilice");
  int bandera =0;
  unsigned long T1=millis();
  unsigned long contador = 0,val=0;
  int val2;
  tft.setTextSize(3);
  while(bandera == 0){
    bool  ab = Touch_getXY(&pixel_x, &pixel_y);
    guardarBtn.press(ab && guardarBtn.contains(pixel_x, pixel_y));
    cancelar2Btn.press(ab && cancelar2Btn.contains(pixel_x, pixel_y));
    analogReadResolution(11);
    val = val + analogRead(36);
    analogReadResolution(10);
    contador++;
    if(millis()>(T1+500)){
      tft.setCursor(40,130);
      tft.setTextColor(BLACK);
      tft.print(val2*3300/2047);
      tft.print(" mV");
      //val = analogRead(36);
      val2 = val/contador;  //calcula el promedio para eliminar ruido
      contador = 0;
      val=0;
      tft.setCursor(40,130);
      tft.setTextColor(WHITE);
      tft.print(val2*3300/2047);
      tft.print(" mV");
      T1=millis();
    }
    
    if (guardarBtn.justReleased()){
      delay(50); 
      bufferT = val2;
      bandera = 1;
    }
    if (cancelar2Btn.justReleased()){
      delay(50); 
      bandera = 1;
    }
  }
  tft.setTextSize(2);
    return(bufferT);
}
/*************************************************************************************************
 * Pantalla medición de pH
 *************************************************************************************************
void medirPH(){
  pantallaMedir();
  bufferA = leerBufferA();
  Serial.print("BufferA = ");
  Serial.println(bufferA);
  bufferB = leerBufferB();
  Serial.print("BufferB = ");
  Serial.println(bufferB);
  bufferC = leerBufferC();
  Serial.print("BufferC = ");
  Serial.println(bufferC);
  bool bandera =0;
  uint16_t gX = 21, gY = 130;
  float ph;
  unsigned long T= millis();
  tft.setCursor(20,200);
  tft.setTextColor(WHITE);
  tft.print(ph);
  tft.print(" pH");
  unsigned long tiempo=millis(); //para indicar tiempo que se hace la grabación. Mejora: reemplazar por hora
  while(bandera == 0){
    bool  d = Touch_getXY(&pixel_x, &pixel_y);
    finalizarGBtn.press(d && finalizarGBtn.contains(pixel_x, pixel_y));
    if (finalizarGBtn.justReleased()){
      bandera = 1;
    }
    analogReadResolution(11);
    int val = analogRead(36);
    analogReadResolution(10);
    
    if(millis()>T+100){
      //gY = map(val,0,2047,160,30);
      //tft.drawPixel(gX,gY,YELLOW);
      tft.setCursor(20,200);
      tft.setTextColor(BLACK);
      tft.print(ph);
      if(val<bufferB){
        //ph = map(val,bufferA,bufferB,(int)(bufferAPH*100),(int)(bufferBPH*100));
        //ph = map(val,bufferA,bufferB,bufferAPH,bufferBPH);
        ph =((val - bufferA) * (bufferBPH - bufferAPH) / (bufferB - bufferA) + bufferAPH);//funcion map
      }
      if(val>bufferB){
        //ph = map(val,bufferB,bufferC,bufferBPH,bufferCPH);
        ph =((val - bufferB) * (bufferCPH - bufferBPH) / (bufferC - bufferB) + bufferBPH);
      }
      guardaSD(val*3300/2047,ph,millis()-tiempo); //se puede mejorar y dar en formato hora
      gY =(ph * (30 - 160)/14+ 160);
      tft.drawPixel(gX,gY,YELLOW);
      tft.print(" pH");
      tft.setCursor(20,200);
      tft.setTextColor(WHITE);
      tft.print(ph);
      tft.print(" pH");
      T = millis();
      gX++;
    }
  }
  return;
}
void pantallaMedir(){
  delay(50);
  tft.fillScreen(BLACK);
  finalizarGBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "FINALIZAR", 2);
  finalizarGBtn.drawButton(false);
  tft.drawLine(20, 30, 20, 160, WHITE);
  tft.drawLine(20, 160, 310, 160, WHITE);
  tft.setTextColor(WHITE);
  tft.setTextSize(1); 
  tft.setCursor(5,5);
  tft.print("pH");
  tft.setCursor(5,30);
  tft.print("14");
  tft.setCursor(5,91);
  tft.print(" 7");
  tft.setCursor(5,153);
  tft.print(" 0");
  tft.setCursor(20,163);
  tft.print("0");
  tft.setCursor(280,163);
  tft.print("tiempo");
  tft.setTextSize(2);
  return;
}
/*************************************************************************************************
 * Pantalla configurar
 *************************************************************************************************
void pantallaConfigurar(){
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(10,15);
  tft.print("A = ");
  tft.setCursor(60,15);
  tft.print(bufferAPH);
  tft.setCursor(170,15);
  tft.print("B = ");
  tft.setCursor(220,15);
  tft.print(bufferBPH);
  tft.setCursor(10,135);
  tft.print("C = ");
  tft.setCursor(60,135);
  tft.print(bufferCPH);
  
  incBufferABtn.initButton(&tft,  40, 90, 60, 40, WHITE, CYAN, BLACK, "+", 2); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
  decBufferABtn.initButton(&tft,  120, 90, 60, 40, WHITE, CYAN, BLACK, "-", 2);
  incBufferBBtn.initButton(&tft, 200, 90, 60, 40, WHITE, CYAN, BLACK, "+", 2);
  decBufferBBtn.initButton(&tft, 280, 90, 60, 40, WHITE, CYAN, BLACK, "-", 2);
  incBufferCBtn.initButton(&tft,  40, 210, 60, 40, WHITE, CYAN, BLACK, "+", 2);
  decBufferCBtn.initButton(&tft,  120, 210, 60, 40, WHITE, CYAN, BLACK, "-", 2);
  finalizarCBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "FINALIZAR", 2);
  incBufferABtn.drawButton(false);//dibuja boton. false es para intercambiar colores
  incBufferBBtn.drawButton(false);
  incBufferCBtn.drawButton(false);
  decBufferABtn.drawButton(false);
  decBufferBBtn.drawButton(false);
  decBufferCBtn.drawButton(false);
  finalizarCBtn.drawButton(false);
  return;
 }
void configurar(){
  int bandera =0;
  pantallaConfigurar();
  
  while(bandera == 0){
    bool  cf = Touch_getXY(&pixel_x, &pixel_y);
    incBufferABtn.press(cf && incBufferABtn.contains(pixel_x, pixel_y));
    decBufferABtn.press(cf && decBufferABtn.contains(pixel_x, pixel_y));
    incBufferBBtn.press(cf && incBufferBBtn.contains(pixel_x, pixel_y));
    decBufferBBtn.press(cf && decBufferBBtn.contains(pixel_x, pixel_y));
    incBufferCBtn.press(cf && incBufferCBtn.contains(pixel_x, pixel_y));
    decBufferCBtn.press(cf && decBufferCBtn.contains(pixel_x, pixel_y));
    finalizarCBtn.press(cf && finalizarCBtn.contains(pixel_x, pixel_y));   
    if (incBufferABtn.justReleased()){
      delay(50); 
      tft.setCursor(60,15);
      tft.setTextColor(BLACK);
      tft.print(bufferAPH);
      bufferAPH = bufferAPH + 0.01;
      tft.setCursor(60,15);
      tft.setTextColor(WHITE);
      tft.print(bufferAPH);
    }
    if (decBufferABtn.justReleased()){
      delay(50); 
      tft.setCursor(60,15);
      tft.setTextColor(BLACK);
      tft.print(bufferAPH);
      bufferAPH = bufferAPH - 0.01;
      tft.setCursor(60,15);
      tft.setTextColor(WHITE);
      tft.print(bufferAPH);
    }
    if (incBufferBBtn.justReleased()){
      delay(50); 
      tft.setCursor(220,15);
      tft.setTextColor(BLACK);
      tft.print(bufferBPH);
      bufferBPH = bufferBPH + 0.01;
      tft.setCursor(220,15);
      tft.setTextColor(WHITE);
      tft.print(bufferBPH);
    }
    if (decBufferBBtn.justReleased()){
      delay(50); 
      tft.setCursor(220,15);
      tft.setTextColor(BLACK);
      tft.print(bufferBPH);
      bufferBPH = bufferBPH - 0.01;
      tft.setCursor(220,15);
      tft.setTextColor(WHITE);
      tft.print(bufferBPH);
    }
    if (incBufferCBtn.justReleased()){
      delay(50); 
      tft.setCursor(60,135);
      tft.setTextColor(BLACK);
      tft.print(bufferCPH);
      bufferCPH = bufferCPH + 0.01;
      tft.setTextColor(WHITE);
      tft.setCursor(60,135);
      tft.print(bufferCPH);
    }
    if (decBufferCBtn.justReleased()){
      delay(50); 
      tft.setCursor(60,135);
      tft.setTextColor(BLACK);
      tft.print(bufferCPH);
      bufferCPH = bufferCPH - 0.01;
      tft.setTextColor(WHITE);
      tft.setCursor(60,135);
      tft.print(bufferCPH);
    }
    if (finalizarCBtn.justReleased()){
      delay(50); 
      bandera = 1;
    }
  }
  return;
}

*/