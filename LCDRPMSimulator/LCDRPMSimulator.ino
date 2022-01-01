/*
 * LCD RPM Simulator (aka MBEDuino)
 * v1.0
 * Alex Judd (alex@skywire.co.uk)
 * Open Source license - not for commercial distribution without prior permission
 */

#include <LiquidCrystal.h>
#include <DFR_Key.h>
#include <SoftwareSerial.h>

//Whether we are in LIVE or SIMULATION mode
int live = 0;

//LCD setup

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

// Sain supplied keypad driver.

static DFR_Key keypad;

int currentKey = keypad.getKey();
int previousKey = SAMPLE_WAIT;

// Multi-screen setup

int activeScreen = 1;

//LCD Volume bar (horizontal)

byte level0[8] = { 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000};
byte level1[8] = { 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000};
byte level2[8] = { 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100};
byte level3[8] = { 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110};
byte level4[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

//LCD Volume bar (vertical)
//Reference: Arduino LCD Spectrum Analyzer - http://tronixstuff.com/2013/01/31/tutorial-arduino-and-the-msgeq7-spectrum-analyzer/

byte level5[8] = { 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };
byte level6[8] = { 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };
byte level7[8] = { 0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };
byte level8[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000 };
byte level9[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000 };
byte levelA[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b00000 };
byte levelB[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000 };

//LCD Volume bar (horizontal split)
//bottom off
byte levelC[8] = { 0b10000, 0b10000, 0b10000, 0b10000, 0b00000, 0b00000, 0b00000, 0b00000};
byte levelD[8] = { 0b11000, 0b11000, 0b11000, 0b11000, 0b00000, 0b00000, 0b00000, 0b00000};
byte levelE[8] = { 0b11100, 0b11100, 0b11100, 0b11100, 0b00000, 0b00000, 0b00000, 0b00000};
byte levelF[8] = { 0b11110, 0b11110, 0b11110, 0b11110, 0b00000, 0b00000, 0b00000, 0b00000};
byte levelG[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000};
//top off
byte levelH[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b10000, 0b10000, 0b10000, 0b10000};
byte levelI[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b11000, 0b11000, 0b11000, 0b11000};
byte levelJ[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b11100, 0b11100, 0b11100, 0b11100};
byte levelK[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b11110, 0b11110, 0b11110, 0b11110};
byte levelL[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111};

//MBE ECU parameters

/*
 * Engine Speed
 * 
 * Scaling = 16bit
 * Min = 0
 * Max = 12000
 * Units = rpm
 * Resolution = 3.90625
 * Min = 0000
 * Max = 0C00
 */

//const char* engineSpeedCommand[] = {0xC3,0x7C,0x00};
int engineSpeed = 0;

/*
 * Ignition
 * 
 * Scaling = 8bit
 * Min = 0
 * Max = 60
 * Units = degs
 * Resolution = c 0.234
 * Min = 0000
 * Max = 0100
 */

const char ignitionCommand[] = {0x87,0x00};
int ignition = 0;

const char throttle1Command[] = {0x9C};
int throttle1 = 0;

const char throttle2Command[] = {0x94};
int throttle2 = 0;

const char waterTempCommand[] = {0xC0};
int watertemp = 0;

char line1[15];
char line2[15];

int characterwidth = 5;
int charactermax = 16;

//ECU setup

SoftwareSerial mbeECU(3,2);

const char GET_BATTERY[1] = {0xA7};
const char GET_MULTIPLE[4] = {0xC0, 0x9C, 0x94, 0xA7};

char response[32];

// Simluation setup

int rpmPotPin = A3;    // select the input pin for the potentiometer
int tp1PotPin = A4;
int tp2PotPin = A5;
int powerPin = 20;
int groundPin =  21;
int ledPin = 13;   // select the pin for the LED
int val = 0;       // variable to store the value coming from the sensor

int counter = 0;
int refresh = 100000;

int rpm = 0;
int minrpm = 0;
int maxrpm = 7500;

int tp1 = 0;
int tp1scaled = 0;
int mintp1 = -12;
int maxtp1 = 93;
int tp2 = 0;
int tp2scaled = 0;
int mintp2 = -12;
int maxtp2 = 93;

int ad1 = 0;
int minad1 = -100;
int maxad1 = 100;
int ad2 = 0;
int minad2 = -100;
int maxad2 = 100;

float tpsa = 0.0;
float mintpsa = 0.0;
float maxtpsa = 180.0;

int band = 0;

void setup()
{

  Serial.begin(115200);
  
   // setup LCD and custom characters
  lcd.clear();
  lcd.begin(16, 2);
  lcd.createChar(0,level0);
  lcd.createChar(1,level1);
  lcd.createChar(2,level2);
  lcd.createChar(3,level3);
  lcd.createChar(4,level4);

  // setup keypad
  keypad.setRate(10);

  //Test setup
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  pinMode(powerPin, OUTPUT);
  pinMode(groundPin, OUTPUT);
  pinMode(rpmPotPin, INPUT);
  pinMode(tp1PotPin, INPUT);
  pinMode(tp2PotPin, INPUT);
  digitalWrite(powerPin, HIGH);
  digitalWrite(groundPin, LOW);
  
  //Make sure the ECU is plugged into pins 2 and 3 for Rx/Tx
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
  Serial.println("Initialising:"); //debug
  Serial.println("Response:");
  Serial.println(response[0]);
  //mbeECU.write((uint8_t)GET_BATTERY);
  Serial.println(mbeECU.read(), HEX);

  lcd.setCursor(0, 0);
  lcd.println("Initialisation");
  lcd.setCursor(0, 1);
  lcd.println("Complete");

  Serial.println("Should be here");
  
}

/*
void loop() { // run over and over
  mbeECU.write((uint8_t)GET_BATTERY);
  delay(1000);
  Serial.println(mbeECU.read());
}
*/

int getRPM() {
    if (rpm < maxrpm) {
      rpm++;
    }
    if (rpm == maxrpm) { rpm = minrpm; }
    return rpm;
}

void screenOne(int rpm, int tp1, int tp2) {
  String rpmString = String(rpm, DEC);
  if (rpmString.length() < 4) {
    rpmString = "0" + rpmString;
  }
  lcd.setCursor(0, 0);
  lcd.println(rpmString + "RPM " + String(tp1, DEC) + "o 2.3m");
  lcd.setCursor(0, 1);
  lcd.println("90oC   " + String(tp2, DEC) + "o 1.7ms");
}

//screenTwo is a rpm only left to right graph

void screenTwo(int rpm) {
  int remainder = rpm;
  Serial.println("remainder = ");
  Serial.println((int)remainder);
  int band = 0;
  lcd.setCursor(0, 0);
  while (remainder > (maxrpm/charactermax)) {
    lcd.setCursor(band, 0);
    lcd.write(byte(4));
    lcd.setCursor(band, 1);
    lcd.write(byte(4));
    band++;
    remainder = remainder - (maxrpm/(charactermax+1));
    Serial.println((int)remainder);
  }
  lcd.setCursor(band, 0);
  lcd.write(byte(remainder/(maxrpm/charactermax/characterwidth)));
  lcd.setCursor(band, 1);
  lcd.write(byte(remainder/(maxrpm/charactermax/characterwidth)));
  band++;
  while (band <= charactermax) {
    lcd.setCursor(band, 0);
    lcd.write(" ");
    lcd.setCursor(band, 1);
    lcd.write(" ");
    band++;
  }
}

//screenThree is dual throttle 1 and throttle 2 left to right graph

void screenThree(int tp1, int tp2) {
  int band = 0;
  int maxrange1 = maxtp1 - mintp1;
  int maxrange2 = maxtp2 - mintp2;
  int maxbars = charactermax * characterwidth;
  int tp1bars = int(((float)tp1 / 1024) * maxbars);
  int tp2bars = int(((float)tp2 / 1024) * maxbars);
  //Serial.println("tp1bars = " + String(tp1bars, DEC));
  
  lcd.setCursor(0, 0);
  if (tp1bars > 0) {
    while (tp1bars > characterwidth) {
      lcd.setCursor(band,0);
      lcd.write(byte(4));
      tp1bars = tp1bars - characterwidth;
      band++;
    }
    lcd.setCursor(band, 0);
    lcd.write(byte(tp1bars-1));
    band++;
  }
  while (band <= charactermax) {
    lcd.setCursor(band, 0);
    lcd.write(" ");
    band++;
  }
  
  band = 0;
  lcd.setCursor(0, 1);
  if (tp2bars > 0) {
    while (tp2bars > characterwidth) {
      lcd.setCursor(band,1);
      lcd.write(byte(4));
      tp2bars = tp2bars - characterwidth;
      band++;
    }
    lcd.setCursor(band, 1);
    lcd.write(byte(tp2bars-1));
    band++;
  }
  while (band <= charactermax) {
    lcd.setCursor(band, 1);
    lcd.write(" ");
    band++;
  }
  
}

void loop() 
{

  int currentKey = keypad.getKey();
  
  switch (currentKey)
  {
      case NO_KEY : {
        break;
      }
      case LEFT_KEY :
      {
          activeScreen--;
          break;
      }
      case RIGHT_KEY :
      {
          activeScreen++;
          break;
      }
      case UP_KEY     :
      {
          activeScreen++;
          break;
      }
      case DOWN_KEY   :
      {
          activeScreen--;
          break;
      }
      case SELECT_KEY :
      {
      }
      default : {
        break;
      };
  }

  if (live) {
    rpm = analogRead(rpmPotPin);
    rpm = (int)(rpm*((maxrpm-minrpm)/1000))/1.024;
  } else {
    rpm = getRPM();
  }
  tp1 = analogRead(tp1PotPin);
  tp2 = analogRead(tp2PotPin);

  switch (activeScreen) {
    case 1:
      tp1 = (int)(tp1*((maxtp1-mintp1)/100))/10.24;
      tp2 = (int)(tp2*((maxtp2-mintp2)/100))/10.24;
      screenOne(rpm, tp1, tp2);
      break;
    case 2:
      //Serial.println(rpm);
      screenTwo(rpm);
      break;
    case 3:
      screenThree(tp1, tp2);
      break;
    case 4:
      break;
    default:
      break;
  }

  if (previousKey != currentKey) {
    previousKey = currentKey;  
  }

}

void testScreenTwo() {
    /*
  screenTwo(1000);
  delay(1000);
  screenTwo(2000);
  delay(1000);
  screenTwo(3000);
  delay(1000);
  screenTwo(4000);
  delay(1000);
  screenTwo(5000);
  delay(1000);
  screenTwo(6000);
  delay(1000);
  screenTwo(5000);
  delay(1000);
  screenTwo(4000);
  delay(1000);;
  screenTwo(3000);
  delay(1000);
  screenTwo(2000);
  delay(1000);
  screenTwo(1000);
  delay(1000);
  screenTwo(0);
  */
}

