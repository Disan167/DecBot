#include <Servo.h>

#define EN_PIN 5     // L293D Enable pin
#define IN1 9        // L293D Input 1
#define IN2 10       // L293D Input 2
#define POT_PIN A0   // Potentiometer (Speed Control)
#define BTN_PIN 2    // Push button pin
#define SWEEP_PIN 6  // Servo motor pin for sweeping arm

bool direction = true; // True = Forward, False = Backward

Servo sweepingArm;  // Create a Servo object for the sweeping arm

int sweepCompleted = 0;  // Track if the sweeping motion is completed

void setup() {
    pinMode(EN_PIN, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(POT_PIN, INPUT);
    pinMode(BTN_PIN, INPUT_PULLUP); // Internal pull-up resistor

    sweepingArm.attach(SWEEP_PIN);  // Attach the servo to pin 6

    attachInterrupt(digitalPinToInterrupt(BTN_PIN), changeDirection, FALLING);
}

void loop() {
    int potValue = analogRead(POT_PIN);  // Read potentiometer (0-1023)
    int speed = map(potValue, 0, 1023, 0, 255);  // Map to PWM (0-255)

    analogWrite(EN_PIN, speed);  // Adjust motor speed

    if (direction) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }

    // Sweep the arm from 0 to 90 degrees (right) with faster movement
    for (int pos = 0; pos <= 90; pos++) {
        sweepingArm.write(pos);
        delay(5);  // Reduced delay for faster movement
    }

    // Sweep the arm from 90 to 0 degrees (left) with faster movement
    for (int pos = 90; pos >= 0; pos--) {
        sweepingArm.write(pos);
        delay(5);  // Reduced delay for faster movement
    }

    // After a full sweep, stop the motor and arm
    sweepCompleted++;
    if (sweepCompleted >= 1) {
        // Stop motor and arm after one complete sweep
        analogWrite(EN_PIN, 0);  // Stop the motor
        sweepCompleted = 0;      // Reset sweep counter
        sweepingArm.write(90);   // Hold the arm at 90 degrees (middle position)
        delay(1000);             // Pause for 1 second before starting another sweep
    }
}

void changeDirection() {
    delay(10); // Debounce
    direction = !direction; // Toggle direction
}
