#include <LiquidCrystal.h> //including library for LCD (16,2) 
const int rs = 9, en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //LCD pinout declaration
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include <RF24.h>   //include library for NRF module
RF24 radio(7, 6); // CE, CSN
const byte address[6] = "00001";

const byte numChars = 35;       //assign size of array
char receivedChars[numChars];   // an array to store the received data

void setup() {
    Serial.begin(115200);                     //open serial port with 115200 baud rate
    Serial.println("<Arduino is ready>");     //print to ensure serial is open

    radio.begin();      //begin radio comms
    radio.openReadingPipe(0, address);       //allow comms to receive
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();      //allow comms to receive
    
    lcd.begin(16,2);      //begin LCD screen, defining the size of the screen
}

void loop() {
    showNewData();
}

void showNewData() {

 if (radio.available()) {
    char text[35] = "";       //create array of 35 chars
    radio.read(&text, sizeof(text));      //receive data and save into the array called "text" and expect data the size that array which is 35 chars
    Serial.println(text);
  
        //loop to print temperature
        lcd.setCursor(0,0);                  //set LCD cursor to start of 1st line to get ready to print temperature
        for (int i = 0; i < 15; i++){        //points 0 - 14 in the received data array contain data for temp
        lcd.print(text[i]);                  //print the aforementioned data from the array to the LCD
        }
        
        lcd.setCursor(0,1);                 //set LCD cursor to start of 2nd line to get ready to print humidity

        //loop to print humidity
        for (int i = 16; i < 29; i++){      //points 16 - 29 in the received data array contain data for humidity
        lcd.print(text[i]);        //print the aforementioned data from the array to the LCD
        }

        
        delay(100);

 }
    }
