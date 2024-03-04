#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // Change the I2C address if necessary

ThreeWire myWire(7, 6, 8); // data, clock, reset
RtcDS1302<ThreeWire> Rtc(myWire);

// resistive sensor 
const int res0Pin = 0;
const int res1Pin = 0;
const int res2Pin = 0;
const int res3Pin = 0;
const int res4Pin = 0;
const int res5Pin = 0;

// capacitive sensor

const int cap0Pin = 0;
const int cap1Pin = 0;
const int cap2Pin = 0;
const int cap3Pin = 0;
const int cap4Pin = 0;
const int cap5Pin = 0;

// resistive percentage
int resprctg0 = 0;
int resprctg1 = 0;
int resprctg2 = 0;
int resprctg3 = 0;
int resprctg4 = 0;
int resprctg5 = 0;

// capacitive percentage

int capprctg0 = 0;
int capprctg1 = 0;
int capprctg2 = 0;
int capprctg3 = 0;
int capprctg4 = 0;
int capprctg5 = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // declare pin for relay
  // relay for resistive sensor
  pinMode(2, OUTPUT);
  //relay for capacitive sensor
  pinMode(3, OUTPUT);


  Rtc.Begin();
  // Run this code once, comment all codes below 
  // RtcDateTime currentTime = RtcDateTime(__DATE__, __TIME__);
  // Rtc.SetDateTime(currentTime);

  Serial.begin(9600); // Initialize serial communication
  Serial.println("RTC initialized.");
}

void loop() {
    // read all data from resistive sensor
    res0Pin = analogRead(A0);
    res1Pin = analogRead(A1);
    res2Pin = analogRead(A2);
    res3Pin = analogRead(A3);
    res4Pin = analogRead(A4);
    res5Pin = analogRead(A5);

    // read all data from capacitive sensor
    cap0Pin = analogRead(A6);
    cap1Pin = analogRead(A7);
    cap2Pin = analogRead(A8);
    cap3Pin = analogRead(A9);
    cap4Pin = analogRead(A10);
    cap5Pin = analogRead(A11);

    // Map sensor value to percentage for resistive sensor
    resprctg0 = map(res0Pin, 490, 1023, 100, 0); 
    resprctg1 = map(res1Pin, 490, 1023, 100, 0);
    resprctg2 = map(res2Pin, 490, 1023, 100, 0);
    resprctg3 = map(res3Pin, 490, 1023, 100, 0);
    resprctg4 = map(res4Pin, 490, 1023, 100, 0);
    resprctg5 = map(res5Pin, 490, 1023, 100, 0);

    // Map sensor value to percentage for capacitive sensor
    capprctg0 = map(cap0Pin, 490, 1023, 100, 0);
    capprctg1 = map(cap1Pin, 490, 1023, 100, 0);
    capprctg2 = map(cap2Pin, 490, 1023, 100, 0);
    capprctg3 = map(cap3Pin, 490, 1023, 100, 0);
    capprctg4 = map(cap4Pin, 490, 1023, 100, 0);
    capprctg5 = map(cap5Pin, 490, 1023, 100, 0);

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

    // print data from the sensor
    lcd.setCursor(0, 2);
    lcd.print("Sensor0: ");
    lcd.print(res0);
    lcd.print("   ");

    

    // condition if low moisture then trigger relay

    // trigger relay if the water is below 10 percent using resistive sensor
    if (resprctg0 < 10 &&  resprctg1 < 10 resprctg2 < 10 &&  resprctg3 < 10 resprctg4 < 10 &&  resprctg5 < 10) {
        Serial.println(" Pump on");
        digitalWrite(2, LOW);
    }
    // trigger relay off if soil moisture is present
    if (resprctg0 > 80 &&  resprctg1 > 80 resprctg2 > 80 &&  resprctg3 > 80 resprctg4 > 80 &&  resprctg5 > 80) {
        Serial.println(" Pump off");
        digitalWrite(2, HIGH);
    }

    // open relay if soil moisuter is not present using capacitive sensor
    if (capprctg0 < 10 &&  capprctg1 < 10 capprctg2 < 10 &&  capprctg3 < 10 capprctg4 < 10 &&  capprctg5 < 10) {
        Serial.println(" Pump on");
        digitalWrite(3, LOW);
    }
    // trigger relay off if soil moisture is present
    if (capprctg0 > 80 &&  capprctg1 > 80 capprctg2 > 80 &&  capprctg3 > 80 capprctg4 > 80 &&  capprctg5 > 80) {
        Serial.println(" Pump off");
        digitalWrite(3, HIGH);
    }

    delay(1000);
}
