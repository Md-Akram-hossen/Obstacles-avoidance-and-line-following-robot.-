// Motor pins
#define enA 10  // Enable pin for Left Motor
#define in1 9   // Left Motor Input 1
#define in2 8   // Left Motor Input 2
#define in3 7   // Right Motor Input 1
#define in4 6   // Right Motor Input 2
#define enB 5   // Enable pin for Right Motor
// Sensors and servo
#define L_S A0  // Left IR sensor
#define R_S A1  // Right IR sensor
#define echo A2 // Ultrasonic echo pin
#define trigger A3 // Ultrasonic trigger pin
#define servo A5 // Servo motor control pin
int Set = 15; // Distance threshold
int distance_L, distance_F, distance_R;
void setup() {
  Serial.begin(9600);
  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(servo, OUTPUT);

  // --- DIRECT MOTOR TEST IN SETUP ---
  // Set both enable pins HIGH for full speed
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Make both motors spin forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  Serial.println("Motors should be spinning forward now...");
  delay(5000); // Let them spin for 5 seconds

  // Stop the motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  Serial.println("Motors stopped.");
  delay(1000);

  // --- END OF DIRECT MOTOR TEST ---

  // Servo sweep at startup (keep this for now)
  for (int angle = 70; angle <= 140; angle += 5) {
    servoPulse(servo, angle);
  }
  for (int angle = 140; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  for (int angle = 0; angle <= 70; angle += 5) {
    servoPulse(servo, angle);
  }
  distance_F = Ultrasonic_read();
  delay(500);
}
void loop() {
  distance_F = Ultrasonic_read();
  Serial.print("D F="); Serial.println(distance_F);
  if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 0)) {
    if (distance_F > Set) {
      forward();
    } else {
      Check_side();
    }
  } else if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 0)) {
    turnRight();
  } else if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 1)) {
    turnLeft();
  }
  delay(10);
}
void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500; // Map angle to PWM pulse width
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(20); // Reduced delay for faster scanning
}
long Ultrasonic_read() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn(echo, HIGH, 30000); // 30ms timeout
  if (time == 0) {
    return 400; // If no echo received, assume max distance
  }
  return time / 29 / 2; // Convert to centimeters
}
void compareDistance() {
  if (distance_L > distance_R) {
    turnLeft();
    delay(500);
    forward();
    delay(600);
    turnRight();
    delay(500);
    forward();
    delay(600);
    turnRight();
    delay(400);
  } else {
    turnRight();
    delay(500);
    forward();
    delay(600);
    turnLeft();
    delay(500);
    forward();
    delay(600);
    turnLeft();
    delay(400);
  }
}
void Check_side() {
  Stop();
  delay(100);
  // Look Right
  for (int angle = 70; angle <= 140; angle += 5) {
    servoPulse(servo, angle);
  }
  delay(300);
  distance_R = Ultrasonic_read();
  Serial.print("D R="); Serial.println(distance_R);
  delay(100);
  // Look Left
  for (int angle = 140; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  delay(500);
  distance_L = Ultrasonic_read();
  Serial.print("D L="); Serial.println(distance_L);
  delay(100);
  // Center
  for (int angle = 0; angle <= 70; angle += 5) {
    servoPulse(servo, angle);
  }
  delay(300);
  compareDistance();
}
// Motor movement functions
void forward() {
  analogWrite(enA, 255); // Full speed
  analogWrite(enB, 255); // Full speed
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void backward() {
  analogWrite(enA, 255); // Full speed
  analogWrite(enB, 255); // Full speed
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void turnRight() {
  analogWrite(enA, 255); // Full speed
  analogWrite(enB, 255); // Full speed
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void turnLeft() {
  analogWrite(enA, 255); // Full speed
  analogWrite(enB, 255); // Full speed
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void Stop() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
