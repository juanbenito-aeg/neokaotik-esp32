bool isAuthenticated = false;

// Define the distance in cm used to determine whether the chest should be closed or open
const int LIMIT_DISTANCE = 5;

// Define the pin that powers the LEDs
const int LED_PIN = 23;

// Define the pins for the laser & its sensor
const int laserPin = 34;
const int laserSensorPin = 35;

// Define the pin for the buzzer
const int buzzerPin = 22;

// Define the pins for the ultrasonic sensor
const int trigPin = 12;
const int echoPin = 14;

// Define the sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration; // Saves the travel time of the ultrasonic waves (time elapsed since transmission and reception of the pulse wave)
float distance; // Saves the distance to an object/person in centimeters

void setup() {
  Serial.begin(115200); // Starts the serial communication
  setUpLEDs();
  setUpLaserAndSensor();
  setUpBuzzer();
  setUpUltrasonicSensor();
}

void setUpLEDs() {
  pinMode(LED_PIN, OUTPUT); // Sets the LEDs pin as an Ouput
}

void setUpLaserAndSensor() {
  pinMode(laserPin, OUTPUT); // Sets the laserPin as an Output
  pinMode(laserSensorPin, INPUT); // Sets the laserSensorPin as an Input
  
  // Turn the laser on
  // digitalWrite(laserPin, HIGH);
}

void setUpBuzzer() {
  pinMode(buzzerPin, OUTPUT); // Sets the buzzer pin as an Ouput
}

void setUpUltrasonicSensor() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  updateDistanceToObjectOrPerson();
  updateLEDsStateAndIsAuthenticated();
  updateBuzzer();
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

void updateLEDsStateAndIsAuthenticated() {
  if (distance <= LIMIT_DISTANCE && !isAuthenticated) {
    digitalWrite(LED_PIN, HIGH);
    
    isAuthenticated = true;
  } else if (distance > LIMIT_DISTANCE && isAuthenticated) {
    digitalWrite(LED_PIN, LOW);
    
    isAuthenticated = false;
  }

  Serial.print("User is authenticated: ");
  Serial.println(isAuthenticated);
}

void updateBuzzer() {
  bool laserSensorValue = digitalRead(laserSensorPin);

  if (!isAuthenticated && laserSensorValue == 0) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  } 
}
