const int trigPin = 12;
const int echoPin = 13;

const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

int threshold = 4; // distance in cm to consider "blocked"

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  int distance = getDistance();

  if (distance >= 1 && distance <= 400) {
    if (distance > threshold) {
      moveForward();
    } else {
      stopBot();
      delay(300);

      // Try turning left first
      turnLeft();
      delay(400);
      stopBot();
      delay(300);

      // Check if the new direction is clear
      int newDistance = getDistance(); 

      if (newDistance <= threshold) {
        // Left path also blocked → turn right twice
        turnRight();
        delay(400);
        stopBot();
        delay(300);

        turnRight();
        delay(400);
        stopBot();
        delay(300);
        // Check right hand side distance and then decide to go forward or to continue in the loop
      }
    }
  } else {
    stopBot(); // fallback if reading is invalid
  }

  delay(50);
}

// ===== Distance Sensing =====

int getDistance() {
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

// ===== Motor Control =====

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopBot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); // Left wheel reverse
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  // Right wheel forward
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  // Left wheel forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); // Right wheel reverse
}