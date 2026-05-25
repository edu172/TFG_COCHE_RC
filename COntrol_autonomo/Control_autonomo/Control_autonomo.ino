// ======================================
// CONFIGURACIÓN DE MOTORES
// ======================================

struct Motor {
  int in1;
  int in2;
  int en;
};

// Motores
Motor motorA = {26, 25, 27}; // delante derecha
Motor motorB = {14, 12, 13}; // delante izquierda
Motor motorC = {18, 5, 19};  // atrás derecha
Motor motorD = {4, 0, 2};    // atrás izquierda

// ======================================
// SETUP
// ======================================

void setup() {

  // IMPORTANTE:
  // Debe coincidir con Python
  Serial.begin(115200);

  Serial.println("Sistema RC Autonomo iniciado");

  setupMotor(motorA);
  setupMotor(motorB);
  setupMotor(motorC);
  setupMotor(motorD);

  stopCar();
}

// ======================================
// CONFIGURACIÓN MOTORES
// ======================================

void setupMotor(Motor m) {
  pinMode(m.in1, OUTPUT);
  pinMode(m.in2, OUTPUT);
  pinMode(m.en, OUTPUT);
}

// ======================================
// CONTROL INDIVIDUAL MOTOR
// ======================================

void setMotor(Motor m, int direction, int speed) {

  speed = constrain(speed, 0, 255);

  // direction:
  // 1  = adelante
  // -1 = atrás
  // 0  = parado

  if (direction == 1) {
    digitalWrite(m.in1, HIGH);
    digitalWrite(m.in2, LOW);
  }
  else if (direction == -1) {
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, HIGH);
  }
  else {
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, LOW);
  }

  analogWrite(m.en, speed);
}

// ======================================
// MOVIMIENTOS DEL COCHE
// ======================================

void moveForward() {

  setMotor(motorA, -1, 180);
  setMotor(motorB, -1, 180);
  setMotor(motorC, -1, 180);
  setMotor(motorD, -1, 180);

  Serial.println("FORWARD");
}

void moveLeft() {

  setMotor(motorA, -1, 170);
  setMotor(motorB, 1, 170);

  setMotor(motorC, -1, 170);
  setMotor(motorD, 1, 170);

  Serial.println("LEFT");
}

void moveRight() {

  setMotor(motorA, 1, 170);
  setMotor(motorB, -1, 170);

  setMotor(motorC, 1, 170);
  setMotor(motorD, -1, 170);

  Serial.println("RIGHT");
}

void stopCar() {

  setMotor(motorA, 0, 0);
  setMotor(motorB, 0, 0);
  setMotor(motorC, 0, 0);
  setMotor(motorD, 0, 0);

  Serial.println("STOP");
}

// ======================================
// LOOP PRINCIPAL
// ======================================

void loop() {

  if (Serial.available()) {

    String comando = Serial.readStringUntil('\n');

    comando.trim();

    // ==================================
    // COMANDOS DESDE PYTHON
    // ==================================

    if (comando == "FORWARD") {

      moveForward();
    }

    else if (comando == "LEFT") {

      moveLeft();
    }

    else if (comando == "RIGHT") {

      moveRight();
    }

    else if (comando == "STOP") {

      stopCar();
    }

    else {

      Serial.println("Comando no reconocido");
    }
  }
}