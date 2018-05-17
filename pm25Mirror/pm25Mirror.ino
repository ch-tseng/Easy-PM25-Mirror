#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#include <SoftwareSerial.h>
SoftwareSerial Serial1(2, 3); // RX, TX

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//PM2.5
;
long pmat10 = 0;
long pmat25 = 0;
long pmat100 = 0;
unsigned int temperature = 0;
unsigned int humandity = 0;

void retrievepm25(){
  int count = 0;
  unsigned char c;
  unsigned char high;
  while (Serial1.available()) { 
    c = Serial1.read();
    if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
      Serial.println("check failed");
      break;
    }
    if(count > 27){ 
      Serial.println("complete");
      break;
    }
     else if(count == 10 || count == 12 || count == 14 || count == 24 || count == 26) {
      high = c;
    }
    else if(count == 11){
      pmat10 = 256*high + c;
      Serial.print("PM1.0=");
      Serial.print(pmat10);
      Serial.println(" ug/m3");
    }
    else if(count == 13){
      pmat25 = 256*high + c;
      Serial.print("PM2.5=");
      Serial.print(pmat25);
      Serial.println(" ug/m3");
    }
    else if(count == 15){
      pmat100 = 256*high + c;
      Serial.print("PM10=");
      Serial.print(pmat100);
      Serial.println(" ug/m3");
    }
     else if(count == 25){        
      temperature = (256*high + c)/10;
      Serial.print("Temp=");
      Serial.print(temperature);
      Serial.println(" (C)");
    }
    else if(count == 27){            
      humandity = (256*high + c)/10;
      Serial.print("Humidity=");
      Serial.print(humandity);
      Serial.println(" (%)");
    }   
    count++;
  }
  while(Serial1.available()) Serial1.read();
  Serial.println();
}

void scroll(String scrollTXT) {
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println(scrollTXT);
  display.display();
  delay(1);
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}

void scroll1(String header, String body) {
  
  display.setTextSize(1);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println(header);
  display.display();
  
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(12,8);
  
  display.println(body);
  display.display();
  delay(1);
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    

  display.stopscroll();
}

void scroll2(String header, String body) {
  display.setTextSize(1);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println(header);
  display.display();
  
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(12,8);
  
  display.println(body);
  display.display();
  delay(1);
 
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}

void headerPrint(String header) {
  display.setTextSize(1);
  //display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(header);
}

void bodyPrint(String body) {
  display.setTextSize(2);
  display.setCursor(0, 17);
  display.println(body);
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  //display.display();
  //delay(1000);
  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Welcome");
  display.setCursor(0, 18);
  display.println("PM25 MIRROR");
  display.display();
  delay(2000);
  display.clearDisplay();
}



void loop() {
  retrievepm25();
  //display.clearDisplay();
  scroll1("Temperature", String(temperature) + "C");
  
  //display.clearDisplay();
  scroll1("Humandity", String(humandity) + "%");

  //display.clearDisplay();
  scroll1("PM1.0", String(pmat10) + "ug");
  scroll1("PM2.5", String(pmat25) + "ug");
  scroll1("PM100", String(pmat100) + "ug");

}
