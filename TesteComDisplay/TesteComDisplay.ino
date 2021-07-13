int entrada = 512; //0 a 1023

float valor; //0 a 5.0 V

char output_valor[4];

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

void setup(){
  DDRB = B11111111;
  PORTB = 0;
  DDRD = B11;
  PORTD = 0;
  Serial.begin(9600);
}

void loop(){
  valor = 5.0 * (float)entrada/1024;

  float_to_output(valor,output_valor);
  
  // output_valor == {representacao do - em display, representacao do 2 com ponto em display, representacao do 5 em display}

  PORTB = output_valor[1];
  PORTD = 1;
  PORTD = 0;

  PORTB = output_valor[2];
  PORTD = 2;
  PORTD = 0;
  
  Serial.println(valor, DEC);
  Serial.print("La string: ");
  Serial.println(output_valor);
  
  entrada++;
}
