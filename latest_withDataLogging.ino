#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <SD.h>
#include <SPI.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // Change the I2C address if necessary

ThreeWire myWire(7, 6, 8); // data, clock, reset
RtcDS1302<ThreeWire> Rtc(myWire);
File myFile; // for SD card logging

// Pin assignments for sensors
int resPins[] = {A0, A1, A2, A3, A4, A5};
int capPins[] = {A6, A7, A8, A9, A10, A11};
int numSensors = 6;

// Resistive sensor percentage
int resprctg[6];

// Capacitive sensor percentage
int capprctg[6];

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Declare pin for relay
  pinMode(2, OUTPUT); // Relay for resistive sensor
  pinMode(3, OUTPUT); // Relay for capacitive sensor

  Rtc.Begin();
  Serial.begin(9600); // Initialize serial communication
  Serial.println("RTC initialized.");

  // SD Card Initialization
  if (SD.begin()) {
    Serial.println("SD card is ready to use.");
  } else {
    Serial.println("SD card initialization failed");
    return;
  }

  // Create file with current date as filename
  String filename = getCurrentDate() + ".txt";
  myFile = SD.open(filename, FILE_WRITE);

  if (myFile) {
    myFile.close(); // Close the file
  } else {
    Serial.println("Error opening file.");
    return;
  }
}

void loop() {
  // Read sensor data
  readSensorData();

  // Display date and time on LCD
  displayDateTime();

  // Log data every 5 minutes
  if (Rtc.GetDateTime().Second() == 0 && Rtc.GetDateTime().Minute() % 5 == 0) {
    logData();
  }

  // Control pin modes based on sensor values
  controlPinModes();

  delay(1000); // Delay for stability
}

void readSensorData() {
  for (int i = 0; i < numSensors; i++) {
    resprctg[i] = map(analogRead(resPins[i]), 490, 1023, 100, 0);
    capprctg[i] = map(analogRead(capPins[i]), 490, 1023, 100, 0);
  }
}

void displayDateTime() {
  RtcDateTime now = Rtc.GetDateTime();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(now.Day());
  lcd.print("/");
  lcd.print(now.Month());
  lcd.print("/");
  lcd.print(now.Year());

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(now.Hour());
  lcd.print(":");
  lcd.print(now.Minute());
  lcd.print(":");
  lcd.print(now.Second());
}

void logData() {
  String data = getCurrentTime() + ",";

  // Log resistive sensor data
  for (int i = 0; i < numSensors; i++) {
    data += String(resprctg[i]);
    if (i < numSensors - 1) {
      data += ",";
    }
  }

  data += ",";

  // Log capacitive sensor data
  for (int i = 0; i < numSensors; i++) {
    data += String(capprctg[i]);
    if (i < numSensors - 1) {
      data += ",";
    }
  }

  // Open file and append data
  String filename = getCurrentDate() + ".csv";
  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    myFile.println(data);
    myFile.close();
    Serial.println("Data logged.");
  } else {
    Serial.println("Error opening file for logging.");
  }
}

String getCurrentDate() {
  RtcDateTime now = Rtc.GetDateTime();
  String currentDate = String(now.Year()) + "_" + String(now.Month()) + "_" + String(now.Day());
  return currentDate;
}

String getCurrentTime() {
  RtcDateTime now = Rtc.GetDateTime();
  String currentTime = String(now.Hour()) + ":" + String(now.Minute()) + ":" + String(now.Second());
  return currentTime;
}

void controlPinModes() {
  // Control pin modes based on sensor values
  // Resistive sensor
  if (resprctg[0] < 10 && resprctg[1] < 10) {
    digitalWrite(2, LOW); // Pump on
  } else if (resprctg[0] > 80 && resprctg[1] > 80) {
    digitalWrite(2, HIGH); // Pump off
  }

  // Capacitive sensor
  if (capprctg[0] < 10 && capprctg[1] < 10 && capprctg[2] < 10 && capprctg[3] < 10 && capprctg[4] < 10 && capprctg[5] < 10) {
    digitalWrite(3, LOW); // Pump on
  } else if (capprctg[0] > 80 && capprctg[1] > 80 && capprctg[2] > 80 && capprctg[3] > 80 && capprctg[4] > 80 && capprctg[5] > 80) {
    digitalWrite(3, HIGH); // Pump off
  }
}
