// Motor pins
#define enA 10
#define in1 9
#define in2 8
#define in3 7
#define in4 6
#define enB 5

// Sensors and Servo
#define L_S A0
#define R_S A1
#define echo A2
#define trigger A3
#define servo A5

int Set = 10; // Obstacle detection threshold (in cm)

// Fake IR sensor values (for simulation)
int fake_left_IR = 0;
int fake_right_IR = 0;

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

  servoPulse(servo, 70); // Servo to center
  delay(500);
}

void loop() {
  int distance_F = Ultrasonic_read();
  Serial.print("Front Distance: ");
  Serial.println(distance_F);

  if (distance_F <= Set) {
    Stop();
    delay(1000);
    handleObstacle();
  } else {
    lineFollow();
  }
}

// --- Functions ---

void lineFollow() {
  int left_ir = fake_left_IR;
  int right_ir = fake_right_IR;

  int state = (left_ir << 1) | right_ir;  // Combine both sensor states into 2-bit number

  switch (state) {
    case 0b00:  // Both detect line
      forward();
      break;
    case 0b01:  // Only right IR off the line
      turnRight();
      break;
    case 0b10:  // Only left IR off the line
      turnLeft();
      break;
    case 0b11:  // Both off the line
      Stop();
      break;
  }
}

void handleObstacle() {
  int distance_Left, distance_Right;

  for (int angle = 70; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  delay(500);
  distance_Left = Ultrasonic_read();
  Serial.print("Left Distance: ");
  Serial.println(distance_Left);
  delay(100);

  if (distance_Left > Set) {
    turnLeft();
    delay(2000);
    Stop();
    delay(200);

    for (int angle = 0; angle <= 70; angle += 5) {
      servoPulse(servo, angle);
    }
    delay(300);

    forward();
    delay(2000);
    Stop();
    delay(200);

    for (int angle = 70; angle <= 140; angle += 5) {
      servoPulse(servo, angle);
    }
    delay(300);

    turnRight();
    delay(2000);
    Stop();
    delay(200);

    for (int angle = 140; angle >= 70; angle -= 5) {
      servoPulse(servo, angle);
    }
    delay(300);
  } else {
    for (int angle = 70; angle <= 140; angle += 5) {
      servoPulse(servo, angle);
    }
    delay(500);
    distance_Right = Ultrasonic_read();
    Serial.print("Right Distance: ");
    Serial.println(distance_Right);
    delay(100);

    turnRight();
    delay(2000);
    Stop();
    delay(200);

    for (int angle = 140; angle >= 70; angle -= 5) {
      servoPulse(servo, angle);
    }
    delay(300);

    forward();
    delay(2000);
    Stop();
    delay(200);

    turnLeft();
    delay(2000);
    Stop();
    delay(200);
  }
}

void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(20);
}

long Ultrasonic_read() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn(echo, HIGH, 30000);
  if (time == 0) {
    return 400;
  }
  return time / 29 / 2;
}

void forward() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
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
