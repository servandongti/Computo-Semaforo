#include<Wire.h>
 bool estadorojo =0;
int ecoPin = 5;
int triggerPin = 4;

int duracion;
int distancia;
unsigned long TiempoA;
unsigned long TiempoB;
//VARIABLES AUXILIARES
byte aux1 = 0;
byte aux2 = 0;
bool estado = 0;

byte verde_2 = 13;
byte amarillo_2 = 12;
byte rojo_2 = 11;

byte almacenaje;

byte almacenaje2;
void setup() {
  Serial.begin(9600);
  pinMode(verde_2, OUTPUT);
  pinMode(amarillo_2, OUTPUT);
  pinMode(rojo_2, OUTPUT);
  Wire.begin(1);
  Wire.onReceive(dato);

  digitalWrite(rojo_2, 1);

}

void loop() {
  medirDistancia();

  delay(30);
  dato();

}
void dato() {

  if (Wire.available() == 1) {
    almacenaje = Wire.read();
    if (almacenaje == 2 && almacenaje2 == 0) {
      if (almacenaje2 == 0) {
        digitalWrite(rojo_2, 0);
      }
      almacenaje2 = 1;


      digitalWrite(verde_2, 1);

      TiempoA = millis();
    }
    if (almacenaje2 == 1) {
      //semaforo 1----------
      semaforo_verde();

      //semaforo 2-------------
      semaforo_amarillo();

      //semaforo 3-------------
      semaforo_rojo();

    }
  }
}

void semaforo_verde() {



  if ((millis() - TiempoA >= 6000) && aux1 == 0) {// 6000 segundos
    TiempoA =  millis();
    digitalWrite(verde_2, 0);
    aux1 = 1;
  }
  if (aux1 == 1) {

    Wire.write(1);//rojo esclavo
    if (millis() - TiempoA >= 500) {
      TiempoA =  millis();
      estado = !estado;
      digitalWrite(verde_2, estado);
      aux2++;
      if (aux2 == 6) {
        aux1 = 2;

      }
    }
  }
}

void semaforo_amarillo() {
  if (aux1 == 2) {
    if (aux2 == 6) {
      digitalWrite(amarillo_2, 1);
      aux2 = 7;
    }
    if ((millis() - TiempoA >= 1000) && aux2 == 7) {// 6000 segundos
      TiempoA =  millis();
      digitalWrite(amarillo_2, 0);
      aux2 = 8;
      aux1 = 3;
    }
  }
}

void semaforo_rojo() {
  if (aux1 == 3) {


    if (aux2 == 8) {
      digitalWrite(rojo_2, 1);
      TiempoA = millis();
      aux2 = 9;
    }
    if ((millis() - TiempoA >= 10000) && aux2 == 9) {// 6000 segundos
      TiempoA =  millis();
      digitalWrite(rojo_2, 1);
      aux2 = 10;
      //aux1 = 4;

    }

    if ((millis() - TiempoA >= 500) && aux2 == 10) {
      TiempoA =  millis();
      estadorojo = !estadorojo;
      digitalWrite(rojo_2, estadorojo);
    }
      
    }
  }




  int medirDistancia() {
    digitalWrite(triggerPin, 1);
    delay(1);
    digitalWrite(triggerPin, 0);
    duracion = pulseIn(ecoPin, HIGH);
    distancia = duracion / 58.2;

    Serial.println(distancia);

  }
