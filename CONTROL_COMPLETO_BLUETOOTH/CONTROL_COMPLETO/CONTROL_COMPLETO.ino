#include <DabbleESP32.h>

//Ultrasonidos pines
const int Trigger = 33;
const int Echo = 34;

// servo pines
const int servoPin = 23;
const int servoChannel = 0;
const int servoFreq = 50;
const int servoResolution = 16;

// Posicion inicial del servo
int posicionServo = 0;

// Estructura motores
struct Motor {
  int in1;
  int in2;
  int en;
};

// Motores pines
Motor motorA = {26, 25, 27};
Motor motorB = {14, 12, 13};
Motor motorC = {18, 5, 19};
Motor motorD = {4, 0, 2};

// Configuración Bluetooth
#define BLUETOOTH_NAME "DabbleESP32Car"


void setupMotor(Motor m) {
  pinMode(m.in1, OUTPUT);
  pinMode(m.in2, OUTPUT);
  pinMode(m.en, OUTPUT);
}

//Direcion motor
void setMotor(Motor m, int direction, int speed) {
  speed = constrain(speed, 0, 255);

  if (direction == 1) { // adelante
    digitalWrite(m.in1, HIGH);
    digitalWrite(m.in2, LOW);

  } else if (direction == -1) { // atrás
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, HIGH);

  } else { // parado
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, LOW);
  }

  analogWrite(m.en, speed);
}

//Funcion del servo
void moverServo(int angulo) {
  angulo = constrain(angulo, 0, 180);

  if (angulo == posicionServo) return;
  posicionServo = angulo;

  int pulso = map(angulo, 0, 180, 500, 2400);
  int duty = (pulso * 65535) / 20000;

  ledcWrite(servoChannel, duty);
}

// Medir distancia ultrasonidos
long medirDistancia() {
  digitalWrite(Trigger, LOW);
  delayMicroseconds(5);

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  long t = pulseIn(Echo, HIGH, 30000);
  long d = t / 59;
  return d;
}

void setup() {

  Dabble.begin(BLUETOOTH_NAME);

  setupMotor(motorA);
  setupMotor(motorB);
  setupMotor(motorC);
  setupMotor(motorD);

  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);

  // Configurar SERVO
  ledcSetup(servoChannel, servoFreq, servoResolution);
  ledcAttachPin(servoPin, servoChannel);

  // Posicion inicial del servo hacia delante
  moverServo(0); 
}


void loop() {

  Dabble.processInput();

  long distancia = medirDistancia();

  // Bloqueo avance por obstáculo
  bool obstaculoDelante = distancia > 0 && distancia < 12;

  if (GamePad.isUpPressed()) {

    // Cámara mirando delante
    moverServo(0);

    if (obstaculoDelante) {

      setMotor(motorA, 0, 0);
      setMotor(motorB, 0, 0);
      setMotor(motorC, 0, 0);
      setMotor(motorD, 0, 0);

    } else {

      setMotor(motorA, 1, 255);
      setMotor(motorB, 1, 255);
      setMotor(motorC, 1, 255);
      setMotor(motorD, 1, 255);
    }

  } else if (GamePad.isDownPressed()) {

    // Cámara mirando atrás
    moverServo(180);

    setMotor(motorA, -1, 255);
    setMotor(motorB, -1, 255);
    setMotor(motorC, -1, 255);
    setMotor(motorD, -1, 255);

  } else if (GamePad.isRightPressed()) {

    setMotor(motorA, 1, 255);
    setMotor(motorB, -1, 255);
    setMotor(motorC, -1, 255);
    setMotor(motorD, 1, 255);

  } else if (GamePad.isLeftPressed()) {

    setMotor(motorA, -1, 255);
    setMotor(motorB, 1, 255);
    setMotor(motorC, 1, 255);
    setMotor(motorD, -1, 255);

  } else {

    setMotor(motorA, 0, 0);
    setMotor(motorB, 0, 0);
    setMotor(motorC, 0, 0);
    setMotor(motorD, 0, 0);
  }

  delay(50);
}
