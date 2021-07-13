/*
Sobre o circuito:
  -Porta D contem a saida para os displays
  -Pino B1 contem a saida de pwm, correspondendo ao codigo de pino 9 para o analogwrite
  -Pinos C0 e C1 sao as entradas dos botoes de diminuir e aumentar, respectivamente, para configurar o pwm
  -Pinos C4, C5 e C6 sao as saidas de multiplexacao do display - centena, dezena e unidade, respectivamente
  -C2 e C3 sao os inputs analogicos de tensao de entrada e de saida do buck, respectivamente
*/

//valor do duty cycle
int valor = 127;


float absoluto;

char valor_display[4];

unsigned char la_tensao = 0;

bool el_decididor = false;

char outputDisplay(char output, bool com_ponto){
  char mostrar = 0;
  
  switch(output){
    case '-':
      mostrar = 0b01000000;
      break;
    case 0:
      mostrar = 0b00111111;
      break;
    case 1:
      mostrar = 0b00000110;
      break;
    case 2:
      mostrar = 0b01011011;
      break;
    case 3:
      mostrar = 0b01001111;
      break;
    case 4:
      mostrar = 0b01100110;
      break;
    case 5:
      mostrar = 0b01101101;
      break;
    case 6:
      mostrar = 0b01111101;
      break;
    case 7:
      mostrar = 0b00000111;
      break;
    case 8:
      mostrar = 0b01111111;
      break;
    case 9:
      mostrar = 0b01101111;
      break;
    default: break;
  }

  if(com_ponto != 0){
    mostrar |= 0b10000000;
  }

  return mostrar;
}

void float_to_output(float value, char * string){
  //12,37
  char dezena =((int)value/10);
  //12,37 -> 12/10 = 1
  
  char unidade = (int)(value - (dezena*10));
  //12,37 -10  = 2,37 = 2
  
  char decimal = (int)((value - ((dezena*10) +unidade))*10);
  //12,37 - (10 + 2) = 0,37 -> 0,37 * 10 = 3,7 = 3

  if(dezena != 0){
    *string = outputDisplay(dezena, 0);
  } else {
    *string = outputDisplay('-', 0);
  }
  
  *(string+1) = outputDisplay(unidade, 1);
  
  *(string+2) = outputDisplay(decimal,0);
  
  *(string+3) = 0;
}

int lastRead = 0;

void setup(){
  //start serial connection
  //Para debug
  Serial.begin(9600);

  //Declarando pinos da porta C
  DDRC = B01110000;


  //Declarando pinos da porta B como saida
  DDRB = B11;

  //Declarando pinos da porta D como saida - Display
  DDRD = B11111111;


  TCCR1A |= (1<<7)|(1<<0); //COM1A1 and WGM10 set
  TCCR1B |= (1<<3)|(1<<1); //WGM12 and CS11 set

  OCR1AL = valor;


  //Setting of ADC on ADC3
  ADMUX |= (1<<5)|(1<<1)|(1<<0); //ADLAR set, ADC3 selected
  ADCSRA |= (1<<7)|(1<<3)|(1<<0); //ADEN, ADIE and ADPS0 set
  DIDR0 |= (1<<3); //ADC3D set
  ADCSRA |= (1<<6); //ADSC set
}


void loop(){
  if(el_decididor == false){
    unsigned char leitura = PINC;
    
    leitura = ~leitura;
    
    //Checa se o botao estava pressionado no ultimo ciclo
    if(1){
      
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

      //Seta o pwm
      OCR1AL = valor; //analogWrite(9, valor);       Output Compare Register for Timer 1 - out pin A - Lower Byte
    }
    
    //debug
    Serial.print("Leitura: ");
    Serial.println(leitura, BIN);
    
    Serial.print("Valor: ");
    Serial.println(valor, DEC);
    
    //Entrega esta leitura a variavel lastRead
    lastRead = leitura;
  } else {
    absoluto = 5.0 * (float)la_tensao/255;
    
    float_to_output(absoluto, valor_display); // funcao (float, array_de_char);
    
    PORTD = valor_display[0];
    PORTC |= (1<<4); // 1<<4 = 00010000
    PORTC &= ~(1<<4); // ~(00010000) = 11101111

    PORTD = valor_display[1];
    PORTC |= (1<<5);
    PORTC &= ~(1<<5);

    PORTD = valor_display[2];
    PORTC |= (1<<6);
    PORTC &= ~(1<<6);
  }
  
  el_decididor = !el_decididor;
}

ISR(ADC_vect){
  la_tensao = ADCH;
  ADCH = 0;
  ADCSRA |= (1<<6); //ADSC set
}
