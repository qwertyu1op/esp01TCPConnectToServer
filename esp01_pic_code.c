/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011NihalBrarath.c  
	Author:		NihalBrarath
	Date:		08/02/2025
	Modified:	None
	© Fanshawe College, 2025

	Description: Describe the purpose of the whole program.  What the 
			program does and why. What platform (hardware part number) and / or 
			supplemental documents required for the program’s use. 
			Describe any supporting information (schematic, programming 
			environment, ect..). 
=============================================================================*/

/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC		= INTIO67
#pragma config PLLCFG	= OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN	= OFF
#pragma config IESO		= OFF
#pragma config PWRTEN	= OFF 
#pragma config BOREN	= ON
#pragma config BORV		= 285 
#pragma config WDTEN	= OFF
#pragma config PBADEN	= OFF
#pragma config LVP		= OFF
#pragma config MCLRE	= EXTMCLR

// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
 

// Constants  =================================================================

#define TRUE 1
#define FALSE 0

#define BITTO 10
#define SANJAY 11
#define NIHAL 12

#define ESP8266_STATION 0x01
#define ESP8266_SOFTAP 0x02

#define ESP8266_TCP 1
#define ESP8266_UDP 0

#define ESP8266_OK 1
#define ESP8266_READY 2
#define ESP8266_FAIL 3
#define ESP8266_NOCHANGE 4
#define ESP8266_LINKED 5
#define ESP8266_UNLINK 6

#define	ESP8266_H

unsigned char _esp8266_waitResponse(void);
void esp8266_mode(unsigned char mode);
void _esp8266_putch(char bt);
char _esp8266_getch();
void ESP8266_send_string(char* st_pt);

/*
I THINK ITS BETTER TO CALL THESE INLINE AND HAVE ITERATION AT THE FUNTION
THE ESP8266 AND PIC LIB DOES IT THAT WAY I WILL PORT THE FUNCTIONS
YOULL KNOW CAUSE THEY HAVE FANCY HEADERS
const char atOK[]="AT\r\n";
const char atRST[]="AT+RST\r\n";
const char atCWMODE[]="AT+CWMODE=1\r\n";
const char atCWJAP[]="AT+CWJAP=\"Pixel_8296\",\"nihalbra\"\r\n";
const char atCWJAP2[]="AT+CWJAP?\r\n";
const char atCIPSTART[]="AT+CIPSTART=\"TCP\",\"10.35.64.57\",8080\r\n";
const char atCIPSEND[]="AT+CIPSEND=26\r\n";
const char atCIPCLOSE[]="AT+CIPCLOSE\r\n";
const char atCIPSEND2[]="AT+CIPSEND=2\r\n";
const char atGMR[]="AT+GMR\r\n";

void connectTCP()
{
	sendUART(atOK);
	sendUART(atRST);
	Delay10TCYx( 2500 );
	
	sendUART(atCWMODE);
	Delay10TCYx( 2500 );
	sendUART(atCWJAP);
	Delay10TCYx( 2500 );

	sendUART(atCWJAP2);
	Delay10TCYx( 2500 );
	sendUART(atCIPSTART);
	Delay10TCYx( 2500 );

	sendUART(atCIPSEND);
	Delay10TCYx( 2500 );

	sendUART("HELLO WORLD");
	sendUART(atCIPSEND2);
	Delay10TCYx( 2500 );

	sendUART(atCIPCLOSE);
	Delay10TCYx( 2500 );
}
*/

/**
 * Output a string to the ESP module.
 *
 * This is a function for internal use only.
 *
 * @param ptr A pointer to the string to send.
 */
void _esp8266_print(unsigned const char *ptr) {
    while (*ptr != 0) {
        _esp8266_putch(*ptr++);
    }
}

void setOscTo16MHZ()
{
	OSCCON=0x72;
	while(!OSCCONbits.HFIOFS);
}

void configAsyncUART()
{
	RCONbits.IPEN=TRUE;
	INTCONbits.GIE=TRUE;
	PIE1bits.RC1IE=TRUE;
	
	//STEP1 SET BAUDRATE 114K
	SPBRG1H:SPBRGH1=34;
	TXSTA1bits.BRGH= TRUE;
	BAUDCON1bits.BRG16= TRUE;
	
	//STEP2 MAKE TRIS 1
	TRISCbits.RC6=TRUE;
	TRISCbits.RC7=TRUE;

	//STEP3 ENABLE ASYNC AND ENABLES PORT PINS
	TXSTA1bits.SYNC=FALSE;
	RCSTA1bits.SPEN=TRUE;

	//TX ENABLE
	TXSTA1bits.TXEN=TRUE;
	
	//RX ENABLE
	RCSTA1bits.CREN=TRUE;
	
	//NOW WHEN WE SEND A BYTE TO TXREG1 THIS WILL TRANSMIT set ansel maybe?
}

void sendUART(char* data)
{
	while(*data)
	{
		//while(!PIR1bits.TX1IF); may not be as accurate as we think
		while(!TXSTAbits.TRMT);
		TXREG1=*data++;
		
	}
//	while(!PIR1bits.RC1IF); to check if i am getting any response
//	PIR1bits.RC1IF=FALSE;
}


//**Function to send one byte of date to UART**//
void _esp8266_putch(char bt)  
{
    while(!PIR1bits.TXIF);  // hold the program till TX buffer is free
    TXREG = bt; //Load the transmitter buffer with the received value
}
//_____________End of function________________//
 
//**Function to get one byte of date from UART**//
char _esp8266_getch()   
{
    if(RCSTA1bits.OERR) // check for Error 
    {
        RCSTA1bits.CREN = 0; //If error -> Reset 
        RCSTA1bits.CREN = 1; //If error -> Reset YOU WILL ALSO NEED TO RESET THE SPEN BIT
    }
    
    while(!PIR1bits.RC1IF);  // hold the program till RX buffer is free
    
    return RCREG1; //receive the value and send it to main function
}
//_____________End of function________________//
 

//**Function to convert string to byte**//
void ESP8266_send_string(char* st_pt)
{
    while(*st_pt) //if there is a char
        _esp8266_putch(*st_pt++); //process it as a byte data
}
//___________End of function______________//



/**
 * Restart the module
 *
 * This sends the `AT+RST` command to the ESP and waits until there is a
 * response.
 *
 * @return true iff the module restarted properly
 */
char esp8266_restart(void) {
    ESP8266_send_string("AT+RST\r\n");
    if (_esp8266_waitResponse() != ESP8266_OK) {
        return FALSE;
    }
    return (_esp8266_waitResponse() == ESP8266_READY);
}


/**
 * Connect to an access point.
 *
 * This sends the AT+CWJAP command to the ESP module.
 *
 * @param ssid The SSID to connect to
 * @param pass The password of the network
 * @return an ESP status code, normally either ESP8266_OK or ESP8266_FAIL
 */
unsigned char esp8266_connect(unsigned char* ssid, unsigned char* pass) {
    _esp8266_print("AT+CWJAP=\"");
    _esp8266_print(ssid);
    _esp8266_print("\",\"");
    _esp8266_print(pass);
    _esp8266_print("\"\r\n");
    return _esp8266_waitResponse();
}

/**
 * Open a TCP or UDP connection.
 *
 * This sends the AT+CIPSTART command to the ESP module.
 *
 * @param protocol Either ESP8266_TCP or ESP8266_UDP
 * @param ip The IP or hostname to connect to; as a string
 * @param port The port to connect to
 *
 * @return true iff the connection is opened after this.
 */
char esp8266_start(unsigned char protocol, char* ip, unsigned char port) {
    _esp8266_print("AT+CIPSTART=\"");
    if (protocol == ESP8266_TCP) {
        _esp8266_print("TCP");
    } else {
        _esp8266_print("UDP");
    }
    _esp8266_print("\",\"");
    _esp8266_print(ip);
    _esp8266_print("\",");
    unsigned char port_str[5] = {"\0\0\0\0"};
    sprintf(port_str, "%u", port);
    _esp8266_print(port_str);
    _esp8266_print("\r\n");
    if (_esp8266_waitResponse() != ESP8266_OK) {
        return 0;
    }
    if (_esp8266_waitResponse() != ESP8266_LINKED) {
        return 0;
    }
    return 1;
}

/**
 * Set the WiFi mode.
 *
 * ESP8266_STATION : Station mode
 * ESP8266_SOFTAP : Access point mode
 *
 * This sends the AT+CWMODE command to the ESP module.
 *
 * @param mode an ORed bitmask of ESP8266_STATION and ESP8266_SOFTAP
 */
void esp8266_mode(unsigned char mode) {
    ESP8266_send_string("AT+CWMODE=");
    _esp8266_putch(mode + '0');
    ESP8266_send_string("\r\n");
    return (_esp8266_waitResponse()== ESP8266_OK);
}


/**
 * Wait until we received the ESP is done and sends its response.
 *
 * This is a function for internal use only.
 *
 * Currently the following responses are implemented:
 *  * OK
 *  * ready
 *  * FAIL
 *  * no change
 *  * Linked
 *  * Unlink
 *
 * Not implemented yet:
 *  * DNS fail (or something like that)
 *
 * @return a constant from esp8266.h describing the status response.
 */
unsigned char _esp8266_waitResponse(void) {
    unsigned char so_far[6] = {0,0,0,0,0,0};
    unsigned const char lengths[6] = {2,5,4,9,6,6};
    unsigned const char* strings[6] = {"OK", "ready", "FAIL", "no change", "Linked", "Unlink"};
    unsigned const char responses[6] = {ESP8266_OK, ESP8266_READY, ESP8266_FAIL, ESP8266_NOCHANGE, ESP8266_LINKED, ESP8266_UNLINK};
    unsigned char received;
    unsigned char response;
    unsigned char continue_loop = TRUE;
	unsigned char i;
    while (continue_loop) {
        received = _esp8266_getch();
        for (i = 0; i < 6; i++) {
            if (strings[i][so_far[i]] == received) {
                so_far[i]++;
                if (so_far[i] == lengths[i]) {
                    response = responses[i];
                    continue_loop = FALSE;
                }
            } else {
                so_far[i] = 0;
            }
        }
    }
    return response;
}

void sysInit()
{
	configAsyncUART();
	setOscTo16MHZ();
}





void main() 
{
	
	//0A AND 0D ARE \n and \r respectively
	//int dataf=010;

	sysInit();
	
	esp8266_mode(ESP8266_STATION);

}
