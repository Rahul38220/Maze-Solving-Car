const int trigPin = 12;
const int echoPin = 13;

const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

int threshold = 4;

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

  if (distance < 2 || distance > 400) {
    distance = getDistance(); // retry once if reading is invalid
  }

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > threshold) {
    moveForward();
  } else {
    stopBot();
    delay(100);

    // Try turning right (actual direction, previously labeled left)
    turnRight();
    delay(400);
    stopBot();
    delay(200);

    int afterRight = getDistance();

    if (afterRight <= threshold) {
      // Path still blocked → perform 180° turn
      turnLeft();
      delay(400);
      stopBot();
      delay(200);

      turnLeft();
      delay(400);
      stopBot();
      delay(200);

      // Recheck after U-turn
      int afterUTurn = getDistance();
      if (afterUTurn <= threshold) {
        // Still blocked → turn left to explore a new path
        turnLeft();
        delay(400);
        stopBot();
        delay(200);
      }
    }
  }

  delay(30); // quick loop delay for responsiveness
}

// === Motor Control ===

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

// 🔄 Actually turns right in physical setup
void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// 🔄 Actually turns left in physical setup
void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// === Distance Sensor ===

int getDistance() {
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Timeout after 30 ms if no echo is received
  duration = pulseIn(echoPin, HIGH, 30000);
  distance = duration * 0.034 / 2;

  return distance;
}
// End of the code