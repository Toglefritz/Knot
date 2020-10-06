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

const int knockSensor = A0;   // The piezo is connected to analog pin 0
const int threshold = 20;      // Threshold value to decide when the detected sound is a knock or not. 
                              // Decrease this value to make the robot more sensitive to knocks or 
                              // increase the value to require harder knocks.
int sensorReading = 0;        // Variable to store the value read from the sensor pin

void setup() { 
  Serial.begin(9600);       // Use the serial port
}

void loop() {
  // Read the sensor and store it in the variable sensorReading
  sensorReading = analogRead(knockSensor);

  // Checks if the sensor reading is higher than the threshold value
  if (sensorReading >= threshold) {
    // Send the string "Knock!" via Serial, followed by newline
    Serial.println("Knock!");
  }
  delay(30);  // A little delay to help with debouncing (registering multiple knocks with only one mallet hit)
}
