/**
 * Arduino-based temp & humidity controller for my fermentation chamber.
 *
 * See README for hardware & library info
 *
 * @file fermistat.ino
 * @version 1.1.0
 *
 * @author    Cory Hughart
 * @copyright Cory Hughart 2019
 * @license   MIT
 */

#include <EEPROM.h>
#include <CheapLCD.h>
#include <DHT.h> // Requires Adafruit_Sensor: https://github.com/adafruit/Adafruit_Sensor
#include <timer.h>

// DHT settings
#define DHTTYPE DHT11

// Relay on/off states
#define ON 0
#define OFF 1

// Temp & Humidity constants
#define TEMP_MIN   10
#define TEMP_MAX   60
#define TEMP_RANGE 2
#define HUMI_MIN   0
#define HUMI_MAX   99
#define HUMI_RANGE 2

// Memory addresses 
#define MEM_TEMP   0
#define MEM_HUMI   1

// Other constants
#define SENSOR_INTERVAL 2000

// Pin definitions
static int tempSensor = 2;
static int moistureSensor = A1;
static int heatOutput = A2;
static int humidifierOutput = A3;

static CLCD_Button_t lastButton = BTN_NONE;

DHT dht(tempSensor, DHTTYPE);
CheapLCD lcd;
auto timer = timer_create_default();

// Sensor readings
int moistureVal = 0;
int humidityVal = 0;
int tempCVal = 0;
int tempFVal = 0;

// Temp & Humidity control values (ideal for koji)
byte setTemp = 30;
byte setHumidity = 70;

// Controller output states
int heatState = OFF;
int humidifierState = OFF;

void setup() {
  Serial.begin(9600);

  // Prevent accidental trigger on startup (relay on if LOW)
  heatOff();
  humidifierOff();

  // Controller pin modes
  pinMode(heatOutput, OUTPUT);
  pinMode(humidifierOutput, OUTPUT);

  loadSetTemp();
  loadSetHumidity();

  // Initialize sensor
  dht.begin();

  // Initialize LCD
  lcd.begin();
  lcd.clear();

  // Check sensors every 2 seconds
  checkDHT();
  timer.every(SENSOR_INTERVAL, checkDHT);

  displayAll();
}

void loop() {
  checkKeypad();

  timer.tick();
}


//
// LCD Display
//

void displayAll() {
  lcd.clear();

  String tString = "T:";
  String hString = "H:";

  // Temp
  lcd.setCursor(0,0);
  lcd.print(tString);
  displaySetTemp();
  displayMeasuredTemp();
  displayHeatState();

  // Humidity
  lcd.setCursor(0,1);
  lcd.print(hString);
  displaySetHumidity();
  displayMeasuredHumidity();
  displayHumidifierState();
}


//
// Sensors
//

void checkDHT() {
  bool hUpdate = false;
  bool tUpdate = false;

  int m = (int) analogRead(moistureSensor);
  int h = (int) dht.readHumidity();

  // Read temperature as Celsius (the default)
  int t = (int) dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  int f = (int) dht.readTemperature(true);

  if (!isnan(m) && m != moistureVal) {
    moistureVal = m;
    hUpdate = true;
  }
  if (!isnan(h) && h != humidityVal) {
    humidityVal = h;
    hUpdate = true;
  }

  if (!isnan(t) && t != tempCVal) {
    tempCVal = t;
    tUpdate = true;
  }
  if (!isnan(f) && f != tempFVal) {
    tempFVal = f;
    tUpdate = true;
  }

  if (hUpdate) {
    displayMeasuredHumidity();
  }

  if (tUpdate) {
    displayMeasuredTemp();
  }

  int humi_min = setHumidity - HUMI_RANGE;
  int humi_max = setHumidity + HUMI_RANGE;
  if (humidityVal <= humi_min) {
    humidifierOn();
  }
  if (humidityVal >= humi_max) {
    humidifierOff();
  }

  int temp_min = setTemp - TEMP_RANGE;
  int temp_max = setTemp + TEMP_RANGE;
  if (tempCVal <= temp_min) {
    heatOn();
  }
  if (tempCVal >= temp_max) {
    heatOff();
  }
}


//
// Keypad
//

void checkKeypad() {
  CLCD_Button_t newReading = lcd.readButton();

  if (newReading != lastButton) {   // button changed
    lastButton = newReading;

    if (newReading != BTN_NONE) {   // button was pressed
      switch(newReading) {
        case BTN_RIGHT:
          humidityUp();
          break;
        case BTN_UP:
          tempUp();
          break;
        case BTN_DOWN:
          tempDown();
          break;
        case BTN_LEFT:
          humidityDown();
          break;
        case BTN_SELECT:
          // Temp & Humidity settings are only saved on SELECT to reduce writes to EEPROM
          saveSetTemp();
          saveSetHumidity();
          // Refresh display, why not
          displayAll();
          break;
        default:
          break;
      }
    }
    else {
      // button was released
    }
  }
}


//
// Temperature
//

void displayMeasuredTemp() {
  lcd.setCursor(5,0);
  String mt = String("[")+tempCVal+" "+tempFVal+"F]";
  while (mt.length() < 10) {
    mt = mt + " ";
  }
  lcd.print(mt);
}
void displaySetTemp() {
  lcd.setCursor(2,0);
  lcd.print(setTemp);
  if (setTemp < 10) {
    lcd.setCursor(3,0);
    lcd.print(" ");
  }
}
void tempUp() {
  setTemp++;
  if (setTemp > TEMP_MAX) {
    setTemp = TEMP_MAX;
  }
  displaySetTemp();
}
void tempDown() {
  setTemp--;
  if (setTemp < TEMP_MIN) {
    setTemp = TEMP_MIN;
  }
  displaySetTemp();
}
void loadSetTemp() {
  byte m = 0;
  EEPROM.get(MEM_TEMP, m);
  Serial.print("Got saved temp: ");
  Serial.println(m);
  if (m != setTemp && m >= TEMP_MIN && m <= TEMP_MAX) {
    setTemp = m;
    displaySetTemp();
  }
}
void saveSetTemp() {
  EEPROM.update(MEM_TEMP, setTemp);
  Serial.print("Temp setting saved: ");
  Serial.println(setTemp);
}


//
// Humidity
//

void displayMeasuredHumidity() {
  String mh = String("[")+humidityVal+" M:"+moistureVal+"]";
  if (mh.length() >= 8) {
    mh = mh + " ";
  }
  lcd.setCursor(5,1);
  lcd.print(mh);

}
void displaySetHumidity() {
  lcd.setCursor(2,1);
  lcd.print(setHumidity);
  if (setHumidity < 10) {
    lcd.setCursor(3,1);
    lcd.print(" ");
  }
}
void humidityUp() {
  setHumidity++;
  if (setHumidity > HUMI_MAX) {
    setHumidity = HUMI_MAX;
  }
  displaySetHumidity();
}
void humidityDown() {
  setHumidity--;
  if (setHumidity < HUMI_MIN) {
    setHumidity = HUMI_MIN;
  }
  displaySetHumidity();
}
void loadSetHumidity() {
  byte m = 0;
  EEPROM.get(MEM_HUMI, m);
  Serial.print("Got saved humidity: ");
  Serial.println(m);
  if (m != setHumidity && m >= HUMI_MIN && m <= HUMI_MAX) {
    setHumidity = m;
    displaySetHumidity();
  }
}
void saveSetHumidity() {
  EEPROM.update(MEM_HUMI, setHumidity);
  Serial.print("Humidity setting saved: ");
  Serial.println(setHumidity);
}


//
// Controller States
//

void displayHeatState() {
  lcd.setCursor(15,0);
  lcd.print(!heatState);
}
void heatOn() {
  heatState = ON;
  digitalWrite(heatOutput, heatState);
  displayHeatState();
}
void heatOff() {
  heatState = OFF;
  digitalWrite(heatOutput, heatState);
  displayHeatState();
}

void displayHumidifierState() {
  lcd.setCursor(15,1);
  lcd.print(!humidifierState);
}
void humidifierOn() {
  humidifierState = ON;
  digitalWrite(humidifierOutput, humidifierState);
  displayHumidifierState();
}
void humidifierOff() {
  humidifierState = OFF;
  digitalWrite(humidifierOutput, humidifierState);
  displayHumidifierState();
}
