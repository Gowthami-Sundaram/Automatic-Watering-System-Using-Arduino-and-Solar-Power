#include <EEPROM.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define MoistureSensorPin A0
#define bt_set  A3
#define bt_up   A4
#define bt_down A5
#define motorRelay 9

int moistureValue;
float setL_moisture = 30.0;
float setH_moisture = 70.0;
float moisture = 0;

int Set = 0, flag = 0, flash = 0;

void setup() {
  pinMode(MoistureSensorPin, INPUT);
  pinMode(bt_set,  INPUT_PULLUP);
  pinMode(bt_up,   INPUT_PULLUP);
  pinMode(bt_down, INPUT_PULLUP);
  pinMode(motorRelay, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Welcome To   ");
  lcd.setCursor(0, 1);
  lcd.print("Plant Watering ");

  if (EEPROM.read(0) == 0) {
    EEPROM.put(10, setL_moisture);
    EEPROM.put(15, setH_moisture);
    EEPROM.write(0, 0);
  }

  EEPROM.get(10, setL_moisture);
  EEPROM.get(15, setH_moisture);

  delay(2000);
  lcd.clear();
}

void loop() {
  moistureValue = analogRead(MoistureSensorPin);
  moisture = map(moistureValue, 0, 1023, 0, 100);

  if (moisture < setL_moisture) {
    digitalWrite(motorRelay, HIGH);
  } else if (moisture > setH_moisture) {
    digitalWrite(motorRelay, LOW);
  }

  if (digitalRead(bt_set) == 0) {
    if (flag == 0) {
      flag = 1;
      Set = Set + 1;
      if (Set > 2) {
        Set = 0;
        flash = 0;
      }
      delay(200);
    }
  } else {
    flag = 0;
  }

  if (digitalRead(bt_up) == 0) {
    if (Set == 1) {
      setL_moisture = setL_moisture + 1;
      if (setL_moisture > 100) setL_moisture = 100;
      EEPROM.put(10, setL_moisture);
    }
    if (Set == 2) {
      setH_moisture = setH_moisture + 1;
      if (setH_moisture > 100) setH_moisture = 100;
      EEPROM.put(15, setH_moisture);
    }
    delay(10);
  }

  if (digitalRead(bt_down) == 0) {
    if (Set == 1) {
      setL_moisture = setL_moisture - 1;
      if (setL_moisture < 0) setL_moisture = 0;
      EEPROM.put(10, setL_moisture);
    }
    if (Set == 2) {
      setH_moisture = setH_moisture - 1;
      if (setH_moisture < 0) setH_moisture = 0;
      EEPROM.put(15, setH_moisture);
    }
    delay(10);
  }

  if (Set == 0) {
    lcd.setCursor(0, 0);
    lcd.print(" Moisture: ");
    lcd.print(moisture, 1);
    lcd.print("%  ");

    lcd.setCursor(0, 1);
    lcd.print(" Motor: ");
    if (digitalRead(motorRelay) == HIGH) {
      lcd.print("ON ");
    } else {
      lcd.print("OFF");
    }
    lcd.print("   ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("  Setting Mode  ");
    lcd.setCursor(0, 1);
    lcd.print("L:");
    if (Set == 1 && flash == 1) {
      lcd.print("    ");
    } else {
      lcd.print(setL_moisture, 1);
    }
    lcd.print("%  ");
    lcd.setCursor(9, 1);
    lcd.print("H:");
    if (Set == 2 && flash == 1) {
      lcd.print("    ");
    } else {
      lcd.print(setH_moisture, 1);
    }
    lcd.print("%  ");
  }

  if (Set > 0) {
    flash = !flash;
  }
  delay(1);
}
