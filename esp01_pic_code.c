/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011Template.c  
	Author:		Name
	Date:		DD/MM/YYYY
	Modified:	Name & date or None
	© Fanshawe College, 2025

	Description: Describe the purpose of the whole program.  What the 
			program does and why. What platform (hardware part number) and / or 
			supplemental documents required for the programís use. 
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

const char atOK[]="AT\n\r";
const char atRST[]="AT+RST\n\r";
const char atCWMODE[]="AT+CWMODE=1\n\r";
const char atCWJAP[]="AT+CWJAP=\"Pixel_8296\",\"nihalbra\"\n\r";
const char atCWJAP2[]="AT+CWJAP?\n\r";
const char atCIPSTART[]="AT+CIPSTART=\"TCP\",\"10.35.64.57\",8080\n\r";
const char atCIPSEND[]="AT+CIPSEND=26\n\r";
const char atCIPCLOSE[]="AT+CIPCLOSE\n\r";
const char atCIPSEND2[]="AT+CIPSEND=2\n\r";
const char atGMR[]="AT+GMR\n\r";


void setOscTo16MHZ()
{
	OSCCON=0x72;
	while(!OSCCONbits.HFIOFS);
}

void configAsyncUART()
{
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
	
	//NOW WHEN WE SEND A BYTE TO TXREG1 THIS WILL TRANSMIT
}

void sendUART(char* data)
{
	while(*data)
	{
		
		while(!PIR1bits.TX1IF);
		TXREG1=*data++;
	}
	
}


void sysInit()
{
	configAsyncUART();
	setOscTo16MHZ();
}


void connectTCP()
{
	

	sendUART(atOK);
	Delay10TCYx( 2500 );
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
