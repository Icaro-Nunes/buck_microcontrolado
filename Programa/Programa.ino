
//valor do duty cycle
unsigned int valor = 127;

//unsigned char barra = 0;

//quantidade de loops
//int ciclos = 0;


int lastRead = 0;


void setup(){
  //start serial connection
  //Para debug
  Serial.begin(9600);
  
  //Data Direction Register for port C

  //Declarando pinos da porta C como entrada
  DDRC = 0; //B 0000 0000

  //Declarando pinos da porta B como saida
  DDRB = B11111111;

  //Declarando pinos da porta D como saida
  DDRD = B11111111;
}


void loop(){
//  if(ciclos == 10){
    
//    ciclos = 0;

    unsigned char leitura = PINC;
    
    //Checa se o botao estava pressionado no ultimo ciclo
    
    if(!(leitura == lastRead)){
      
      //Checa ce o botao em C0 esta pressionado
      if (leitura & B00000001) {
        
        if(valor <255){
          valor++;
        }

        //Leva o valor atualizado a porta D, que e a porta dos leds        
        PORTD = valor;

      } 
      
      //Checa ce o botao em C1 esta pressionado
      if(leitura & B00000010) {
        
        if(valor>0){
          valor--;
        }
        
        //Leva o valor atualizado a porta D, que e a porta dos leds
        PORTD = valor;
       
      }
      
      //Seta o pwm
      analogWrite(9, valor);
    }
    
    //debug
    Serial.println(valor, BIN);
    
    //Entrega esta leitura a variavel lastRead
    lastRead = leitura;

//  }

//    ciclos++;
}
