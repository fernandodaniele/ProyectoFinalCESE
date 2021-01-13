/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/12/21
 *===========================================================================*/

/*=============================================================================
 * Inclusiones de archivos y bibliotecas
 *===========================================================================*/
#include <Adafruit_GFX.h>  //librería gráfica para TFT
#include <MCUFRIEND_kbv.h> //librería especifica del módulo
#include "main.h"
#include "pantallaTFT.h"
#include "panelTactil.h"
#include "uart.h"

/*=============================================================================
 * Definiciones y macros
 *===========================================================================*/
#define BLACK   0x0000    //Colores predefinidos. Se pueden agregar o modificar
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define TXT_BTN_SIZE    2
#define CALIBRAR_BTN    "CALIBRAR"
#define TITULAR_BTN     "TITULAR"
#define CONFIGURAR_BTN  "AJUSTES"
#define WIFI_BTN        "WIFI"
#define B1_BTN          "BUFFER 1"
#define B2_BTN          "BUFFER 2"
#define B3_BTN          "BUFFER 3"
#define REGRESAR_BTN    "REGRESAR"
#define T_INC_DEC        70
#define RETARDO_PANTALLA 1000
/*=============================================================================
 * Variables y objetos locales
 *===========================================================================*/
MCUFRIEND_kbv tft;        //Objeto pantalla gráfica

Adafruit_GFX_Button calibrarBtn, titularBtn, configurarBtn, wifiBtn;  //botones pantalla principal
Adafruit_GFX_Button bufferABtn, bufferBBtn, bufferCBtn, regresarBtn;  //botones pantalla elegir buffer
Adafruit_GFX_Button guardarBtn, cancelarBtn;                          //botones pantalla calibrar
Adafruit_GFX_Button finalizarBtn;                                     //botones pantalla medir
Adafruit_GFX_Button agitadorBtn, modBufferBtn, volumenBtn, limpiezaBtn, regresar2Btn;  //botones pantalla configuracion
Adafruit_GFX_Button incBufferABtn, decBufferABtn, incBufferBBtn, decBufferBBtn,
                    incBufferCBtn, decBufferCBtn, guardar2Btn;  //botones pantalla configurar buffers
Adafruit_GFX_Button incVolumenBtn, decVolumenBtn, guardar3Btn;  //botones para la pantalla volumen de corte
Adafruit_GFX_Button finalizar2Btn;                              //boton pantalla limpieza
Adafruit_GFX_Button finalizar3Btn;                              //boton pantalla WiFi
Adafruit_GFX_Button agitadorOnBtn, agitadorOffBtn, regresar3Btn;

char bufferA[6], bufferB[6], bufferC[6]; //variables de calibración en bits formato decimal (0-0V 2047-3.3V)
double bufferAPH = 4.00, bufferBPH = 7.00, bufferCPH = 10.00; //variables de calibración en ph
int pixel_x, pixel_y;   
int volumenCorte = 50;

uint16_t gX, gY; //variables para la curva de titulacion
float ph;
unsigned long T;

/*=============================================================================
 * Definición de funciones globales públicas
 *===========================================================================*/
 //Inicializa el display TFT
 void inicializarTFT(){
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3)
  ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(1); //horizontal
  pantallaInicial();
}

//Muestra la pantalla de inicio
void pantallaInicial(){
  tft.fillScreen(BLACK);  //borra cualquier imagen previa

  //Se inician y muestran los botones
                        // (gfx, x, y, w, h, outline, fill, textcolor, nombre, textsize)
  calibrarBtn.initButton(&tft,  80, 150, 140, 40, WHITE, CYAN, BLACK, CALIBRAR_BTN, TXT_BTN_SIZE); 
  titularBtn.initButton(&tft, 240, 150, 140, 40, WHITE, CYAN, BLACK, TITULAR_BTN, TXT_BTN_SIZE);
  configurarBtn.initButton(&tft,  80, 210, 140, 40, WHITE, CYAN, BLACK, CONFIGURAR_BTN, TXT_BTN_SIZE);
  wifiBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, WIFI_BTN, TXT_BTN_SIZE);
  calibrarBtn.drawButton(false);//dibuja boton. false es para intercambiar colores
  titularBtn.drawButton(false);
  configurarBtn.drawButton(false);
  wifiBtn.drawButton(false);

  //Se dibuja un rectangulo para escribir dentro
  tft.drawRect(10, 10, 300, 110, WHITE); //(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  tft.setCursor(15,13);
  tft.setTextColor(WHITE);
  tft.print("...... TITULADOR .......");
  tft.setCursor(15,38);
  tft.print("...... AUTOMATICO ......");
  tft.setCursor(15,61);
  tft.print("Desarrollado por");
  tft.setCursor(15,86);
  tft.print("U.T.N. San Francisco.");
}

//Consulta si se presionó algún botón en la pantalla inicial
int tactilInicial(){
  //Consulta si se presionó el tactil y guarda las coordenadas
  bool down = Touch_getXY(&pixel_x, &pixel_y);
  
  //Compara el dato obtenido con los botones de la pantalla
  calibrarBtn.press(down && calibrarBtn.contains(pixel_x, pixel_y));
  titularBtn.press(down && titularBtn.contains(pixel_x, pixel_y));
  configurarBtn.press(down && configurarBtn.contains(pixel_x, pixel_y));
  wifiBtn.press(down && wifiBtn.contains(pixel_x, pixel_y));

  //Si alguno de los botones fue soltado, retorna el valor correspondiente
  if (calibrarBtn.justReleased()) {
    return 1;
  }
  else if (titularBtn.justReleased()) {
    return 2; 
  }
  else if (configurarBtn.justReleased()) {
    return 3;
  }
  else if (wifiBtn.justReleased()) {
    return 4; 
  }
  else{
    return 0;
  } 
}

//Muestra la pantalla con el valor actual de cada buffer y da la opción de elegir 
//cada uno de ellos por separado para realizar la calibración
void pantallaElegirBuffer(){
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,20);
    tft.print("Leyendo valor actual de buffers... ");
    //Solicita al ESP el valor de cada uno de los buffers
    if(leerElectrodoA(bufferA)==0){
      tft.print("Error al leer buffer A... ");
    }
    else if (leerElectrodoB(bufferB)==0){
      tft.print("Error al leer buffer B... ");
    }
    else if (leerElectrodoC(bufferC)==0){
      tft.print("Error al leer buffer C... ");
    }
    else{
      //Limpia la pantalla y muestra el valor actual de los buffers
      tft.fillScreen(BLACK);
      tft.setTextColor(WHITE);
      tft.setCursor(10,20);
      tft.print("bufferA = ");
      tft.print(bufferA);
      tft.setCursor(10,40);
      tft.print("bufferB = ");
      tft.print(bufferB);
      tft.setCursor(10,60);
      tft.print("bufferC = ");
      tft.print(bufferC);
    }
    //bufferC = leerBufferC();
   
    //Dibuja los botones correspondientes a esta pantalla
    bufferABtn.initButton(&tft,  80, 150, 140, 40, WHITE, CYAN, BLACK, B1_BTN, TXT_BTN_SIZE);
    bufferBBtn.initButton(&tft, 240, 150, 140, 40, WHITE, CYAN, BLACK, B2_BTN, TXT_BTN_SIZE);
    bufferCBtn.initButton(&tft,  80, 210, 140, 40, WHITE, CYAN, BLACK, B3_BTN, TXT_BTN_SIZE);
    regresarBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, REGRESAR_BTN, TXT_BTN_SIZE);
    bufferABtn.drawButton(false);
    bufferBBtn.drawButton(false);
    bufferCBtn.drawButton(false);
    regresarBtn.drawButton(false);
}

//Consulta si se presionó algún botón en la pantalla de elección de buffer
int tactilElegirBuffer(){
    bool  ec = Touch_getXY(&pixel_x,&pixel_y);
    bufferABtn.press(ec && bufferABtn.contains(pixel_x, pixel_y));
    bufferBBtn.press(ec && bufferBBtn.contains(pixel_x, pixel_y));
    bufferCBtn.press(ec && bufferCBtn.contains(pixel_x, pixel_y));
    regresarBtn.press(ec && regresarBtn.contains(pixel_x, pixel_y));
    if (bufferABtn.justReleased()){
      return 1;
    }
    else if (bufferBBtn.justReleased()){
      return 2;
    }
    else if (bufferCBtn.justReleased()){
      return 3;
    }
    else if (regresarBtn.justReleased()){
      return 4; 
    }
    else{
      return 0;
    }
}

//Muestra la pantalla con el valor actual de ph que será guardado como valor para calibrar el instrumento
void pantallaCalibrar (){
  tft.fillScreen(BLACK);
  cancelarBtn.initButton(&tft,  80, 210, 140, 40, WHITE, CYAN, BLACK, "CANCELAR", TXT_BTN_SIZE);
  guardarBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "GUARDAR", TXT_BTN_SIZE);
  cancelarBtn.drawButton(false);
  guardarBtn.drawButton(false);
  tft.setTextColor(WHITE);
  tft.setCursor(10,10);
  tft.println("Realice la medicion");
  tft.println("Presione guardar cuando el");
  tft.println("valor en mV se estabilice");
}

//Consulta si se presionó algún botón en la pantalla de elección de buffer
int tactilCalibrar(){
  float tempMV = ph;
  //Acá tengo que leer el valor del electrodo desde el ESP
  if(leerElectrodo(&ph)==0){
      tft.print("Error al leer el electrodo... ");
  }
  tft.setCursor(100,110);
  tft.setTextColor(BLACK);
  tft.print(tempMV);
  tft.print(" mV");
  //Acá tengo que mostrar en pantalla ese valor leido
  tft.setCursor(100,110);
  tft.setTextColor(WHITE);
  tft.print(ph);
  tft.print(" mV");

  bool  ab = Touch_getXY(&pixel_x, &pixel_y);
  guardarBtn.press(ab && guardarBtn.contains(pixel_x, pixel_y));
  cancelarBtn.press(ab && cancelarBtn.contains(pixel_x, pixel_y));

  if (guardarBtn.justReleased()){
    return 3;
  }
  else if (cancelarBtn.justReleased()){
    return 4;
  }
  else{
    return 0;
  }
}

//Muestra la pantalla con la curva de titulación y el valor actual de pH
void pantallaMedir(){
 
  tft.fillScreen(BLACK);
  finalizarBtn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "FINALIZAR", TXT_BTN_SIZE);
  finalizarBtn.drawButton(false);

  //Dibuja el grafico
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

  tft.setCursor(20,200);
  tft.setTextColor(WHITE);
  if(leerElectrodo(&ph)==0){
      tft.print("Error al leer el electrodo... ");
  }
  tft.print(ph);
  tft.print(" pH");
  T= millis();
  gX = 20;
}

//Actualiza el grafico y consulta si se presionó el boton de finalizar
int tactilMedir(){
  
  if(millis()>T+0){

    float tmpPH = ph;
    //Acá debería consultar al ESP el valor del pH
    if(leerElectrodo(&ph)==0){
      tft.print("Error al leer el electrodo... ");
    }

    gY =(ph * (30 - 160)/14+ 160);
    tft.drawPixel(gX,gY,YELLOW);

    tft.setCursor(20,200);
    tft.setTextColor(BLACK);
    tft.print(tmpPH);
    tft.print(" pH");
    tft.setCursor(20,200);
    tft.setTextColor(WHITE);
    tft.print(ph);
    tft.print(" pH");
    T = millis();
    gX++;
  }
  
  bool  d = Touch_getXY(&pixel_x, &pixel_y);
  finalizarBtn.press(d && finalizarBtn.contains(pixel_x, pixel_y));
  
  if (finalizarBtn.justReleased()){
    return 4;
  }
  else{
    return 0;
  }

}

//Muestra la pantalla con la opciones para configurar los buffers a utilizar, el volumen de corte y para ejecutar la limpieza de la bomba
void pantallaConfigurar()
{
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,20); 
    tft.print ("Seleccione una opcion");
    modBufferBtn.initButton(&tft,  80, 90, 140, 40, WHITE, CYAN, BLACK, "BUFFERS", TXT_BTN_SIZE); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
    volumenBtn.initButton(&tft, 240, 90, 140, 40, WHITE, CYAN, BLACK, "VOLUMEN", TXT_BTN_SIZE);
    limpiezaBtn.initButton(&tft,  80, 150, 140, 40, WHITE, CYAN, BLACK, "LIMPIEZA", TXT_BTN_SIZE); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
    agitadorBtn.initButton(&tft, 240, 150, 140, 40, WHITE, CYAN, BLACK, "AGITADOR", TXT_BTN_SIZE);
    regresar2Btn.initButton(&tft, 160, 210, 300, 40, WHITE, CYAN, BLACK, "REGRESAR", TXT_BTN_SIZE);
    modBufferBtn.drawButton(false);
    volumenBtn.drawButton(false);
    limpiezaBtn.drawButton(false);
    agitadorBtn.drawButton(false);
    regresar2Btn.drawButton(false);
}

//Consulta si se presionó algún botón en la pantalla de configuracion
int tactilConfigurar(){
    bool  presionado = Touch_getXY(&pixel_x,&pixel_y);
    modBufferBtn.press(presionado && modBufferBtn.contains(pixel_x, pixel_y));
    volumenBtn.press(presionado && volumenBtn.contains(pixel_x, pixel_y));
    limpiezaBtn.press(presionado && limpiezaBtn.contains(pixel_x, pixel_y));
    agitadorBtn.press(presionado && agitadorBtn.contains(pixel_x, pixel_y));
    regresar2Btn.press(presionado && regresar2Btn.contains(pixel_x, pixel_y));
    if (modBufferBtn.justReleased()){
      return 1;
    }
    else if (volumenBtn.justReleased()){
     return 2;
    }
    else if (limpiezaBtn.justReleased()){
     return 3;
    }
    else if (agitadorBtn.justReleased()){
      return 4; 
    }
    else if (regresar2Btn.justReleased()){
      return 5; 
    }
    else{
      return 0;
    }
  
}

//Muestra la pantalla que permite ajustar el valor de cada uno de los 3 buffers que se utilizaran
void pantallaConfigurarBuffer(){

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(10,15);
  tft.print("Leyendo valor actual \n de buffers...");

  if(leerBufferA(bufferA)==0){
      tft.print("Error al leer buffer A... ");
  }
  bufferAPH = atoi(bufferA);
  bufferAPH = bufferAPH /100;

  if(leerBufferB(bufferB)==0){
      tft.print("Error al leer buffer B... ");
  }
  bufferBPH = atoi(bufferB);
  bufferBPH = bufferBPH /100;

  if(leerBufferC(bufferC)==0){
      tft.print("Error al leer buffer C... ");
  }
  bufferCPH = atoi(bufferC);
  bufferCPH = bufferCPH /100;

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
  
  incBufferABtn.initButton(&tft,  40, 90, 60, 40, WHITE, CYAN, BLACK, "+", TXT_BTN_SIZE);
  decBufferABtn.initButton(&tft,  120, 90, 60, 40, WHITE, CYAN, BLACK, "-", TXT_BTN_SIZE);
  incBufferBBtn.initButton(&tft, 200, 90, 60, 40, WHITE, CYAN, BLACK, "+", TXT_BTN_SIZE);
  decBufferBBtn.initButton(&tft, 280, 90, 60, 40, WHITE, CYAN, BLACK, "-", TXT_BTN_SIZE);
  incBufferCBtn.initButton(&tft,  40, 210, 60, 40, WHITE, CYAN, BLACK, "+", TXT_BTN_SIZE);
  decBufferCBtn.initButton(&tft,  120, 210, 60, 40, WHITE, CYAN, BLACK, "-", TXT_BTN_SIZE);
  guardar2Btn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "GUARDAR", TXT_BTN_SIZE);
  incBufferABtn.drawButton(false);
  incBufferBBtn.drawButton(false);
  incBufferCBtn.drawButton(false);
  decBufferABtn.drawButton(false);
  decBufferBBtn.drawButton(false);
  decBufferCBtn.drawButton(false);
  guardar2Btn.drawButton(false);
 }

//Permite ajustar el valor de los buffers mediante botones y consulta si se presionó el boton de guardar
int  tactilConfigurarBuffer()
{
  bool  presionado = Touch_getXY(&pixel_x, &pixel_y);
  incBufferABtn.press(presionado && incBufferABtn.contains(pixel_x, pixel_y));
  decBufferABtn.press(presionado && decBufferABtn.contains(pixel_x, pixel_y));
  incBufferBBtn.press(presionado && incBufferBBtn.contains(pixel_x, pixel_y));
  decBufferBBtn.press(presionado && decBufferBBtn.contains(pixel_x, pixel_y));
  incBufferCBtn.press(presionado && incBufferCBtn.contains(pixel_x, pixel_y));
  decBufferCBtn.press(presionado && decBufferCBtn.contains(pixel_x, pixel_y));
  guardar2Btn.press(presionado && guardar2Btn.contains(pixel_x, pixel_y)); 

  if (incBufferABtn.justReleased()){
    delay(T_INC_DEC);
    tft.setCursor(60,15);
    tft.setTextColor(BLACK);
    tft.print(bufferAPH);
    bufferAPH = bufferAPH + 0.01;
    tft.setCursor(60,15);
    tft.setTextColor(WHITE);
    tft.print(bufferAPH);
  }
  else if (decBufferABtn.justReleased()){
    delay(T_INC_DEC);
    tft.setCursor(60,15);
    tft.setTextColor(BLACK);
    tft.print(bufferAPH);
    bufferAPH = bufferAPH - 0.01;
    tft.setCursor(60,15);
    tft.setTextColor(WHITE);
    tft.print(bufferAPH);
  }
  else if (incBufferBBtn.justReleased()){
    delay(T_INC_DEC);
    tft.setCursor(220,15);
    tft.setTextColor(BLACK);
    tft.print(bufferBPH);
    bufferBPH = bufferBPH + 0.01;
    tft.setCursor(220,15);
    tft.setTextColor(WHITE);
    tft.print(bufferBPH);
  }
  else if (decBufferBBtn.justReleased()){
    delay(T_INC_DEC);
    tft.setCursor(220,15);
    tft.setTextColor(BLACK);
    tft.print(bufferBPH);
    bufferBPH = bufferBPH - 0.01;
    tft.setCursor(220,15);
    tft.setTextColor(WHITE);
    tft.print(bufferBPH);
  }
  else if (incBufferCBtn.justReleased()){
    delay(T_INC_DEC); 
    tft.setCursor(60,135);
    tft.setTextColor(BLACK);
    tft.print(bufferCPH);
    bufferCPH = bufferCPH + 0.01;
    tft.setTextColor(WHITE);
    tft.setCursor(60,135);
    tft.print(bufferCPH);
  }
  else if (decBufferCBtn.justReleased()){
    delay(T_INC_DEC);
    tft.setCursor(60,135);
    tft.setTextColor(BLACK);
    tft.print(bufferCPH);
    bufferCPH = bufferCPH - 0.01;
    tft.setTextColor(WHITE);
    tft.setCursor(60,135);
    tft.print(bufferCPH);
  }
  else if (guardar2Btn.justReleased()){
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,15);
    tft.print("Guardando valor \n de buffers...");
    char bufferStr [6];
    int temp = (int) (bufferAPH * 100) ;
    sprintf(bufferStr, "%d", temp);
    grabarBufferA(bufferStr);

    temp = (int) (bufferBPH * 100) ;
    sprintf(bufferStr, "%d", temp);
    grabarBufferB(bufferStr);

    temp = (int) (bufferCPH * 100) ;
    sprintf(bufferStr, "%d", temp);
    grabarBufferC(bufferStr);
    return 4;
  }
  else
  {
    return 0;
  }
}

//Muestra la pantalla que permite ajustar el valor del volumen de corte
void pantallaVolumenCorte()
{
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(10,15);
  tft.print("Configurar el volumen de corte");
  tft.setCursor(10,135);
  leerVolumenCorte(&volumenCorte);
  tft.print("Volumen = ");
  tft.setCursor(125,135);
  tft.print(volumenCorte);

  incVolumenBtn.initButton(&tft,  40, 210, 60, 40, WHITE, CYAN, BLACK, "+", TXT_BTN_SIZE);
  decVolumenBtn.initButton(&tft,  120, 210, 60, 40, WHITE, CYAN, BLACK, "-", TXT_BTN_SIZE);
  guardar3Btn.initButton(&tft, 240, 210, 140, 40, WHITE, CYAN, BLACK, "GUARDAR", TXT_BTN_SIZE);
  incVolumenBtn.drawButton(false);
  decVolumenBtn.drawButton(false);
  guardar3Btn.drawButton(false);
 }

//Permite configurar el volumen de corte y consulta si se presionó el boton de guardar
int  tactilVolumenCorte()
{
  bool  presionado = Touch_getXY(&pixel_x, &pixel_y);
  incVolumenBtn.press(presionado && incVolumenBtn.contains(pixel_x, pixel_y));
  decVolumenBtn.press(presionado && decVolumenBtn.contains(pixel_x, pixel_y));
  guardar3Btn.press(presionado && guardar3Btn.contains(pixel_x, pixel_y));   
  if (incVolumenBtn.justReleased()){
    delay(T_INC_DEC); 
    tft.setCursor(125,135);
    tft.setTextColor(BLACK);
    tft.print(volumenCorte);
    volumenCorte = volumenCorte + 1;
    tft.setTextColor(WHITE);
    tft.setCursor(125,135);
    tft.print(volumenCorte);
  }
  else if (decVolumenBtn.justReleased()){
    delay(T_INC_DEC);
    tft.setCursor(125,135);
    tft.setTextColor(BLACK);
    tft.print(volumenCorte);
    volumenCorte = volumenCorte - 1;
    tft.setTextColor(WHITE);
    tft.setCursor(125,135);
    tft.print(volumenCorte);
  }
  else if (guardar3Btn.justReleased()){
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,15);
    tft.print("Guardando volumen de corte...");
    char volumenStr [6];
    sprintf(volumenStr, "%d", volumenCorte);
    guardarVolumenCorte(volumenStr);
    return 4;
  }
  else
  {
    return 0;
  }
}

//Muestra la pantalla de limpieza de la bomba
void pantallaLimpieza ()
{
  tft.fillScreen(BLACK);
  finalizar2Btn.initButton(&tft,  80, 210, 140, 40, WHITE, CYAN, BLACK, "FINALIZAR", TXT_BTN_SIZE);
  finalizar2Btn.drawButton(false);

  tft.setTextColor(WHITE);
  tft.setCursor(10,10);
  tft.println("Realizando limpieza");
}

//Consulta si se presionó el boton de finalizar
int tactilLimpieza()
{
  bool  presionado = Touch_getXY(&pixel_x, &pixel_y);
  finalizar2Btn.press(presionado && finalizar2Btn.contains(pixel_x, pixel_y));

  if (finalizar2Btn.justReleased()){ 
    return 4;
  }
  else{
    return 0;
  }
}

//Muestra la pantalla con los datos para conectarse al WiFi
void pantallaWIFI ()
{
  tft.fillScreen(BLACK);
  finalizar3Btn.initButton(&tft,  80, 210, 140, 40, WHITE, CYAN, BLACK, "FINALIZAR", 2); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
  finalizar3Btn.drawButton(false);

  tft.setTextColor(WHITE);
  tft.setCursor(10,10);
  tft.println("Pantalla wifi");
}

//Consulta si se presionó el boton de finalizar
int tactilWIFI()
{
  bool  presionado = Touch_getXY(&pixel_x, &pixel_y);
  finalizar3Btn.press(presionado && finalizar3Btn.contains(pixel_x, pixel_y));

  if (finalizar3Btn.justReleased()){
    return 4;
  }
  else{
    
    return 0;
  }
}

void imprimirError(){
  tft.fillScreen(BLACK);
  tft.print("Error");
  delay(RETARDO_PANTALLA);
}

void imprimirGuardando()
{
  tft.fillScreen(BLACK);
  tft.print("Guardando...");
}

void pantallaCalibrarA()
{
  imprimirGuardando();
  //Cuando se de guardar hay que enviar un señal para que se guarde ese valor
  if(calibrarBufferA()==0)
  {
      imprimirError();
  }
  else
  {
      //leer valor guardado y mostrar en pantalla
  }
}

void pantallaCalibrarB()
{
  imprimirGuardando();
  //Cuando se de guardar hay que enviar un señal para que se guarde ese valor
  if(calibrarBufferB()==0)
  {
      imprimirError();
  }
  else
  {
      //leer valor guardado y mostrar en pantalla
  }
}

void pantallaCalibrarC()
{
  imprimirGuardando();
  //Cuando se de guardar hay que enviar un señal para que se guarde ese valor
  if(calibrarBufferC()==0)
  {
      imprimirError();
  }
  else
  {
      //leer valor guardado y mostrar en pantalla
  }
}


void pantallaAgitador()
{
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,20); 
    tft.println ("Seleccione una opcion");
    agitadorOnBtn.initButton(&tft,  80, 150, 140, 40, WHITE, CYAN, BLACK, "ON", TXT_BTN_SIZE); // (gfx, x, y, w, h, outline, fill, textcolor, "Calibracion", textsize)
    agitadorOffBtn.initButton(&tft, 240, 150, 140, 40, WHITE, CYAN, BLACK, "OFF", TXT_BTN_SIZE);
    regresar3Btn.initButton(&tft, 160, 210, 300, 40, WHITE, CYAN, BLACK, "REGRESAR", TXT_BTN_SIZE);
    agitadorOnBtn.drawButton(false);
    agitadorOffBtn.drawButton(false);
    regresar3Btn.drawButton(false);
}


int tactilAgitador(){
    bool  presionado = Touch_getXY(&pixel_x,&pixel_y);
    agitadorOnBtn.press(presionado && agitadorOnBtn.contains(pixel_x, pixel_y));
    agitadorOffBtn.press(presionado && agitadorOffBtn.contains(pixel_x, pixel_y));
    regresar3Btn.press(presionado && regresar3Btn.contains(pixel_x, pixel_y));
    if (agitadorOnBtn.justReleased()){
      return 1;
    }
    else if (agitadorOffBtn.justReleased()){
      return 2;
    }
    else if (regresar3Btn.justReleased()){
      return 4; 
    }
    else{
      return 0;
    }
  
}

void habilitoAgitador()
{
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,20); 
    tft.println ("Agitador habilitado");
    delay(RETARDO_PANTALLA);
}

void deshabilitoAgitador()
{
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,20); 
    tft.print("Agitador deshabilitado");
    delay(RETARDO_PANTALLA);
}

void errorAgitador()
{
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setCursor(10,20); 
    tft.println ("Error al configurar agitador");
    delay(RETARDO_PANTALLA);
}