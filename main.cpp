#include "mbed.h"
#include "arm_book_lib.h"

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

AnalogIn potentiometer(A0);
AnalogIn lm35(A1); // 10 mV/\xB0 C
PwmOut buzzer(D9);
DigitalIn mq2(A3);

bool quit = false;

char receivedChar = '\0';

void availableCommands();
void uartTask();
void pcSerialComStringWrite( const char* str );
char pcSerialComCharRead();

int main()
{
    availableCommands();
    while( true ) {
        uartTask();
    }
}

void availableCommands()
{
    pcSerialComStringWrite("Print the readings until 'q' is pressed:\r\n" );

    pcSerialComStringWrite("Press 'g' the reading of the DOUT signal of the ");
    pcSerialComStringWrite("MQ-2 gas sensor \r\n");

    pcSerialComStringWrite("\r\nWARNING: The readings are printed continuously ");
    pcSerialComStringWrite("until 'q' or 'Q' are pressed.\r\n\r\n");
}

void uartTask()
{
    char receivedChar = '\0';
    char str[100] = "";
    receivedChar = pcSerialComCharRead();
    if( receivedChar !=  '\0') {
        switch (receivedChar) {

        case 'g':
        case 'G':
            while( !(receivedChar == 'q' || receivedChar == 'Q') ) {
                if (!mq2) {
                    while (!mq2) {
                        pcSerialComStringWrite("Gas is being detected\r\n");
                        buzzer.period(1.0/100.0);
                        buzzer = 5;
                        delay(1);
                        buzzer.period(1.0/105);
                        buzzer = 5;
                        delay(1);
                        buzzer = 0;
                    }
                } else {
                    pcSerialComStringWrite("Gas is not being detected\r\n");
                }
                delay( 200 );
                receivedChar = pcSerialComCharRead();
            }
            break;

        default:
            availableCommands();
            break;
        }
    }
}

void pcSerialComStringWrite(const char* str)
{
    uartUsb.write(str, strlen(str));
}

char pcSerialComCharRead()
{
    char receivedChar = '\0';
    if(uartUsb.readable()) {
        uartUsb.read(&receivedChar, 1);
    }
    return receivedChar;
}