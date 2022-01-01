#include <SoftwareSerial.h>

SoftwareSerial mbeECU(3,2);

const char GET_BATTERY[1] = {0xA7};
byte RETURN = "\r";
byte TEST = '\xA7';
byte TEST2 = '\xFF';

char incomingByte = 'A';   // for incoming serial data
int incomingValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Initialised:");
  mbeECU.begin(4800);
 // mbeECU.setTimeout(1000);  
  //mbeECU.flush();
  //mbeECU.write(RETURN);

}

void loop() {
  // put your main code here, to run repeatedly:
  // send data only when you receive data:
   //mbeECU.flush();
   //mbeECU.write(GET_BATTERY);
   mbeECU.write(TEST);
   mbeECU.write(RETURN);
   mbeECU.flush();
   delay(1000);
   mbeECU.flush();
   incomingByte = mbeECU.read();
   Serial.print("I received: ");
   int incomingValue = incomingByte;
   Serial.println(atoi(incomingByte));
}
