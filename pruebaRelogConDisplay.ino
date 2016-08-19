
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <idDHT11.h>

#define printByte(args)  write(args);

RTC_DS1307 RTC;

int idDHT11pin = 2; //Digital pin for comunications
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)
uint8_t relog[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
float muestra[3] ={1,1,1};
int im=0;
int potenciometro = A8;
int poteValor = 0;

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void dht11_wrapper();
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

void setup()
{
  Wire.begin(); // Inicia el puerto I2C
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  RTC.begin();
  Serial.begin(9600);
  lcd.createChar(1, relog);
}
void dht11_wrapper() {
  DHT11.isrCallback();
}

float temperatura(){
  return muestra[0]+muestra[1]+muestra[2];
}

void loop()
{
  DateTime now = RTC.now();
  DHT11.acquire();
  
  poteValor= analogRead(potenciometro);
  
  lcd.clear();
  lcd.printByte(1);
  lcd.print(now.hour(), DEC); // Horas
  lcd.print(':');
  lcd.print(now.minute(), DEC); // Minutos
  
  lcd.setCursor(7,0);
  lcd.print("|");
  lcd.print(DHT11.getCelsius(),0);
  lcd.printByte(0xB2);
  lcd.print(" ");
  lcd.print(DHT11.getHumidity(),0);
  lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print(poteValor);

  delay(1000);
  
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
}
