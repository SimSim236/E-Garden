#include <LiquidCrystal.h>
#include <DHT11.h>
#include <Servo.h>
#include <SoftwareSerial.h>

String msg;
SoftwareSerial hc05(7, 6);
char Incoming_value = 0;

Servo servo_13;
Servo servo_8;

DHT11 dht11(12);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int fan = 2;
const int lamp = 11;
const int amp = 3;

int screen_display = 1;

void setup()
{
    lcd.begin(16, 2);
    pinMode(fan, OUTPUT);
    pinMode(lamp, OUTPUT);
    pinMode(amp, OUTPUT);

    pinMode(7, INPUT);
    pinMode(6, OUTPUT);
    hc05.begin(9600);

    servo_13.attach(13);
    servo_8.attach(8);

    digitalWrite(fan, LOW);
    digitalWrite(lamp, LOW);
    digitalWrite(amp, LOW);

    Serial.begin(9600);
}

void loop()
{
    int temperature = dht11.readTemperature();

    if (temperature > 35 && temperature < 50)
    {
        digitalWrite(fan, LOW);
        digitalWrite(lamp, LOW);
    }
    else if (temperature < 10 && temperature > 0)
    {
        digitalWrite(fan, LOW);
        digitalWrite(lamp, HIGH);
    }
    else
    {
        digitalWrite(fan, LOW);
        digitalWrite(lamp, LOW);
    }

    int valS = analogRead(A4);
    float humidityS = (valS * 10 / 876) * 10;

    if (humidityS < 50)
    {
        servo_8.write(95);
    }
    else
    {
        servo_8.write(1500);
    }

    int humidityA = dht11.readHumidity();

    if (humidityA > 70)
    {
        servo_13.write(180);
    }
    else
    {
        servo_13.write(0);
    }

    int Lux = analogRead(A5);
    int valLux = (Lux * 0.8) - 9.6;

    if (valLux < 50)
    {
        digitalWrite(amp, HIGH);
    }
    else
    {
        digitalWrite(amp, LOW);
    }

    if (hc05.available() > 0)
    {
        hc05.print("Temperature: " + temperature + " °C");
        hc05.print("\n");

        hc05.print("Humidity Soil: " + humidityS + " %");
        hc05.print("\n");

        hc05.print("Humidity Air: " + humidityA + " %");
        hc05.print("\n");

        hc05.print("Luminosity: " + valLux + " Lux");
        hc05.print("\n");
        hc05.print("\n");

        hc05.print("Indications: ");
        hc05.print("\n");

        if (temperature > 35)
        {
            hc05.print("Température élevée ⚠️");
        }
        else if (temperature < 10)
        {
            hc05.print("Température basse ⚠️");
        }
        else if (humidityS < 50)
        {
            hc05.print("Humidité du sol basse ⚠️");
        }
        else if (humidityA > 70)
        {
            hc05.print("Humidité de l'air élevée ⚠️");
        }
        else if (valLux < 50)
        {
            hc05.print("Luminosité basse ⚠️");
        }
        else
        {
            hc05.print("Conditions optimales 😁");
        }
    }

    if (analogRead(A0) > 400 && screen_display == 1)
    {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Temp: ");
        lcd.setCursor(6, 0);
        lcd.print(temperature);
        lcd.print(" C");

        lcd.setCursor(0, 1);
        lcd.print("Fan= ");
        lcd.setCursor(8, 1);
        lcd.print("---");
        lcd.setCursor(9, 1);
        lcd.print("Hot= ");

        if (temperature > 35 && temperature < 50)
        {
            lcd.setCursor(5, 1);
            lcd.print("ON ");
            lcd.setCursor(13, 1);
            lcd.print("OFF ");
        }
        else if (temperature < 10 && temperature > 0)
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
    if (analogRead(A0) < 405 && screen_display == 2)
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
            lcd.print(" HIGH");
        }
        else
        {
            lcd.setCursor(5, 1);
            lcd.print(" NICE");
        }
        delay(2000);
        screen_display = 3;
    }
    if (analogRead(A0) < 405 && screen_display == 3)
    {
        if (humidityA > 0)
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
    }
    if (analogRead(A0) < 405 && screen_display == 4)
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
        delay(2500);
        screen_display = 1;
    }
}

// Made by Sim & Lutr4nn - E-Garden 2024
