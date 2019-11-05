/**
* @file		variables.h
* @author	Manuel Caballero
* @date 	2/2/2015
* @brief 	Variables del sistema.
* \copyright
* 	AqueronteBlog@gmail.com
*
* Este archivo es propiedad intelectual del blog Aqueronte,
* cuya direccion web, es la siguiente:
*
* 	http://unbarquero.blogspot.com/
*
* Se permite cualquier modificacion del archivo siempre y cuando
* se mantenga la autoria del autor.
*/

/**
 *  \brief     Variables Globales
 *  \details
 * 				- TX_Buff:		Dicha variable se emplea conjutamente en el servicio de interrupci�n
 *                  	 		de la transmisi�n de datos UART, para indicar el n�mero de Bytes a
 *                  			enviar.
 */


/**
 *  \brief     Variables Globales
 *  \details
 *  			- *pBuff:		Puntero encargado de almacenar los datos le�dos por el dispositivo
 *  							externo nRF24L01+. Se emplea conjuntamente con el servicio de
 *  							interrupci�n de la transmisi�n de datos UART para enviar su contenido.
 */
volatile uint8_t	*pBuff;
