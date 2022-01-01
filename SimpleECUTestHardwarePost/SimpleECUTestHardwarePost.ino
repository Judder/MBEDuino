#include <SoftwareSerial.h>

int RX_PIN = 10;
int TX_PIN = 11;
SoftwareSerial mbeECU(RX_PIN, TX_PIN);

const char GET_BATTERY[1] = {0xA7};
byte RETURN = "\r";
byte BATTERY = '\xA7';
byte TPS1 = '\x9C';
byte TPS2 = '\x94';

String serialData = "";   // for incoming serial data
int len = 0;
int out = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("MBE Setup:");
  mbeECU.begin(4800);
  len = mbeECU.available();
  
  // continually write \r until we get a response
  while (len == 0) {
    len = mbeECU.available();
    Serial.println(len);
    if (len == 0) {
      //prompt the ECU for an answer
      Serial.println("Asking ECU");
      mbeECU.write('\r');
    }
    //wait 1/10th of a second
    delay(1000);
  }

  Serial.print("Serial Number of ECU: ");
  
  // now read response until we get a new line character
  while (mbeECU.available()) {
    int inChar = mbeECU.read();
    if (inChar != '\n') {
      serialData += (char)inChar;
    } else {
      int dataSize = serialData.length();
      Serial.print(serialData);
      Serial.print(" length: ");
      Serial.print(dataSize);
      Serial.print("\n");
      delay(3000);
      serialData = "";
    }
  }
}

void loop() {
  len = mbeECU.available();
  
  // write next instruction
  mbeECU.write(TPS1);
  
  // wait until we get a response
  while (len == 0) {
    len = mbeECU.available();
    //wait 1/10th of a second
    delay(100);
  }

  // now read response until we get a new line character
  while (mbeECU.available()) {
    int inChar = mbeECU.read();
    if (inChar != '\n') {
      serialData += (char)inChar;
    } else {
      int dataSize = serialData.length();
      Serial.print(serialData);
      Serial.print(" length: ");
      Serial.print(dataSize);
      Serial.print("\n");
      delay(3000);
      serialData = "";
    }
  }
  serialData = "";

  //out = mbeECU.read(len);
  //while (len > 0) {
  //  Serial.write('LOOK->%s', out[len-1]);
  //}
}
