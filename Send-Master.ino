
#include <dht.h> //including library for DHT sensor and intitalising the set up
dht DHT; 
#define DHT11_PIN 5
#include <RF24.h>
RF24 radio(7, 6); // CE, CSN
const byte address[6] = "00001";

//initialise pin numbers
int buzzer = 4;      // buzzer pin
int trigPin = 9;     // sensor trigger pin for ultrasonic sensor/ulstrasonic transducer
int echoPin = 8;     // sensor echo pin
int buttonIn = 3;    // button trigger pin

int trigPin1 = 1;   // sensor trigger pin for second ultrasonic sensor/ulstrasonic transducer 
int echoPin1 = 0;   // sensor 2 echo pin

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);   //open serial port with 115200 baud rate
  radio.begin();          //begin radio comms
  radio.openWritingPipe(address);    //allow comms to transmitt
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();     //make sure module is NOT receiving

  pinMode(trigPin, OUTPUT); //set trigger pin as an output
  pinMode(echoPin, INPUT_PULLUP);  //set echo pin as an output
  pinMode(buzzer, OUTPUT);  //set buzzer pin as an output
  pinMode(trigPin1, OUTPUT); //set trigger pin as an output
  pinMode(echoPin1, INPUT_PULLUP);  //set echo pin as an output

  pinMode(buttonIn, INPUT_PULLUP);    //define button in as input and a pull up state
  digitalWrite(buttonIn, HIGH);       //set buttonIn to be high
}

void loop() {
  // put your main code here, to run repeatedly:
int buttonState = 0;  //set button state equal to 0
  buttonState = digitalRead(buttonIn);  //reassign button state from the initial 0 to whatever the digital read is, in this case HIGH
  digitalWrite(trigPin,buttonState);    //set triggerpin to the value of buttonState, which is either high or low

  
  long timeTaken, timeTaken1;                 //define variables timeTaken and dist to be 'long' which allow it to store a large number
  int dist, dist0, dist1;
  int prox0 = 0;                         //define variable prox as an integer with value 0
  int prox1 = 0;
 
  if(digitalRead(buttonIn)==HIGH) {     //create if statement to complete the following commands if the button is 'HIGH'/unpressed

 
     //define the value read from the DHT sensor as an integer
     int chk = DHT.read11(DHT11_PIN); ;
     int t = DHT.temperature;           //assign value for temp to variable t
     int h = DHT.humidity;              //assign value for humidity to variable h
     String stringOne = "Temp: ";       //create different strings containing info to print temp and humidity to serial
     String stringTwo = "C";
     String stringThree = stringOne + t + char(223) + stringTwo; //creating one string using other strings //char(223) is degree symbol on LCD display
     String stringFour = "      ";      //print spacing for LCD display
     String stringFive = "Humidity: ";
     String stringSix = "%";            //print % as string
     String stringSeven = stringThree + stringFour + stringFive + h + stringSix;
   
     Serial.println(stringThree + stringFour + stringFive + h + stringSix); //print different strings to serial
     char charBuf[35];
     stringSeven.toCharArray(charBuf, 35); 
     radio.write(&charBuf, sizeof(charBuf));
     delay(1500);
}

if(digitalRead(buttonIn)==LOW){       //create if statement to complete the following commands if the button is 'LOW'/pressed

  digitalWrite(trigPin, LOW);         //set trigger pin to LOW
   digitalWrite(trigPin1, LOW);         //set trigger pin to LOW
  delayMicroseconds(5);               //delay of 2 microseconds
  digitalWrite(trigPin, HIGH);        //set trigger pin to HIGH
  digitalWrite(trigPin1, HIGH);        //set trigger pin to HIGH
  delayMicroseconds(20);             //delay of 100 microseconds
  digitalWrite(trigPin, LOW);         //set trigger pin to LOW
  digitalWrite(trigPin1, LOW);         //set trigger pin to LOW
  timeTaken = pulseIn(echoPin, HIGH); //determine distance of wave
  Serial.println(timeTaken);
  timeTaken1 = pulseIn(echoPin1, HIGH); //determine distance of wave
  Serial.println(timeTaken1);
  dist0 = (timeTaken*0.0343) / 2;      //using timeTaken calc distance of object and assign to variable dist
  Serial.println(dist0);
 // prox0 = map(dist, 0, 600, 8, 0);      //map the distannce between 0 and 48 cm to a value between 0 and 8
 
  dist1 = (timeTaken1*0.0343) / 2;      //using timeTaken calc distance of object and assign to variable dist
  Serial.println(dist1);
 // prox1 = map(dist, 0, 600, 8, 0);      //map the distannce between 0 and 48 cm to a value between 0 and 8

  //Serial.println("Final Debug");    //uncomment these lines while debugging to see distances for each sensor
  //Serial.println(dist0);
  //Serial.println(dist1);
  //Serial.println(dist);

  if (dist0 > 0 && dist1 > 0)      //if both sensors are recording a distance more than 0cm
  {
   dist = (dist0 + dist1)/2;       //make dist = the average distance
  }
    else if (dist0 <= 0 || dist1 <= 0)     //if either of the readings are 0cm set average dist to 0cm
    {
      dist = 0;
      }

  Serial.println(dist);


  
 //sound buzzer at different distances  
 
  if (dist >= 70) {  //if distance if 1 or 2 
    tone(buzzer,1500,100);        //sound buzzer at 1500HZ and duration of 100ms
    delay(2000);                  //2 second delay
   }
   else {                         //otherwise
  noTone(buzzer);                 //no sound
  }

  if (dist >= 50 && dist < 60  ) {               //if distance if 3
    tone(buzzer,1500,100);        //sound buzzer at 1500HZ and duration of 100ms
     delay(1000);                 //1 second delay
   }
   else {                         //otherwise
  noTone(buzzer);                 //no sound
  }

  if (dist > 40 && dist <50) {               //if distance if 4
    tone(buzzer,1500,100);        //sound buzzer at 1500HZ and duration of 100ms
     delay(500);                  //half second delay
   }
   else {                         //otherwise
  noTone(buzzer);                 //no sound
  }


  if (dist > 30 && dist <= 40 ) {    //if distance if 5 or 6 
    tone(buzzer,2000,80);         //sound buzzer at 2000HZ and duration of 80ms
     delay(200);                  //delay 200milliseconds
  } else {                        //otherwise
    noTone(buzzer);               //no sound
  }

  if (dist <= 30) {   //if distance if 7 or 8 
    tone(buzzer,2000,100);         //sound buzzer at 2000HZ and duration of 80ms
     delay(50);                   //delay 50milliseconds
  } else {                        //otherwise 
    noTone(buzzer);               //no sound
  }

      Serial.flush();
        //if the readings are in range print the distance
   if (dist >= 31 && dist <= 200){       //if value of dist is more than 31cm and less than 200cm complete following
     String string1 = "Distance: ";
     String string2 = "cm  ";
     String string3 = "               ";     //blank space to ensure rest of line on LCD shows nothing
     String string4 = string1 + dist + string2 +string3;   //adding strings together
     Serial.println(string4);           //print total string to serial
     char charBuf1[35];                 //create an array for 35 chars
     string4.toCharArray(charBuf1, 35);     //convert the string to an array of chars to be sent by radio comms
     radio.write(&charBuf1, sizeof(charBuf1));    //Write the message to radio comms
     delay(100);                   //print cm on LCD
 }
      //if the readings are not in range print out of range
 else if(dist < 31  || dist > 200 || timeTaken <= 1800 || timeTaken1 <=1800){                  //else if function for if any of these are true, print "out of range" 
  Serial.println("Out of Range                      ");      //print Out of Range and clear rest of  LCD
  const char text[] = "Out of Range                      ";
  radio.write(&text, sizeof(text));                   //Write the message to radio comms
  delay(100);
 }
}     //close buttonIn==LOW loop
}     //close void loop
