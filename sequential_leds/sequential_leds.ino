// Configuracion
int pulsador = 2;       // El pin del pulsador
int primerLed = 3;      // El pin del primer LED
int numeroDeLedes = 3;  // Cuantos LEDes tenemos en secuencia

int velocidad = 1000;  // Milisegundos para un ciclo completo

void setup() {
  pinMode(pulsador, INPUT);
  for (
    int ledActual = primerLed;
    ledActual <= (numeroDeLedes + primerLed - 1);
    ledActual += 1
  ) {
    pinMode(ledActual, OUTPUT);
  }
}

void loop() {
  for (
    int ledActual = primerLed;
    ledActual <= (numeroDeLedes + primerLed - 1);
    ledActual += 1
  ) {
    // Led previo
    digitalWrite(
        (ledActual - 1) < primerLed 
        ? primerLed + numeroDeLedes - 1
        : ledActual - 1,
      LOW);
      
    if (digitalRead(pulsador) == LOW) {
      break; 
    }
    // Led actual
    digitalWrite(ledActual, HIGH);

    delay(velocidad / numeroDeLedes);
  }
}