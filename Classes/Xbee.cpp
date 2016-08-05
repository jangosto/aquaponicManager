#include "Xbee.h"

#include <string.h>

#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART

#include <semaphore.h>


Xbee::Xbee()
{
    uart = openUart();
    initMutex();
    initSemaphore();
}

bool Xbee::initMutex()
{
    bool response = true;

    //UART TX mutex
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        response = false;
        printf("\n[ERROR][Xbee:initMutex] Mutex inicialization failed\n");
    }

    return response;
}

int Xbee::initSemaphore()
{
    bool response = true;

    //UART RX semaphore
    if (sem_init(&semaphore,0,0) != 0)
    {
        response = false;
        printf("\n[ERROR][Xbee::initSemaphore] Semaphore inicialization failed\n");
    }

    return response;
}

string Xbee::read()
{
    sem_wait(&semaphore);
    if (uart != -1) {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];
        int rx_length = read(uart, (void*)rx_buffer, 255);       //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0) {
            //An error occured (will occur if there are no bytes)
        } else if (rx_length == 0) {
            //No data waiting
        } else {
            //Bytes received
            rx_buffer[rx_length] = '\0';
            strcpy(dataRX,rx_buffer);
            printf("\n[INFO][Xbee::read] %i bytes read: %s\n", rx_length, rx_buffer);
        }
    }
    sem_post(&semaphore);
}

int Xbee::send(string data)
{
    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    int response = 2;
    int i;
    int numSendedChars;

    pthread_mutex_lock(&mutex);

    fprintf(stdout,"\n[INFO][Xbee::send] Sending data -%s-...\n",data);

    p_tx_buffer = &tx_buffer[0];

    i=0;
    while (data[i] != '\0') {
        *p_tx_buffer++ = data[i];
        i++;
    }
    
    if (uart != -1) {
        response = 1;
        numSendedChars = write(uart, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));

        if (numSendedChars < 0) {
            printf("\n[ERROR][Xbee::send] UART TX error\n");
            response = 0;
        }
    } else {
        printf("\n[ERROR][Xbee::send] UART TX error because uart is not available\n");
    }
    printf("\n[INFO][Xbee::send] Data sended.\n");

    pthread_mutex_unlock(&mutex);

    return (response);
}

int Xbee::openUart ()
{
    int uart0Handler = -1;
    struct termios uartOptions;

    uart0Handler = open(PATH, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0Handler == -1) {
    {
        printf("\n[ERROR][Xbee::openUart] Error opening UART path.\n");
        return(-1);
    }
    tcgetattr(uart0Handler, &uartOptions);
    uartOptions.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    uartOptions.c_iflag = IGNPAR | ICRNL;
    uartOptions.c_oflag = 0;
    uartOptions.c_lflag = 0;
    tcflush(uart0Handler, TCIFLUSH);
    tcsetattr(uart0Handler, TCSANOW, &uartOptions);

    usleep(10000);

    return(descriptor_uart0);
}

string Xbee::getDataRX()
{
    return dataRX;
}
