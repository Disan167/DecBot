#include <Servo.h>

#define EN_PIN 5     // L293D Enable pin
#define IN1 9        // L293D Input 1
#define IN2 10       // L293D Input 2
#define POT_PIN A0   // Potentiometer (Speed Control)
#define BTN_PIN 2    // Push button pin for sweeping
#define IR_PIN 4     // IR sensor pin for obstacle detection
#define SWEEP_PIN 6  // Servo motor pin for sweeping arm

bool sweeping = false;  // Sweeping status (OFF by default)
bool lastButtonState = HIGH;
bool obstacleDetected = false; // For simulating obstacle detection

int sweepPosition = 0;  // Current position of the sweeping arm

Servo sweepingArm;  // Create a Servo object for the sweeping arm

void setup() {
    pinMode(EN_PIN, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(POT_PIN, INPUT);
    pinMode(BTN_PIN, INPUT_PULLUP); // Internal pull-up resistor for button 1
    pinMode(IR_PIN, INPUT);         // Input for IR sensor

    sweepingArm.attach(SWEEP_PIN);  // Attach the servo to pin 6
}

void loop() {
    int potValue = analogRead(POT_PIN);  // Read potentiometer (0-1023)
    int speed = map(potValue, 0, 1023, 0, 255);  // Map to PWM (0-255)

    analogWrite(EN_PIN, speed);  // Adjust motor speed

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Button 1 Press Detection for sweeping control
    bool buttonState = digitalRead(BTN_PIN);
    if (buttonState == LOW && lastButtonState == HIGH) {
        sweeping = !sweeping;  // Toggle sweeping on/off
        delay(200);  // Debounce delay
    }
    lastButtonState = buttonState;

    // Check IR sensor for obstacle detection
    obstacleDetected = (digitalRead(IR_PIN) == LOW); // When IR sensor detects something

    // If sweeping is enabled
    if (sweeping) {
        if (!obstacleDetected) {
            // Sweep the arm back and forth when no obstacle is detected
            for (int pos = sweepPosition; pos <= 90; pos++) {
                sweepingArm.write(pos);
                delay(5);  // Faster movement
            }
            sweepPosition = 90;  // Update the current position
            for (int pos = sweepPosition; pos >= 0; pos--) {
                sweepingArm.write(pos);
                delay(5);
            }
            sweepPosition = 0;  // Reset to the start position
        } else {
            // If obstacle detected, change the angle of sweeping arm and continue sweeping
            sweepPosition = 45;  // Move the arm to a new position (adjustable)
            sweepingArm.write(sweepPosition);
            delay(500);  // Pause after detecting obstacle
        }
    }
}
