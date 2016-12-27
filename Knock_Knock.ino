#include <Adafruit_NeoPixel.h>

const int RedPin = 1;
const int GreenPin = 2;
const int BluePin = 3;

const int RedPin2 = 4;
const int GreenPin2 = 5;
const int BluePin2 = 6;

const int RedPin3 = 7;
const int GreenPin3 = 8;
const int BluePin3 = 9;

const int StripPin1 = 10;
const int StripPin2 = 11;

int game_case = 0;

//Weighted probability for RGB LED color
int color_weights[] = {51, 51, 51, 51, 51, 52, 52, 53, 53, 53};
int led_weights[] = {1, 1, 1, 1, 1, 2, 2, 3, 3, 3};
int led_weights2[] = {4, 4, 4, 4, 4, 5, 5, 6, 6, 6};
int led_weights3[] = {7, 7, 7, 7, 7, 8, 8, 9, 9, 9};

int index = 1;
int index2 = 0;

//Set up Analog sensor readings and thresholding
const int knockSensor = A0; // the first piezo is connected to analog pin 0
const int knockSensor2 = A4; // the second piezo is connected to analog pin 4
const int threshold = 350;  // threshold value to decide when the detected sound is a knock or not


int sensorReading = 0;      // variable to store the value read from the sensor pin
int player = 0;

int sensorReading2 = 0;

//POINTS tracking
int points1 = 0;
int points2 = 0;
int color_knocked = 0;

//Set up pixel strips
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(16, StripPin1);

Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(16, StripPin2);

//Set up interval timer
int delay_time = 0;
unsigned long previous_millis = 0;
const long interval = 1000;
unsigned long current_millis = 0;

void setup() {
  
  Serial.begin(9600);

  //Initialize LED strips
  strip1.begin();
  strip1.setBrightness(60);
  strip1.show();

  strip2.begin();
  strip2.setBrightness(60);
  strip2.show();

  //Initialize LED pins
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(RedPin2, OUTPUT);
  pinMode(GreenPin2, OUTPUT);
  pinMode(BluePin2, OUTPUT);
  pinMode(RedPin3, OUTPUT);
  pinMode(GreenPin3, OUTPUT);
  pinMode(BluePin3, OUTPUT);

}

void loop() {
  switch(game_case) {
 
    case(0) : 

      //Condition for if max points have been reached
      if (points1 >= 15 || points2 >= 15) {game_case = 2;}

      
      sensorReading = analogRead(knockSensor);
      sensorReading2 = analogRead(knockSensor2);

      //Tracks which player hit first
       
      if (sensorReading >=threshold) {
        player = 1;
        game_case = 1;
        break;
      }

      if (sensorReading2 >= threshold) {
        player = 2;
        game_case = 1;
        break;
      }
     


      //Controls RGB LED flashes

      current_millis = millis();
      
      if (current_millis-previous_millis >= interval) {
        
         digitalWrite(led_weights[index], LOW);
         digitalWrite(led_weights2[index], LOW);
         digitalWrite(led_weights3[index], LOW);
         
         previous_millis = current_millis;
         index = random(0,10);

         
         digitalWrite(led_weights[index], HIGH);
         digitalWrite(led_weights2[index], HIGH);
         digitalWrite(led_weights3[index], HIGH);
        
      }
      
      break;
 
    case(1) : 
    
      sensorReading = 0;
      color_knocked = color_weights[index];
    
     //Continue to update RGB flashes if necessary
     if (current_millis-previous_millis >= interval) {
         digitalWrite(led_weights[index], LOW);
         digitalWrite(led_weights2[index], LOW);
         digitalWrite(led_weights3[index], LOW);
         previous_millis = current_millis;
         index2 = random(0,10);
         
         
         digitalWrite(led_weights[index2], HIGH);
         digitalWrite(led_weights2[index2], HIGH);
         digitalWrite(led_weights3[index2], HIGH);
        
      }

      //The following is a list of conditions which track
      //points for each player, including point losses
      
      if (player == 1) {
        if (color_knocked == 52) {
        
          points1 += 1;
          Serial.println("POINTS 1");
          Serial.println(points1);
          
          strip1.setPixelColor(points1 - 1, 0, 0, 255);
          strip1.show();
          
        }

       if (color_knocked == 53 && points1 > 0) {
        
          points1 -= 1;
          strip1.setPixelColor(points1, 0, 0, 0);
          strip1.show();
        }

       else {
          Serial.println("POINTS 1");
          Serial.println(points1);
       }
      
      }

      if (player == 2) {
        if (color_knocked == 52) {
        
          points2 += 1;
          strip2.setPixelColor(points2 - 1, 0, 0, 255);
          strip2.show();
          Serial.println("POINTS 2");
          Serial.println(points2);
        }

       if (color_knocked == 53) {
        
          points2 -= 1;
          strip2.setPixelColor(points2, 0, 0, 0);
          strip2.show();
          Serial.println("POINTS 2");
          Serial.println(points2);
        }

       else {
          Serial.println("POINTS 2");
          Serial.println(points2);
       }
      
      }

      
      delay(700);
      game_case = 0;
      break;
  
  case(2) :

    digitalWrite(led_weights[index], LOW);

    //Trigger LED strips depending on which player won
    if (points1 >= 15) {

      Serial.println("PLAYER 1 Wins!!");
    
      for (int i = 0; i < 16; ++i) {
        strip1.setPixelColor(i, 255, 0, 0);
        strip1.show();
        delay(200);
      }

      for (int i = 0; i < 16; ++i) {
        strip1.setPixelColor(i, 0, 0, 0);
        strip1.show();
        delay(10);
      }
      

    
    }

    else {
      
      Serial.println("PLAYER 2 Wins!!!");

       for (int i = 0; i < 16; ++i) {
        strip2.setPixelColor(i, 255, 0, 0);
        strip2.show();
        delay(200);
      }

      for (int i = 0; i < 16; ++i) {
        strip2.setPixelColor(i, 0, 0, 0);
        strip2.show();
        delay(10);
      }
      
    }


    
  }
}


void celebration() {
  for (int i = 1; i < 4; ++i) {
    digitalWrite(i, HIGH);
    delay(300);
    digitalWrite(i, LOW);
  }
}

