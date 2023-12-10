#include <SPI.h>
#include <RFID.h>

#include <Adafruit_LiquidCrystal.h>
#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);
String rfidCard;
int seconds = 0;
int BUZZER_PIN = 5;
const int redLEDPin = 8;
const int greenLEDPin = 7;
Adafruit_LiquidCrystal lcd_1(0);
struct IpIdMapping
{
    String ip;
    String idd;
};
IpIdMapping ipIdMappings[] = {
    {"8 220 242 68", "12214760"},
    {"8 23 237 213", "12214135"},

};
void setup()
{
    Serial.begin(9600);
    Serial.println("Starting the RFID Reader...");
    SPI.begin();
    rfid.init();
    lcd_1.begin(16, 2);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(redLEDPin, OUTPUT);
    pinMode(greenLEDPin, OUTPUT);
    lcd_1.print("Program started");
    delay(500);
}

void loop()
{
    if (rfid.isCard())
    {
        if (rfid.readCardSerial())
        {
            rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
            Serial.println(rfidCard);
            delay(300);
            displayIntValues(rfidCard);
        }
        rfid.halt();
    }
}

String getLocationId(String ip)
{
    for (int i = 0; i < sizeof(ipIdMappings) / sizeof(ipIdMappings[0]); i++)
    {
        if (ip == ipIdMappings[i].ip)
        {
            return ipIdMappings[i].idd;
        }
    }

    return "FAIL";
}
void beep(int frequency, int duration)
{
    tone(BUZZER_PIN, frequency, duration);
    delay(duration);
    noTone(BUZZER_PIN);
}

void displayIntValues(String ip)
{

    String idd = getLocationId(ip);
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("IP: ");
    lcd_1.print(ip);
    if (idd == "FAIL")
    {
        indicateFail();
    }
    else
    {
        indicateSuccess();
    }
    lcd_1.setCursor(0, 1);
    lcd_1.print("ID: ");
    lcd_1.print(idd);
    delay(2000);
    lcd_1.clear();
}

void indicateSuccess()
{
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    beep(2000, 150);
    digitalWrite(redLEDPin, LOW);
}

void indicateFail()
{
    digitalWrite(redLEDPin, LOW);
    beep(2000, 1000);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
}
