#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
#include <uart.hh>
#include <stdexcept>

UART::UART(const char* port, const Barude barude):
    fd(open(port, O_RDWR | O_NOCTTY)),
	runFlag(true),
	th(&UART::run, this)
		/* ttyS7 is the FT232 based USB2SERIAL Converter   */
		/* O_RDWR   - Read/Write access to serial port       */
		/* O_NOCTTY - No terminal will control the process   */
		/* Open in blocking mode,read will wait              */
	{
    	if(fd == -1)						/* Error Checking */
			throw std::invalid_argument("UART instance couldn't be created. Cannot create access to serial port '" + std::string(port) + "'");
		/*---------- Setting the Attributes of the serial port using termios structure --------- */
		struct termios SerialPortSettings;	/* Create the structure                          */
		tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */
		/* Setting the Baud rate */
		cfsetispeed(&SerialPortSettings, barude); /* Set Read  Speed as 9600                       */
		cfsetospeed(&SerialPortSettings, barude); /* Set Write Speed as 9600                       */
		/* 8N1 Mode */
		SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
		SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
		SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
		SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
		SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
		SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
		SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
		SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
		SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
		/* Setting Time outs */
		SerialPortSettings.c_cc[VMIN] = 10; /* Read at least 10 characters */
		SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */
		if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
			throw std::invalid_argument("Cannot set atributes of serial port.");
		/*------------------------------- Read data from serial port -----------------------------*/
		tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */
		th.detach();
	}

UART::~UART(){
	runFlag = false;
	if(th.joinable())
		th.join();
	close(fd); /* Close the serial port */
}

unsigned int UART::size(){
	return buffor.size();
}

void UART::clear(){
	tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */
	while(!buffor.empty())
		buffor.pop();
}

bool UART::empty(){
	return buffor.empty();
}

void UART::run(){
	char data = 0;
	int bytes_read = 0; /* Read the data                   */
	while(runFlag){
		bytes_read = read(fd, &data, 1); /* Read the data                   */
		if(bytes_read != 1)
			throw std::invalid_argument("reading char from UART return size = " + std::to_string(bytes_read));
		buffor.push(uint8_t(data));
	}
}

uint8_t UART::getChar(){
	if(buffor.empty())
		throw std::invalid_argument("Trying to get data from empty buffor.");
    char toReturn = buffor.front();
	buffor.pop();
    return toReturn;
}
