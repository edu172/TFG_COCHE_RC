// Pines de conexión de los 4 motores
struct Motor {
  int in1;
  int in2;
  int en;
};

// Motores
Motor motorA = {26, 25, 27}; //alante derecha
Motor motorB = {14, 12, 13}; //alante izquierda
Motor motorC = {18, 5, 19}; //atras derecha
Motor motorD = {4, 0, 2}; //atras izquierda

void setup() {
  Serial.begin(9600);
  Serial.println("Introduce w (adelante), s (atrás), p (parar), q/e (girar)");

  // Configuración de pines
  setupMotor(motorA);
  setupMotor(motorB);
  setupMotor(motorC);
  setupMotor(motorD);
}

void setupMotor(Motor m) {
  pinMode(m.in1, OUTPUT);
  pinMode(m.in2, OUTPUT);
  pinMode(m.en, OUTPUT);
}

void setMotor(Motor m, int direction, int speed) {
  // direction: 1 = adelante, -1 = atrás, 0 = parado
  speed = constrain(speed, 0, 255);

  if (direction == 1) {
    digitalWrite(m.in1, HIGH);
    digitalWrite(m.in2, LOW);
  } else if (direction == -1) {
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, HIGH);
  } else {
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, LOW);
  }

  analogWrite(m.en, speed); // Control de velocidad PWM
}

void loop() {
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "w") {
      // Adelante
      setMotor(motorA, -1, 255);
      setMotor(motorB, -1, 255);
      setMotor(motorC, -1, 255);
      setMotor(motorD, -1, 255);
      Serial.println("Adelante");
    } 
    else if (comando == "s") {
      // Atrás
      setMotor(motorA, 1, 200);
      setMotor(motorB, 1, 200);
      setMotor(motorC, 1, 200);
      setMotor(motorD, 1, 200);
      Serial.println("Atrás");
    } 
    else if (comando == "p") {
      // Parar
      setMotor(motorA, 0, 0);
      setMotor(motorB, 0, 0);
      setMotor(motorC, 0, 0);
      setMotor(motorD, 0, 0);
      Serial.println("Parado");
    }
    else if (comando == "q") {
      // Giro izquierda
      setMotor(motorA, -1, 180);
      setMotor(motorB, 1, 180);
      setMotor(motorC, -1, 180);
      setMotor(motorD, 1, 180);
      Serial.println("izquierda");
    }
    else if (comando == "e") {
      // Giro derecha
      setMotor(motorA, 1, 180);
      setMotor(motorB, -1, 180);
      setMotor(motorC, 1, 180);
      setMotor(motorD, -1, 180);
      Serial.println("derecha");
    }
    else {
      Serial.println("Escribelo bien guaje");
    }
  }
}
