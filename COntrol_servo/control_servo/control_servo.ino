const int servoPin = 18;  // Pin de señal del servo

// Configuración PWM
const int freq = 50;      
const int channel = 0;
const int resolution = 16; 

void setup() {
  Serial.begin(115200);

  // Configurar canal PWM
  ledcSetup(channel, freq, resolution);

  // Asignar canal al pin
  ledcAttachPin(servoPin, channel);

  Serial.println("Servo listo");
}

void moverServo(int angulo) {
  // Convertir ángulo (0–180) a pulso (500–2400 us)
  int pulso = map(angulo, 0, 180, 500, 2400);

  // Convertir microsegundos a duty cycle
  int duty = (pulso * 65535) / 20000; 
  // 20 ms = periodo de 50 Hz

  ledcWrite(channel, duty);
}

void loop() {

  // 0° → 180°
  for (int a = 0; a <= 180; a++) {
    moverServo(a);
    delay(15);
  }

  delay(1000);

  // 180° → 0°
  for (int a = 180; a >= 0; a--) {
    moverServo(a);
    delay(15);
  }

  delay(1000);
}
