#include<Wire.h>
bool estadorojo = 0;
//TIEMPO
unsigned long TiempoA;
unsigned long TiempoB;

//VARIABLES AUXILIARES
byte aux1 = 0;
byte aux2 = 0;
bool estado = 0;
//semaforo 1 - maestro
byte verde_1 = 13;
byte amarillo_1 = 12;
byte rojo_1 = 11;

//SEMAFORO ESCLAVO
byte verde_2 = 0;
byte amarillo_2 = 0;
byte rojo_2 = 1;

void setup() {
  Serial.begin(9600);
  pinMode(verde_1, OUTPUT);
  pinMode(amarillo_1, OUTPUT);
  pinMode(rojo_1, OUTPUT);

  digitalWrite(verde_1, 1);
  Wire.begin();

  Wire.beginTransmission(1);
  Wire.write(1);
  Wire.endTransmission();
}

void loop() {
  //semaforo 1----------
  semaforo_verde();

  //semaforo 2-------------
  semaforo_amarillo();

  //semaforo 3-------------
  semaforo_rojo();
}
void semaforo_rojo() {
  if (aux1 == 3) {

    Wire.beginTransmission(1);
    Wire.write(2);
    Wire.endTransmission();

    if (aux2 == 8) {
      digitalWrite(rojo_1, 1);
      aux2 = 9;
    }
    if ((millis() - TiempoA >= 20000) && aux2 == 9) {// 6000 segundos
      TiempoA =  millis();
      digitalWrite(rojo_1, 0);
      aux2 = 10;
      //aux1 = 4;

    }
    if ((millis() - TiempoA >= 500) && aux2 == 10) {
      TiempoA =  millis();
      estadorojo = !estadorojo;
      digitalWrite(amarillo_1, estadorojo);
    }

  }
}



void semaforo_amarillo() {
  if (aux1 == 2) {
    if (aux2 == 6) {
      digitalWrite(amarillo_1, 1);
      aux2 = 7;
    }
    if ((millis() - TiempoA >= 1000) && aux2 == 7) {// 6000 segundos
      TiempoA =  millis();
      digitalWrite(amarillo_1, 0);
      aux2 = 8;
      aux1 = 3;
    }
  }
}
void semaforo_verde() {



  if ((millis() - TiempoA >= 6000) && aux1 == 0) {// 6000 segundos
    TiempoA =  millis();
    digitalWrite(verde_1, 0);
    aux1 = 1;
  }
  if (aux1 == 1) {

    Wire.write(1);//rojo esclavo
    if (millis() - TiempoA >= 500) {
      TiempoA =  millis();
      estado = !estado;
      digitalWrite(verde_1, estado);
      aux2++;
      if (aux2 == 6) {
        aux1 = 2;

      }
    }
  }
}
