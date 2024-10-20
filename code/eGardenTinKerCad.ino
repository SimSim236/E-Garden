#include <LiquidCrystal.h>
#include <Servo.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo servo_8;
Servo servo_7;

int measure = A0;
const int fan = 13;
const int lamp = 9;
const int amp = 10;
const int buttonPin = 6;

int screen_display = 1;

void setup()
{
    lcd.begin(16, 2);

    servo_8.attach(8);
    servo_8.write(0);

    servo_7.attach(7);
    servo_7.write(1500);

    pinMode(A1, INPUT);
    pinMode(fan, OUTPUT);
    pinMode(lamp, OUTPUT);
    pinMode(amp, OUTPUT);
    pinMode(buttonPin, INPUT);

    digitalWrite(fan, LOW);
    digitalWrite(lamp, LOW);
    digitalWrite(amp, LOW);

    Serial.begin(9600);
}

void loop()
{
    int reading = analogRead(measure);
    float voltage = reading * 5.0 / 1024.0;
    int temperatureC = (voltage - 0.5) * 100;

    if (temperatureC > 35)
    {
        digitalWrite(fan, HIGH);
        digitalWrite(lamp, LOW);
    }
    else if (temperatureC < 10)
    {
        digitalWrite(fan, LOW);
        digitalWrite(lamp, HIGH);
    }
    else
    {
        digitalWrite(fan, LOW);
        digitalWrite(lamp, LOW);
    }

    int valS = analogRead(A1);
    float humidityS = (valS * 10 / 876) * 10;

    if (humidityS < 50)
    {
        servo_7.write(95);
        delay(200);
    }
    else
    {
        servo_7.write(1500);
    }

    int Lux = analogRead(A2);
    int valLux = (Lux * 0.8) - 9.6;

    if (valLux < 50)
    {
        digitalWrite(amp, HIGH);
    }
    else
    {
        digitalWrite(amp, LOW);
    }

    int Gaz = analogRead(A4);
    int TauxGaz = (Gaz * 10 / 876) * 10;

    int valA = analogRead(A3);
    float humidityA = (valA * 10 / 876) * 10;

    if (Gaz > 700)
    {
        servo_8.write(0);
    }
    else if (humidityA > 70)
    {
        servo_8.write(90);
    }
    else
    {
        servo_8.write(0);
    }

    if (screen_display == 1)
    {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Temp: ");
        lcd.setCursor(6, 0);
        lcd.print(temperatureC);
        lcd.print(" C");

        lcd.setCursor(0, 1);
        lcd.print("Fan= ");
        lcd.setCursor(8, 1);
        lcd.print("---");
        lcd.setCursor(9, 1);
        lcd.print("Hot= ");

        if (temperatureC > 35)
        {
            lcd.setCursor(5, 1);
            lcd.print("ON ");
            lcd.setCursor(13, 1);
            lcd.print("OFF ");
        }
        else if (temperatureC < 10)
        {
            lcd.setCursor(5, 1);
            lcd.print("OFF");
            lcd.setCursor(13, 1);
            lcd.print("ON ");
        }
        else
        {
            lcd.setCursor(5, 1);
            lcd.print("OFF");
            lcd.setCursor(13, 1);
            lcd.print("OFF ");
        }
        delay(2000);
        screen_display = 2;
    }
    if (digitalRead(buttonPin) == HIGH && screen_display == 2)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("hum Soil: ");
        lcd.setCursor(9, 0);
        lcd.print(humidityS);
        lcd.print("%");

        if (humidityS < 50)
        {
            lcd.setCursor(5, 1);
            lcd.print(" LOW");
        }
        else
        {
            lcd.setCursor(5, 1);
            lcd.print(" NICE");
        }
        delay(2000);
        screen_display = 3;
    }
    if (digitalRead(buttonPin) == HIGH && screen_display == 3)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("hum Air: ");
        lcd.setCursor(9, 0);
        lcd.print(humidityA);
        lcd.print("%");

        if (humidityA > 70)
        {
            lcd.setCursor(5, 1);
            lcd.print(" HIGH");
        }
        else
        {
            lcd.setCursor(5, 1);
            lcd.print(" NICE");
        }
        delay(2000);
        screen_display = 4;
    }
    if (digitalRead(buttonPin) == HIGH && screen_display == 4)
    {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Lumi: ");
        lcd.setCursor(7, 0);
        lcd.print(valLux);
        lcd.print(" Lux");

        if (valLux < 50)
        {
            lcd.setCursor(5, 1);
            lcd.print(" DOWN");
        }
        else
        {
            lcd.setCursor(5, 1);
            lcd.print(" NICE");
        }
        delay(2000);
        screen_display = 5;
    }
    if (digitalRead(buttonPin) == HIGH && screen_display == 5)
    {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Taux CO2: ");
        lcd.setCursor(11, 0);
        lcd.print(TauxGaz);
        lcd.print("%");

        if (Gaz > 700)
        {
            lcd.setCursor(5, 1);
            lcd.print(" HIGH");
        }
        else
        {
            lcd.setCursor(5, 1);
            lcd.print(" NICE");
        }
        delay(2500);
        screen_display = 1;
    }
}

// Made by Sim & Lutr4nn - E-Garden 2024
