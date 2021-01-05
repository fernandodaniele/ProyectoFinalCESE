/*=============================================================================
 * Autor: Fernando Ezequiel Daniele <fernandodaniele1993@gmai.com>
 * Fecha: 2020/12/21
 *===========================================================================*/
 
 /*=============================================================================
 * Declaración de funciones globales públicas
 *===========================================================================*/
 /**
  * @brief Inicializa el display TFT
  */
void inicializarTFT();

/**
 * @brief Muestra la pantalla de inicio
 * 
 */
void pantallaInicial();

/**
 * @brief Consulta si se presionó algún botón en la pantalla inicial
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 1 Botón de calibrar
 *  @li @c 2 Botón de titular
 *  @li @c 3 Botón de configurar
 *  @li @c 4 Botón de WiFi
 */
int tactilInicial();

/**
 * @brief Muestra la pantalla con el valor actual de cada buffer y da
 *        la opción de elegir cada uno de ellos por separado para
 *        realizar la calibración
 */
void pantallaElegirBuffer();

/**
 * @brief Consulta si se presionó algún botón en la pantalla de elección de buffer
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 1 Botón de buffer 1
 *  @li @c 2 Botón de buffer 2
 *  @li @c 3 Botón de buffer 3
 *  @li @c 4 Botón de regresar
 */
int tactilElegirBuffer();

/**
 * @brief Muestra la pantalla con el valor actual de ph que será guardado como 
 *        valor para calibrar el instrumento
 * 
 */
void pantallaCalibrar ();

/**
 * @brief Consulta si se presionó algún botón en la pantalla de elección de buffer
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 3 Botón de guardar
 *  @li @c 4 Botón de cancelar
 */
int tactilCalibrar();

/**
 * @brief Muestra la pantalla con la curva de titulación y el valor actual de pH
 * 
 */
void pantallaMedir();

/**
 * @brief Actualiza el grafico y consulta si se presionó el boton de finalizar
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 4 Botón de finalizar
 */
int tactilMedir();

/**
 * @brief Muestra la pantalla con la opciones para configurar los buffers a utilizar,
 * el volumen de corte y para ejecutar la limpieza de la bomba
 * 
 */
void pantallaConfigurar();

/**
 * @brief Consulta si se presionó algún botón en la pantalla de configuracion
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 1 Botón para configurar los valores de los buffers
 *  @li @c 2 Botón para configurar el volumen de corte
 *  @li @c 3 Botón para realizar la limpieza de la bomba
 *  @li @c 4 Botón de regresar
 */
int  tactilConfigurar();

/**
 * @brief Muestra la pantalla que permite ajustar el valor de cada uno de los 3 buffers que se utilizaran
 * 
 */
void pantallaConfigurarBuffer();

/**
 * @brief Permite ajustar el valor de los buffers mediante botones y consulta si se presionó el boton de guardar
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 4 Botón de finalizar
 */
int  tactilConfigurarBuffer();

/**
 * @brief Muestra la pantalla que permite ajustar el valor del volumen de corte
 * 
 */
void pantallaVolumenCorte();

/**
 * @brief Permite configurar el volumen de corte y consulta si se presionó el boton de guardar
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 4 Botón de finalizar
 */
int  tactilVolumenCorte();

/**
 * @brief Muestra la pantalla de limpieza de la bomba
 * 
 */
void pantallaLimpieza();

/**
 * @brief Consulta si se presionó el boton de finalizar
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 4 Botón de finalizar
 */
int tactilLimpieza();

/**
 * @brief Muestra la pantalla con los datos para conectarse al WiFi
 * 
 */
void pantallaWIFI ();

/**
 * @brief Consulta si se presionó el boton de finalizar
 * 
 * @return int 
 *  @li @c 0 No se presionó ningún boton
 *  @li @c 4 Botón de finalizar
 */
int tactilWIFI ();

void imprimirError();

void imprimirGuardando();

void pantallaCalibrarA();

void pantallaCalibrarB();

void pantallaCalibrarC();

void pantallaAgitador();

int tactilAgitador();

void habilitoAgitador();

void deshabilitoAgitador();

void errorAgitador();