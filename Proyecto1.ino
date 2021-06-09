//Dual axis solar tracker Arduino

//Giralsol
#include <Servo.h>

Servo horizontal;
Servo vertical;

int up_left; //Fotorresistor de arriba a la izquierda
int up_right; //Fotorresistor de arriba a la derecha
int botton_left;//Fotorresistor de abajo a la izquierda
int botton_right;//Fotorresistor de abajo a la derecha
int prom_right;//Promedio del valor de los fotorresistores a la derecha
int prom_left;//Promedio del valor de los fotorresistores a la izquierda
int prom_up;//Promedio del valor de los fotorresistores de arriba
int prom_botton;//Promedio del valor de los fotorresistores de abajo
int servovLimitHigh = 90;//Máximo valor de giro del servo de movimiento vertical
int servovLimitLow = 0;//Mínimo valor de giro del servo de movimiento vertical
int servohLimitHigh = 180;//Máximo valor de giro del servo de movimiento horizontal
int servohLimitLow = 0;//Mínimo valor de giro del servo de movimiento horizontal
int servov = 0;//Posición inicial servo de movimiento vertical
int servoh = 90;//Posición inicial servo de movimiento horizontal
int urX = 90;//Posición inicial servo de movimiento horizontal al activar el control manual
int urY = 0;//Posición inicial de servo de movimiento vertical al activar el control manual
int cont = 0;//Contador: Cuenta las veces que el botón central del joystick es presionado para cambiar el de control manual a automático o viceversa

void setup () {
  Serial.begin(9600);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  horizontal.attach(9);
  vertical.attach(10);
  horizontal.write(servoh);
  vertical.write(servoh);
  delay(1000);
}

void loop() {
  up_left = analogRead(A2);
  up_right = analogRead(A3);
  botton_left = analogRead(A4);
  botton_right = analogRead(A5);

  prom_right = (botton_right + up_right)/2;
  prom_left = (botton_left + up_left)/2;
  prom_up = (up_left + up_right)/2;
  prom_botton = (botton_left + botton_right)/2;
  int dvert = prom_up - prom_botton;
  int dhoriz = prom_right - prom_left;
  
  int dtime = 200; int tol = 300;
////////////////////// Control automático //////////////////
  if (digitalRead(5) != 1) {
    cont = ++cont;
  }
  if (cont%2 == 0){
    digitalWrite(6, LOW);
    //Servo vertical
    if (-1*tol > dvert || dvert > tol) {
      if (prom_up < prom_botton) {
        servov = servov -2;
         if (servov < servovLimitLow) {
          servov = servovLimitLow;
         }
      }
      else if (prom_up > prom_botton) {
        servov = servov +2;
        if (servov > servovLimitHigh) {
          servov = servovLimitHigh;
        }
      }
      vertical.write(servov);    
    }
    //Servo horizontal
    if (-1*tol > dvert || dvert < tol) {
      if (prom_right< prom_left) {
        servoh = servoh -2;  
        if (servoh < servohLimitLow) {
          servoh = servohLimitLow;  
        }
      }
      else if (prom_right > prom_left) {
        servoh = servoh +2; 
        if (servoh > servohLimitHigh) {
          servoh = servohLimitHigh;  
        } 
      }
      horizontal.write(servoh);
    }
  }
 ///////////////// Modo manual///////////////
 
 else { 
  digitalWrite(6, HIGH);
  //Servo vertical
  if (analogRead(1) <= 1023 && analogRead(1) >= 1000) {
    urY = urY +5;
    if (urY > servovLimitHigh) {
      urY = servovLimitHigh;  
    }
  }
  if (analogRead(1) <= 20 && analogRead(1) >= 0) {
    urY = urY -5;
    if (urY < servovLimitLow) {
     urY = servovLimitLow;  
    } 
  }
  vertical.write(urY);
  //Servo horizontal
  if (analogRead(0) <= 1023 && analogRead(0) >= 1000) {
    urX = urX -5;
    if (urX < servohLimitLow) {
      urX = servohLimitLow;  
    }  
  }
  if (analogRead(0) <= 20 && analogRead(0) >= 0) {
    urX = urX + 5;
    if (urX > servohLimitHigh) {
      urX = servohLimitHigh;  
    }
  }
  horizontal.write(urX);
 }
 
 delay(dtime);

}
