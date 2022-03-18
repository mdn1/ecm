#include <unistd.h>		   //Needed for I2C port
#include <fcntl.h>		   //Needed for I2C port
#include <sys/ioctl.h>	   //Needed for I2C port
#include <linux/i2c-dev.h> //Needed for I2C port
#include <stdio.h>		   //Needed for printf
#include <unistd.h>		   //Needed for sleep

int file_i2c;
int length;
int slave_adress = 0x08;
unsigned char buffer[60] = {0};
char filename[20];
int adapter_nr = 1; //get available adapters with the cmd: ->	ls /dev/*i2c*

void init(){
	//----- OPEN THE I2C BUS -----
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);

	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		// ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}

	if (ioctl(file_i2c, I2C_SLAVE, slave_adress) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// ERROR HANDLING; you can check errno to see what went wrong
		return;
	}
}


void read_msg() {
	//----- READ BYTES -----
	// length = 4;			//<<< Number of bytes to read
	// if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	// {
	// 	//ERROR HANDLING: i2c transaction failed
	// 	printf("Failed to read from the i2c bus.\n");
	// }
	// else
	// {
	// 	printf("Data read: %s\n", buffer);
	// }
}


void send_msg() {
	buffer[0] = 0x01;
	buffer[1] = 0x00;
	length = 1;									   //<<< Number of bytes to write

	//Turn on LED on arduino
	if (write(file_i2c, buffer, length) != length) // write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}

	sleep(1); //1s

	//Turn off LED on arduino
	if (write(file_i2c, (unsigned char*) &buffer +1, length) != length) // write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
}

int main(){
	init();
	send_msg();
}


//To combine with the following arduino code:
/*
//*********************************
//Simple I2C communication between Raspberry Pi and Arduino. If the value 0 is typed in on the raspi, the intern LED will turn on.
//Otherwise if one is entered then the intern Arduino LED will turn on.
//*********************************
//Master device = Raspberry Pi
//I2C-> SDA=GPIO2 SCL=GPIO3
//*********************************
//Slave Device= Arduino UNO (running this code)
//I2C-> SDA=A4 SCL=A5
//*********************************

//Include the Wire library for I2C
#include <Wire.h>

// LED on pin 13
//const int ledPin = 13; 

void setup() {
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);
  
  // Call receiveEvent when data received                
  Wire.onReceive(receiveEvent);
  
  // Setup pin 13 as output and turn LED off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

// Function that executes whenever data is received from master
void receiveEvent(int howMany) {
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    digitalWrite(LED_BUILTIN, c);
  }
}
void loop() {
  delay(100);
}
*/