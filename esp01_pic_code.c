#include <p18f45k22.h>
#include <usart.h>

void uartInit()
{
	{OpenUSART1( USART_TX_INT_OFF&USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT   &USART_CONT_RX&USART_BRGH_HIGH,25);
}

void UART_Send_String(const char* str) {
    while(*str != '\0') {
        putc1USART(*str);
        str++;
    }
}

void main() {
    UART_Init(); // Initialize UART
    UART_Send_String("AT\r\n"); // Send AT command
    __delay_ms(1000); // Wait for response
    UART_Send_String("AT+CIPMUX=1\r\n"); // Set multiple connections
    __delay_ms(1000);
    UART_Send_String("AT+CIPSERVER=1,80\r\n"); // Start server on port 80
    __delay_ms(1000);
    while(1) {
        // Your code to handle communication
    }
}
