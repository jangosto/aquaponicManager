#include "SerialPort.h"

std::mutex SerialPort::rxMutex;
std::mutex SerialPort::txMutex;

SerialPort::SerialPort()
{
    uart = openUart();
}

int SerialPort::read()
{
    int response;

    rxMutex.lock();
    if (uart != -1) {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];
	    usleep(40000);
        int rx_length = ::read(uart, (void*)rx_buffer, 255);       //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0) {
            response = -1;
            //An error occured (will occur if there are no bytes)
        } else if (rx_length == 0) {
            response = 0;
            //No data waiting
        } else {
            //Bytes received
            rx_buffer[rx_length] = '\0';
            std::string tempString(reinterpret_cast< char const* >(rx_buffer));
            dataRX.assign(tempString);
            response = rx_length;
            printf("\n[INFO][SerialPort::read] %i bytes read: %s", rx_length, rx_buffer);
        }
    }
    rxMutex.unlock();

    return response;
}

int SerialPort::send(std::string data)
{
    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    int response = 2;
    int i;
    int numSendedChars;

    txMutex.lock();

    printf("\n[INFO][SerialPort::send] Sending data -%s-...",data.c_str());

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
            printf("\n[ERROR][SerialPort::send] UART TX error");
            response = 0;
        }
    } else {
        printf("\n[ERROR][SerialPort::send] UART TX error because uart is not available\n");
    }
    printf("\n[INFO][SerialPort::send] Data sended.");

    txMutex.unlock();

    return (response);
}

int SerialPort::openUart ()
{
    int uart0Handler = -1;
    struct termios uartOptions;

    uart0Handler = open(SERIAL_PATH, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0Handler == -1) {
        printf("\n[ERROR][SerialPort::openUart] Error opening UART path.");
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

    return(uart0Handler);
}

std::string SerialPort::getDataRX ()
{
    return dataRX;
}

int SerialPort::getUart ()
{
    return uart;
}
