/*
    Project: Knot, a robotic offcut piece of scrap lumber

  __/\\\________/\\\___________________________________________        
   _\/\\\_____/\\\//____________________________________________       
    _\/\\\__/\\\//_____________________________________/\\\______      
     _\/\\\\\\//\\\______/\\/\\\\\\_______/\\\\\_____/\\\\\\\\\\\_     
      _\/\\\//_\//\\\____\/\\\////\\\____/\\\///\\\__\////\\\////__    
       _\/\\\____\//\\\___\/\\\__\//\\\__/\\\__\//\\\____\/\\\______   
        _\/\\\_____\//\\\__\/\\\___\/\\\_\//\\\__/\\\_____\/\\\_/\\__  
         _\/\\\______\//\\\_\/\\\___\/\\\__\///\\\\\/______\//\\\\\___ 
          _\///________\///__\///____\///_____\/////_________\/////____
      
    Author: Toglefritz

    Description: Knot represents what is arguably the final evolutionary form of a spare
    bit of 2x4 lumber. By hollowing out the wood, it can be filled by a RedBoard Artemis,
    the board for which this sketch was written, two stepper motor controllers, and two
    stepper motors. The whole thing is designs such that all this tech is hidden when
    the robot is viewed from the top, meaning it just looks like a normal piece of non-robotic
    wood when viewed from that perspective. The robot is controlled by tapping on the top
    of the wooden chassis. One tap moves the robot forward, two taps turns right, three 
    turns left, and four moves the robot backward.

    Instructions for building the robot can be found on Instructables.
*/

// Include libraries for running the Quiic stepper drivers
#include <SparkFun_Qwiic_Step.h>

// Knot uses two stepper drivers
QwiicStep motor1;
QwiicStep motor2;

const int knockSensor = A0;   // The piezo is connected to analog pin 0
const int threshold = 20;     // Threshold value to decide when the detected sound is a knock or not. 
                              // Decrease this value to make the robot more sensitive to knocks or 
                              // increase the value to require harder knocks.
int sensorReading = 0;        // Variable to store the value read from the sensor pin
int knockCount = 0;           // A variable to determine how many times the robot was knocked
int prevKnockTime = 0;        // A variable to track the time of each knock for counting purposes
int knockWindow = 700;        // The maximum time (in ms) between knocks for them to count as a single command
int knockDebounce = 30;       // A variable representing the minimum time to wait between knocks, to avoid registering
                              // multiple knocks with one mallet hit
int motorStepsPerRev = 200;   // The number of steps for one full revolution of the motors

void setup() { 
  // Set the pin to which the knock sensor is connected as an INPUT
  pinMode(knockSensor, INPUT);

  // Start Serial communication
  Serial.begin(9600);
  Serial.println("Hello, my name is Knot.");
  Serial.println("I may look like lumber, but I am actually a robot.");
  Serial.println();

   Wire.begin();
   
   // Check if the stepper drivers are correctly connected to I2C
   if (motor1.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }
  if (motor2.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }
}

void loop() { 
  // Read the sensor and store it in the variable sensorReading
  sensorReading = analogRead(knockSensor);

  // Checks if the sensor reading is higher than the threshold value
  if (sensorReading >= threshold && millis() - prevKnockTime > knockDebounce) {
    Serial.println("Knock!"); 
    prevKnockTime = millis();
    knockCount++;
  }

  // See if we've waited at least one knockWindow period since the last knock
  // We also impose a mininmum time window for debouncing
  if (knockCount > 0 && millis() - prevKnockTime > knockWindow && millis() - prevKnockTime > knockDebounce) {
    Serial.println(String(knockCount) + " knocks detected");
    knockCount = 0;

    // Alright! Let's move the robot.
    // TODO go hide in a secure location with at least several months of provisions and then write the code to make the robot move
  }
}
