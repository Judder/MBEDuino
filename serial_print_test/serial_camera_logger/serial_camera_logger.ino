#include <SoftwareSerial.h>

#include <SD.h>

/*
JPEG Camera Example Sketch
The sketch will take a picture on the JPEG Serial Camera and store the jpeg to an SD card
on an SD Shield
Written by Ryan Owens
SparkFun Electronics

Hardware Notes:
This sketch assumes the arduino has the microSD shield from SparkFun attached.
The camera Rx/Tx should be attached to pins 2 and 3.
IMPORTANT: The JPEG camera requires a TTL level shifter between the camera output
and the arduino. Bypassing this may damage the Arduino pins.
*/

//This example requires the MemoryCard, SdFat, JPEGCamera and NewSoftSerial libraries
/* Arduino JPeg Camera Library
 * Copyright 2010 SparkFun Electronics
 * Written by Ryan Owens
*/


class JPEGCamera
{
	public:
		JPEGCamera();
		void begin(void);
		int reset(char * response);
		int getSize(char * response, int * size);
		int takePicture(char * response);
		int stopPictures(char * response);
		int readData(char * response, int address);
		
	private:
		int sendCommand(const char * command, char * response, int length);
};

//Commands for the LinkSprite Serial JPEG Camera
const char GET_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
const char RESET_CAMERA[4] = {0x56, 0x00, 0x26, 0x00};
const char TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
const char STOP_TAKING_PICS[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
char READ_DATA[8] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00};

//We read data from the camera in chunks, this is the chunk size
const int read_size=32;

//Make sure the camera is plugged into pins 2 and 3 for Rx/Tx
SoftwareSerial cameraPort(8,9);

JPEGCamera::JPEGCamera()
{
}

//Initialize the serial connection
void JPEGCamera::begin(void)
{
	//Camera baud rate is 38400
	cameraPort.begin(38400);
}

//Get the size of the image currently stored in the camera
//pre: response is an empty string. size is a pointer to an integer
//post: response string contains the entire camera response to the GET_SIZE command
//		size is set to the size of the image
//return: number of characters in the response string
//usage: length = camera.getSize(response, &img_size);
int JPEGCamera::getSize(char * response, int * size)
{
	int count=0;
	//Send the GET_SIZE command string to the camera
	count = sendCommand(GET_SIZE, response, 5);
	//Read 4 characters from the camera and add them to the response string
	for(int i=0; i<4; i++)
	{
		while(!cameraPort.available());
		response[count+i]=cameraPort.read();
	}
	//Set the number of characters to return
	count+=4;
	//The size is in the last 2 characters of the response.
	//Parse them and convert the characters to an integer
    *size = response[count-2]*256;
    *size += (int)response[count-1] & 0x00FF;	
	//Send the number of characters in the response back to the calling function
	return count;
}

//Reset the camera
//pre: response is an empty string
//post: response contains the cameras response to the reset command
//returns: number of characters in the response
//Usage: camera.reset(response);
int JPEGCamera::reset(char * response)
{
	return sendCommand(RESET_CAMERA, response, 4);
}

//Take a new picture
//pre: response is an empty string
//post: response contains the cameras response to the TAKE_PICTURE command
//returns: number of characters in the response
//Usage: camera.takePicture(response);
int JPEGCamera::takePicture(char * response)
{
	return sendCommand(TAKE_PICTURE, response, 5);
}

//Stop taking pictures
//pre: response is an empty string
//post: response contains the cameras response to the STOP_TAKING_PICS command
//returns: number of characters in the response
//Usage: camera.stopPictures(response);
int JPEGCamera::stopPictures(char * response)
{
	return sendCommand(STOP_TAKING_PICS, response, 5);
}

//Get the read_size bytes picture data from the camera
//pre: response is an empty string, address is the address of data to grab
//post: response contains the cameras response to the readData command, but the response header is parsed out.
//returns: number of characters in the response
//Usage: camera.readData(response, cur_address);
//NOTE: This function must be called repeatedly until all of the data is read
//See Sample program on how to get the entire image.
int JPEGCamera::readData(char * response, int address)
{
	int count=0;

	//Flush out any data currently in the serial buffer
	cameraPort.flush();
	
	//Send the command to get read_size bytes of data from the current address
	for(int i=0; i<8; i++)cameraPort.write(READ_DATA[i]);
	cameraPort.write(address>>8);
	cameraPort.write(address);
	cameraPort.write((uint8_t)0x00);
	cameraPort.write((uint8_t)0x00);
	cameraPort.write((uint8_t)(read_size>>8));
	cameraPort.write(read_size);
	cameraPort.write((uint8_t)0x00);
	cameraPort.write((uint8_t)0x0A);	
	
	//Print the data to the serial port. Used for debugging.
	/*
	for(int i=0; i<8; i++)Serial.print(READ_DATA[i]);
	Serial.print(address>>8rt.write);
	Serial.print(addressrt.write);
	Serial.print(0x00rt.write);
	Serial.print(0x00rt.write);
	Serial.print(read_size>>8rt.write);
	Serial.print(read_sizert.write);
	Serial.print(0x00rt.write);
	Serial.print(0x0Art.write);	
	Serial.println();
	*/

	//Read the response header.
	for(int i=0; i<5; i++){
		while(!cameraPort.available());
		cameraPort.read();
	}
	
	//Now read the actual data and add it to the response string.
	count=0;
	while(count < read_size)
	{
		while(!cameraPort.available());
		*response++=cameraPort.read();
		count+=1;
	}
	
	//Return the number of characters in the response.
	return count;
}

//Send a command to the camera
//pre: cameraPort is a serial connection to the camera set to 3800 baud
//     command is a string with the command to be sent to the camera
//     response is an empty string
//	   length is the number of characters in the command
//post: response contains the camera response to the command
//return: the number of characters in the response string
//usage: None (private function)
int JPEGCamera::sendCommand(const char * command, char * response, int length)
{
	char c=0;
	int count=0;
	//Clear any data currently in the serial buffer
	cameraPort.flush();
	//Send each character in the command string to the camera through the camera serial port
	for(int i=0; i<length; i++){
		cameraPort.write(*command++);
	}
	//Get the response from the camera and add it to the response string.
	for(int i=0; i<length; i++)
	{
		while(!cameraPort.available());
		*response++=cameraPort.read();	
		count+=1;
	}
	
	//return the number of characters in the response string
	return count;
}

 

//Create an instance of the camera
JPEGCamera camera;

//Create a character array to store the cameras response to commands
char response[32];
//Count is used to store the number of characters in the response string.
unsigned int count=0;
//Size will be set to the size of the jpeg image.
int size=0;
//This will keep track of the data address being read from the camera
int address=0;
//eof is a flag for the sketch to determine when the end of a file is detected
//while reading the file data from the camera.
int eof=0;
File myFile;

void setup()
{
    pinMode(10, OUTPUT);      // set the SS pin as an output (necessary!)
    digitalWrite(10, HIGH);   // but turn off the W5100 chip!
//Setup the camera, serial port and memory card
    camera.begin();
    Serial.begin(9600);
    Serial.println("Starting");
    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("Starting 1");
    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
         Serial.println("test.txt:");
    }
    //Reset the camera
    Serial.println("Starting 2");
    count=camera.reset(response);
    Serial.write(response);
     delay(5000);
    Serial.println("Starting 3");
    //Take a picture
    count=camera.takePicture(response);
    //Print the response to the 'TAKE_PICTURE' command.
    Serial.write(response);
    Serial.write(count);
    Serial.println();

    //Get the size of the picture
    count = camera.getSize(response, &size);
    //Print the size
    Serial.print("Size: ");
    Serial.println(size);

    //Create a file called 'test.txt' on the SD card.
    //NOTE: The memoryCard libary can only create text files.
    //The file has to be renamed tjpg when copied to a computer..jpg

    
    //Starting at address 0, keep reading data until we've read 'size' data.
    while(address < size)
    {
        //Read the data starting at the current address.
        count=camera.readData(response, address);
        //Store all of the data that we read to the SD card
        for(int i=0; i<count; i++){
            //Check the response for the eof indicator (0xFF, 0xD9). If we find it, set the eof flag
            if((response[i] == (char)0xD9) && (response[i-1]==(char)0xFF))eof=1;
            //Save the data to the SD card
            myFile.write(response[i]);
            //If we found the eof character, get out of this loop and stop reading data
            if(eof==1)break;
        }
        //Increment the current address by the number of bytes we read
        address+=count;
        //Make sure we stop reading data if the eof flag is set.
        if(eof==1)break;
    }
    //Close the file
     myFile.close();
     Serial.print("Done.");
}

void loop()
{

}
