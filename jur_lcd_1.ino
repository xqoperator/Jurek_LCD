
/*************************************************************************************

  Mark Bramwell, July 2010

  This program will test the LCD panel and the buttons.When you push the button on the shield，
  the screen will show the corresponding one.
 
  Connection: Plug the LCD Keypad to the UNO(or other controllers)

**************************************************************************************/

#include "FAB_LED.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel


// Declare the LED protocol and the port
ws2812b<D,2>  strip;
const uint8_t numPixels = 4;
const uint8_t maxBrightness = 128;
grb  pixels[numPixels] = {};

void setPIX(char n, uint8_t r, uint8_t g, uint8_t b) {
  pixels[n].r = r;
  pixels[n].g = g;
  pixels[n].b = b;
}

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor 

    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    if (adc_key_in > 1000) return btnNONE; 
/*
    // For V1.1 us this threshold
    if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 250)  return btnUP; 
    if (adc_key_in < 450)  return btnDOWN; 
    if (adc_key_in < 650)  return btnLEFT; 
    if (adc_key_in < 850)  return btnSELECT;  
*/
   // For V1.0 comment the other threshold and use the one below:
   //*
     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 195)  return btnUP; 
     if (adc_key_in < 380)  return btnDOWN; 
     if (adc_key_in < 555)  return btnLEFT; 
     if (adc_key_in < 790)  return btnSELECT;   
   //*/

    return btnNONE;                // when all others fail, return this.
}

void setup() {
   lcd.begin(16, 2);               // start the library
   lcd.setCursor(0,0);             // set the LCD cursor   position 
   lcd.print("Wpisz kod!");     // print a simple message on the LCD

   strip.clear(2 * numPixels);
   pixels[0].r = maxBrightness;
   pixels[1].g = maxBrightness;
   pixels[2].b = maxBrightness;
}

uint16_t c = 10;
int poprzedni = btnNONE;

void loop() {
  strip.sendPixels(numPixels, pixels);
  setPIX(3,0,0,((millis()/1000) & 1) ? 0 : maxBrightness);
  
   lcd.setCursor(9,1);             // move cursor to second line "1" and 9 spaces over
//   lcd.print(millis()/1000);       // display seconds elapsed since power-up
   lcd.print(c);       // display seconds elapsed since power-up
   //lcd.print(analogRead(0));
   lcd.print("  ");

   lcd.setCursor(0,1);             // move to the begining of the second line
   lcd_key = read_LCD_buttons();   // read the buttons

   switch (lcd_key){               // depending on which button was pushed, we perform an action

       case btnRIGHT:{             //  push button "RIGHT" and show the word on the screen
            lcd.print("RIGHT ");
            setPIX(0,maxBrightness,0,0);
            break;
       }
       case btnLEFT:{
             lcd.print("LEFT   "); //  push button "LEFT" and show the word on the screen
            setPIX(0,0,maxBrightness,0);
             break;
       }    
       case btnUP:{
             lcd.print("UP    ");  //  push button "UP" and show the word on the screen
             if(poprzedni == btnNONE)
               if(c<20) c++;
             break;
       }
       case btnDOWN:{
             lcd.print("DOWN  ");  //  push button "DOWN" and show the word on the screen
             if(poprzedni == btnNONE) 
               if(c>0) c--;
             break;
       }
       case btnSELECT:{
             lcd.print("SELECT");  //  push button "SELECT" and show the word on the screen
             break;
       }
       case btnNONE:{
             lcd.print("NONE  ");  //  No action  will show "None" on the screen
          for(int o=0; o<numPixels-1; o++) {
            setPIX(o,0,0,0);
          }
       }
   }

   poprzedni = lcd_key;
}
