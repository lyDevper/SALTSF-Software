// SALTSH Innovation Project
#include "LiquidCrystal_I2C.h"
#include "HX711.h"
#include <Servo.h>

#define DEBUG_HX711

// calibration parameter from calibrating code with known values
#define CALIBRATION_FACTOR -221326.0
#define offs_weight 0.00;

// Create the lcd object address 0x3F and 16 columns x 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// data pin and clock pin
byte pinData = 3;
byte pinClk = 2;
// define HX711
HX711 scale;

Servo servo1;
Servo servo2;

#define dcmotor_pin 11

void setup() {
  dcmotorOff();

  servo1.attach(10);
  servo2.attach(9);
  gripperOpen();

  pinMode(dcmotor_pin, OUTPUT);

  //----------------------------------------------------------------------------//
  lcd.init();
  lcd.backlight();
  lcd.print("WEIGHT SCALE");

#ifdef DEBUG_HX711
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("[HX7] Sensor start HX711");
#endif

  //Initializing sensor
  scale.begin(pinData, pinClk);
  // apply the calibration value
  scale.set_scale(CALIBRATION_FACTOR);
  // Initialize the tare
  //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale.tare();

  //---------------------------- init process-----------------------------------//
  delay(1500);
  dcmotorOff();
  gripperOpen();
  delay(3200);
  gripperClose();
  delay(1200);
  dcmotorOn();
}

float weight;
float timeCount = 0;
void loop() {
  weight = scale.get_units();
  displayWeight(weight);

  if (weight > 2.00) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bucket Full!");
    lcd.setCursor(0, 1);
    lcd.print("Remove salt");
    //stop dcmotor
    dcmotorOff();

    delay(5000);
    lcd.init();
    lcd.backlight();
    lcd.print("WEIGHT SCALE");
    //exit(0);
  }

  if (timeCount > 18000) {
    dcmotorOff();
  }

  delay(100);
  timeCount += 100;
}

void displayWeight(float weight) {
#ifdef DEBUG_HX711
  Serial.print("[HX7] Reading: ");
  Serial.print(scale.get_units() * 1000, 0);
  Serial.print(" gs");
  Serial.println();
#endif
  lcd.setCursor(0, 1);
  lcd.print("Reading:");
  lcd.print(getWeight() * 1000, 0);
  lcd.print(" gs");
}

float getWeight() {
  return scale.get_units() - offs_weight;
}

void gripperOpen() {
  servo1.write(25);
  //delay(2);
  servo2.write(50);
  delay(150);
}

void gripperClose() {
  servo1.write(90);
  //delay(2);
  servo2.write(0);
  delay(150);
}

// acive low relay
void dcmotorOn() {
  digitalWrite(dcmotor_pin, LOW);
}

void dcmotorOff() {
  digitalWrite(dcmotor_pin, HIGH);
}
