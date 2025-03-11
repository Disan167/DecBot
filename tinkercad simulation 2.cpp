#include <Servo.h>

#define EN_PIN 5     // L293D Enable pin
#define IN1 9        // L293D Input 1
#define IN2 10       // L293D Input 2
#define POT_PIN A0   // Potentiometer (Speed Control)
#define SWEEP_PIN 6  // Servo motor pin for sweeping arm
#define IR_PIN 7     // IR sensor pin

bool direction = true; // True = Forward, False = Backward

Servo sweepingArm;  // Servo object for sweeping arm

void setup() {
    pinMode(EN_PIN, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(POT_PIN, INPUT);
    pinMode(IR_PIN, INPUT); // IR sensor input

    sweepingArm.attach(SWEEP_PIN);  // Attach the servo to pin 6
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

    // Check for obstacles using IR sensor
    if (digitalRead(IR_PIN) == LOW) { // Object detected (LOW signal)
        analogWrite(EN_PIN, 0);  // Stop motor
        sweepingArm.write(90);   // Hold sweeping arm in the middle
        delay(1000);             // Pause before retrying
        return;                  // Skip sweeping if obstacle detected
    }

    // Sweep the arm back and forth
    for (int pos = 0; pos <= 90; pos++) {
        sweepingArm.write(pos);
        delay(5);
    }
    for (int pos = 90; pos >= 0; pos--) {
        sweepingArm.write(pos);
        delay(5);
    }
}
