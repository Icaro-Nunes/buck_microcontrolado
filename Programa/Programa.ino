/*
 Input Pullup Serial
 
 This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a 
 digital input on pin 2 and prints the results to the serial monitor.
 
 The circuit: 
 * Momentary switch attached from pin 2 to ground 
 * Built-in LED on pin 13
 
 Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal 
 20K-ohm resistor is pulled to 5V. This configuration causes the input to 
 read HIGH when the switch is open, and LOW when it is closed. 
 
 created 14 March 2012
 by Scott Fitzgerald
 
 http://www.arduino.cc/en/Tutorial/InputPullupSerial
 
 This example code is in the public domain
 
 */
 

unsigned char valor = 127;

//int teste = 0;

int ciclos = 0;

int lastRead = 0;

void setup(){
  //start serial connection
  Serial.begin(9600);
  
  DDRC = 0;
  DDRB = B11111111;

  DDRD = B11111111;
}

void loop(){
  if(ciclos == 10){
    ciclos = 0;
    char leitura = PINC;
    
    
    if(leitura & lastRead){
      if (leitura & B00000001) {
        if(valor <255){
          valor++;
        }
        
        PORTD = valor;
      } 
      
      if(leitura & B00000010) {
        
        if(valor>0){
          valor--;
        }
        
        PORTD = valor;
        
        analogWrite(9, valor);
      }
    }
  //  Serial.println(teste);
    Serial.println(valor, BIN);
    
    lastRead = leitura;
  }

    ciclos++;
}
