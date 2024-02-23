#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

int soilMoistureValue = 0;
int percentage=0;
int soilMoistureValue2 = 0;
int percentage2=0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM IS ON ");
  lcd.print("");
  delay(3000);
  lcd.clear();
}

void loop() {
lcd.setCursor(0, 0);
  lcd.print("Value:");
  lcd.setCursor(5, 0); // Adjusted position
  lcd.print(percentage);

  // Print the second set of values
  lcd.setCursor(0, 2); // Adjusted row
  lcd.print("Value:");
  lcd.setCursor(5, 2); // Adjusted position
  lcd.print(percentage2);
  
  soilMoistureValue = analogRead(A0);
  soilMoistureValue2 = analogRead(A1);

  Serial.println(percentage);

  percentage = map(soilMoistureValue, 490, 1023, 100, 0);
  percentage2 = map(soilMoistureValue2, 490, 1023, 100, 0);

  if(percentage < 10)  

  {

    Serial.println(" pump on");

    digitalWrite(2, HIGH);
    

  }

  if(percentage > 80 )

  {

    Serial.println("pump off");

    digitalWrite(2,LOW);
   
  }
  if(percentage2 < 10)  

  {

    Serial.println(" pump on");

    
    digitalWrite(3,HIGH);

  }

  if( percentage2 > 80)

  {

    Serial.println("pump off");

    
    digitalWrite(3,LOW);
  }

}
