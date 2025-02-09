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

#define OFFICE_1 0b001
#define OFFICE_2 0b010
#define OFFICE_3 0b011
#define OFFICE_4 0b100
#define OFFICE_5 0b100
#define OFFICE_6 0b101
#define OFFICE_7 0b110
#define OFFICE_8 0b111

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
//	while(!PIR1bits.RC1IF);
//	PIR1bits.RC1IF=FALSE;
}




void sysInit()
{
	configAsyncUART();
	setOscTo16MHZ();
}


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


void main() 
{
	
	//0A AND 0D ARE \n and \r respectively
	//int dataf=010;

	sysInit();
	sendUART(atGMR);
	Delay10TCYx( 2500 );

	connectTCP(); 

}
