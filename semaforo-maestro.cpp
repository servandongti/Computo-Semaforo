#include <RTClib.h>

const int rojo = 11;
const int amarillo = 12;
const int verde = 13;
const int ecoPin = 5;
const int triggerPin = 4;
RTC_DS1307 rtc;
unsigned long lastDetectionTime = 0;
boolean isSensorActive = false;

enum TrafficLightState {
  Green,
  Yellow,
  Red
};

TrafficLightState currentState = Green;
unsigned long stateStartTime = 0;
unsigned long stateDuration = 0;

void setup() {
  rtc.begin();

  pinMode(verde, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(rojo, OUTPUT);

  pinMode(triggerPin, OUTPUT);
  pinMode(ecoPin, INPUT);
}

void loop() {
  semaforo_principal();
}

void semaforo_principal() {
  DateTime now = rtc.now();
  // Rest of the code...

  int distancia = medirDistancia();

  switch (currentState) {
    case Green:
      stateDuration = 6000; // Green light duration
      if (millis() - stateStartTime >= stateDuration) {
        parpadearLuz(verde, 500);
        currentState = Yellow;
        stateStartTime = millis();
      }
      break;

    case Yellow:
      stateDuration = 1000; // Yellow light duration
      if (millis() - stateStartTime >= stateDuration) {
        currentState = Red;
        stateStartTime = millis();
      }  
      break;

    case Red:
      isSensorActive = true;
      stateDuration = 500; // Red light duration
     
      if (isSensorActive && millis() - stateStartTime >= stateDuration && distancia <= 20) {
        currentState = Green;
        stateStartTime = millis();
      } 
      isSensorActive = false;
      break;
      
  }

  if (currentState == Red && !isSensorActive && millis() - stateStartTime >= 10000) {
      currentState = Yellow;
      while(currentState == Yellow) {
        digitalWrite(rojo, LOW);
        parpadearLuz(amarillo, 500);
      }
  }

  updateTrafficLights();
}
void updateTrafficLights() {
  digitalWrite(rojo, currentState == Red ? HIGH : LOW);
  digitalWrite(amarillo, currentState == Yellow ? HIGH : LOW);
  digitalWrite(verde, currentState == Green ? HIGH : LOW);
}

void cambiarLuz(int pin, unsigned long duracion) {
  digitalWrite(pin, HIGH);
  delay(duracion);
  digitalWrite(pin, LOW);
  delay(500);
}

void parpadearLuz(int pin, unsigned long duracion) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pin, HIGH);
    delay(duracion);
    digitalWrite(pin, LOW);
    delay(duracion);
  }
}

int medirDistancia() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  unsigned long duracion = pulseIn(ecoPin, HIGH);
  int distancia = duracion * 0.034 / 2;

  return distancia;
}