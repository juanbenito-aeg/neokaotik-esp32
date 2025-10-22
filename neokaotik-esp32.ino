// Include the Servo library
#include <ESP32Servo.h>

const String CLOSED = "Closed";
const String OPEN = "Open";
String chestState = CLOSED;

// Define the distance in cm used to determine whether the chest should be closed or open
const int LIMIT_DISTANCE = 15;

// Define the pin that powers the LEDs
const int LED_PIN = 23;

// Define the servo and the pin it is connected to
Servo myServo;
const int servoPin = 13;

// Define the pins for the ultrasonic sensor
const int trigPin = 12;
const int echoPin = 14;

// Define the sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration; // Saves the travel time of the ultrasonic waves (time elapsed since transmission and reception of the pulse wave)
float distance; // Saves the distance to an object/person in centimeters

void setup() {
  Serial.begin(115200); // Starts the serial communication
  setUpLED();
  setUpServo();
  setUpUltrasonicSensor();
}

void setUpServo() {
  // Tell the servo library which pin it is connected to
  myServo.attach(servoPin);
}

void setUpLED() {
  pinMode(LED_PIN, OUTPUT); // Sets the LED pin as an Ouput
}

void setUpUltrasonicSensor() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  updateDistanceToObjectOrPerson();
  updateChestState();
}

void updateDistanceToObjectOrPerson() {
  // Clears the trigPin (gives a short LOW pulse to ensure a clean HIGH pulse will be gotten)
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distance = duration * SOUND_SPEED/2;

  // Prints the distance in the Serial Monitor
  Serial.print("Distance in cm: " );
  Serial.println(distance);

  delay(1000);
}

void updateChestState() {
  if (distance <= LIMIT_DISTANCE && chestState == CLOSED) {
    myServo.write(90);
    
    digitalWrite(LED_PIN, HIGH);

    chestState = OPEN;
  } else if (distance > LIMIT_DISTANCE && chestState == OPEN) {
    myServo.write(0);

    digitalWrite(LED_PIN, LOW);
    
    chestState = CLOSED;
  }

  Serial.print("Chest state: ");
  Serial.println(chestState);
}