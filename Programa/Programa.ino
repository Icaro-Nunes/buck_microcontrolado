
//valor do duty cycle
int valor = 100;

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

    unsigned char leitura = PINC;
    
    leitura = leitura ^ 0xFF;
    
    //Checa se o botao estava pressionado no ultimo ciclo
    
    if(/*leitura != lastRead && leitura != 0*/1){
      
      //Checa se o botao em C0 esta pressionado
      if (leitura & B00000001) {
        
        if(valor <255){
          valor++;
        }

      } 
      
      //Checa se o botao em C1 esta pressionado
      if(leitura & B00000010) {
        
        if(valor>0){
          valor--;
        }
       
      }
      
      //Preparar barra de progresso
      unsigned char barra = 0;
      
      int divisao = (valor + 1)/32;
      Serial.print("Divisao: ");
      Serial.println(divisao, DEC);
      
      //unsigned int divisao = 3;
      
      for(int i = 0; i<divisao; i++){
        barra = barra << 1; // B00000000 -> B00000000
        barra += 1; // B0000
      }
      
      Serial.print("Barra: ");
      Serial.println(barra, BIN);
      
      //Leva o valor atualizado a porta D, que e a porta dos leds
      PORTD = ~barra;
      
      //Seta o pwm
      analogWrite(9, valor);
    }
    
    //debug
    Serial.print("Leitura: ");
    Serial.println(leitura, BIN);
    
    Serial.print("Valor: ");
    Serial.println(valor, DEC);
    
    //Entrega esta leitura a variavel lastRead
    lastRead = leitura;

}
