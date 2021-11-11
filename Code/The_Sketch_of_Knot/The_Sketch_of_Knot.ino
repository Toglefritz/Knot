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

/*

CONTROLS: 

  -  A single knock moves the robot forward.
  -  Two knocks turns the robot right.
  -  Three knocks turns the robot left.

*/

// Include libraries for running the Quiic stepper drivers
#include <SparkFun_Qwiic_Step.h>

// Knot uses two stepper drivers
QwiicStep motor1;
QwiicStep motor2;

const int knockSensor = A0;   // The piezo is connected to analog pin 0
const int threshold = 100;     // Threshold value to decide when the detected sound is a knock or not. 
                              // Decrease this value to make the robot more sensitive to knocks or 
                              // increase the value to require harder knocks.
int sensorReading = 0;        // Variable to store the value read from the sensor pin
int knockCount = 0;           // A variable to determine how many times the robot was knocked
unsigned long prevKnockTime = 0;        // A variable to track the time of each knock for counting purposes
int knockWindow = 700;        // The maximum time (in ms) between knocks for them to count as a single command
int knockDebounce = 50;       // A variable representing the minimum time to wait between knocks, to avoid registering
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
  if (motor2.begin(8) == false) {
   Serial.println("Device did not acknowledge! Freezing.");
   while (1);
  }

  // Set the motor drivers to run full steps
  motor1.setStepSize(STEPSIZE_FULL);
  motor2.setStepSize(STEPSIZE_FULL);

  // Set the motor speed
  motor1.setSpeed(80.0);
  motor2.setSpeed(80.0);

  // Set the motors' acceleration
  motor1.setAcceleration(200);
  motor2.setAcceleration(200);

  // Disable the motors until we are ready for them to move
  motor1.disable();
  motor2.disable();
}

void loop() {  
  // Read the sensor and store it in the variable sensorReading
  sensorReading = analogRead(knockSensor);

  // Checks if the sensor reading is higher than the threshold value
  if (sensorReading >= threshold && millis() - prevKnockTime > knockDebounce && motor1.isRunning() == false && motor2.isRunning() == false) {
    Serial.println("Knock! (" + String(sensorReading) + ")"); 
    prevKnockTime = millis();
    Serial.println(prevKnockTime);
    knockCount++;
  }
  // See if we've waited at least one knockWindow period since the last knock
  // We also impose a mininmum time window for debouncing
  if (knockCount > 0 && millis() - prevKnockTime > knockWindow && millis() - prevKnockTime > knockDebounce) {
    Serial.println(String(knockCount) + " knocks detected");

    // Turn on the motors
    motor1.enable();
    motor2.enable();

    // Alright! Let's move the robot.
    switch (knockCount) {
      // One knock means move forward
      // Move both motors at once in the same direction
      case 1:  
        Serial.println("Moving forward!");
        motor1.move(-motorStepsPerRev);
        motor2.move(-motorStepsPerRev);
      break;
      // Two knocks means turn right
      case 2:
        Serial.println("Turning right!");
        motor1.move(-motorStepsPerRev);
        motor2.move(motorStepsPerRev);
      break;
      // Three knocks means turn left
      case 3:
        Serial.println("Turning left!");
        motor1.move(motorStepsPerRev);
        motor2.move(-motorStepsPerRev);
      break;
      // By default, we will not do anything
      default:
      break;
    }

    // Rest the  knockCount
     knockCount = 0;
  }
}
