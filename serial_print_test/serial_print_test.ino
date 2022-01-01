#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// LiquidCrystal Print
// by Parzivail

const char GET_BATTERY[1] = {0xA7};
const char GET_MULTIPLE[4] = {0xC0, 0x9C, 0x94, 0xA7};

char response[32];

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

SoftwareSerial mbeECU(3,2);

void setup()
{// put your setup code here, to run once:
  lcd.clear();
  lcd.begin(16, 2);

  //Make sure the camera is plugged into pins 2 and 3 for Rx/Tx
  mbeECU.begin(4800);
  mbeECU.setTimeout(1000);
  mbeECU.flush();
  //Serial.println("Writing:");
  //mbeECU.write((uint8_t)GET_BATTERY);
  mbeECU.write("\r");
  int i = 0;
  if (mbeECU.available()) {
    response[i] = mbeECU.peek();
    i++;
  }
  Serial.begin(115200);
  Serial.println("Initialising:"); //debug
  Serial.println("Response:");
  Serial.println(response[0]);
  mbeECU.write((uint8_t)GET_BATTERY);
  Serial.println(mbeECU.read(), HEX);
}

int counter = 0;
int refresh = 100000;

int rpm = 800;
int minrpm = 800;
int maxrpm = 6000;

float tpsa = 0.0;
float mintpsa = 0.0;
float maxtpsa = 180.0;

/*
void loop() { // run over and over
  mbeECU.write((uint8_t)GET_BATTERY);
  delay(1000);
  Serial.println(mbeECU.read());
}
*/

void loop() 
{
  if (counter == refresh) {
    if (rpm < maxrpm) {
      rpm++;
    }
    if (rpm == maxrpm) { rpm = minrpm; }
    
    if (tpsa < maxtpsa) {
      tpsa += 0.1;
    }
    if (tpsa >= maxtpsa) { tpsa = mintpsa; }
  
    lcd.setCursor(0, 0);
    lcd.println(String(rpm, DEC) + "RPM " + String(tpsa, 1) + "o 2.3m");
    lcd.setCursor(0, 1);
    lcd.println("90oC   24.8o 1.7ms");
    counter = 0;
  }
  counter++;
}

