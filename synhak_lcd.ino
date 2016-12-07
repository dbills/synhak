/* YourDuino.com Example Software Sketch
 16 character 2 line I2C Display
 Backpack Interface labelled "A0 A1 A2" at lower right.
 ..and
 Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
 MOST use address 0x27, a FEW use 0x3F
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


void lcd_setup()   
{
  //Serial.begin(9600);

  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight

// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  

}

static int calc_numsize(int x) {
  int i=0;
  while(1) {
    x=x/10;
    i++;
    if(x==0)
      break;
  }
  return i;
}

static void _lcd_update(int rotations,int rpm) {
  lcd.setCursor(0,0); //Start at character 4 on line 0
  // 1234567890123456
  //  Revs:  |   RPM:
  // +00000  |  +6000
  lcd.print("  Revs  |  RPM  ");
  lcd.setCursor(0,1);
  String line2;
  if(rotations>=0)
    line2+=".+";
  else
    line2+=".";
  
  int sz=calc_numsize(rotations);
  for(int i=0;i<5-sz;i++) {
    line2+=".";
  }
  line2+=rotations;
  line2+="_|__";
  
  sz=calc_numsize(rpm);
  for(int i=0;i<4-sz;i++) {
    line2+=".";
  }
  line2+=rpm;    
  
  lcd.print(line2);
}

static unsigned long last_update=0;

void lcd_update(int rotations,int rpm) {
  if(millis()-last_update > 750) {
    _lcd_update(rotations,rpm);
    last_update=millis();
  }
}



