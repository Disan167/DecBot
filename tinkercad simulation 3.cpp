#include <Servo.h>

#define EN_PIN 5     // L293D Enable pin
#define IN1 9        // L293D Input 1
#define IN2 10       // L293D Input 2
#define POT_PIN A0   // Potentiometer (Speed Control)
#define BTN_PIN 2    // Push button pin for sweeping
#define SWEEP_PIN 6  // Servo motor pin for sweeping arm

bool sweeping = false;  // Sweeping status (OFF by default)
bool lastButtonState = HIGH;  

Servo sweepingArm;  // Create a Servo object for the sweeping arm

void setup() {
    pinMode(EN_PIN, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(POT_PIN, INPUT);
    pinMode(BTN_PIN, INPUT_PULLUP); // Internal pull-up resistor

    sweepingArm.attach(SWEEP_PIN);  // Attach the servo to pin 6
}

void loop() {
    int potValue = analogRead(POT_PIN);  // Read potentiometer (0-1023)
    int speed = map(potValue, 0, 1023, 0, 255);  // Map to PWM (0-255)

    analogWrite(EN_PIN, speed);  // Adjust motor speed

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Button Press Detection
    bool buttonState = digitalRead(BTN_PIN);
    if (buttonState == LOW && lastButtonState == HIGH) {
        sweeping = !sweeping;  // Toggle sweeping on/off
        delay(200);  // Debounce delay
    }
    lastButtonState = buttonState;

    // If sweeping is enabled, move the servo back and forth
    if (sweeping) {
        for (int pos = 0; pos <= 90; pos++) {
            sweepingArm.write(pos);
            delay(3);  // Faster sweeping
        }
        for (int pos = 90; pos >= 0; pos--) {
            sweepingArm.write(pos);
            delay(3);
        }
    }
}
