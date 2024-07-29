#include <SoftwareSerial.h>
#include <Kangaroo.h>

// Arduino TX (pin 11) goes to Kangaroo S1
// Arduino RX (pin 10) goes to Kangaroo S2
// Arduino GND         goes to Kangaroo 0V
// Arduino 5V          goes to Kangaroo 5V (OPTIONAL, if you want Kangaroo to power the Arduino)
#define TX_PIN 11
#define RX_PIN 10

// Independent mode channels on Kangaroo are, by default, '1' and '2'.
SoftwareSerial  SerialPort(RX_PIN, TX_PIN);
KangarooSerial  K(SerialPort);
KangarooChannel K1(K, '1');
KangarooChannel K2(K, '2');

void setup() {
  SerialPort.begin(9600);
  SerialPort.listen();
  
  K1.start();
  K1.home().wait();
  K2.start();
  K2.home().wait();
}

void loop() {
  long maximum = K1.getMax().value();
  long minimum = K2.getMax().value();
  long rev = 2*maximum;
  long speedLimit = (maximum - minimum) / 10;

  K1.streaming(true);
  K2.streaming(true);
  
  for (int i = 0; i < speedLimit; i++) {
    K1.s(i);
    K2.s(i);
  }
  K1.streaming(false);
  K2.streaming(false);
  
  long relativeP1 = K1.getP().value();
  long relativeP2 = K2.getP().value();

  long amountStillGo1 = rev - relativeP1;
  long amountStillGo2 = rev - relativeP2;
  
  K1.p(amountStillGo1, speedLimit);
  K2.p(amountStillGo2, speedLimit);
  delay(3000);

  K1.streaming(true);
  K2.streaming(true);
  
  for (int i = 0; i > -speedLimit; i--) {
    K1.s(i);
    K2.s(i);
  }
  K1.streaming(false);
  K2.streaming(false);

  relativeP1 = K1.getP().value();
  relativeP2 = K2.getP().value();

  amountStillGo1 = rev + relativeP1;
  amountStillGo2 = rev + relativeP2;
  
  K1.p(-amountStillGo1, -speedLimit);
  K2.p(-amountStillGo2, -speedLimit);
  delay(3000);
  
}
