
//ins
const size_t LEN_OP_P = 101;
uint8_t X_values[LEN_OP_P];
uint8_t Y_values[LEN_OP_P];
uint8_t operations[LEN_OP_P];
char input_buffer [400];
size_t quantity_operations;

//outs
short F0 = 10;
short F1 = 11;
short F2 = 12;
short F3 = 13;

void setup(){
	Serial.begin(9600);
	Serial.write("Iniciando a Machine...\n");
  	Serial.setTimeout(1000);
}

uint8_t toHex(char c){
	uint8_t result = 255;
	if(c >= '0' && c <= '9'){
		result = c - '0';
	}else if(c >= 'A' && c <= 'F'){
		result = c - 'A' + 10;
	}else if(c >= 'a' && c <= 'f'){
		result = c - 'a' + 10;
	}

	return result;
}

void load_program(){

    unsigned short index = 1;
	char * token = strtok(input_buffer," ");
	while(token && index <= 100){
		X_values[index] = toHex(token[0]);
        Y_values[index] = toHex(token[1]);
        operations[index] = toHex(token[2]);
        quantity_operations++;
        index++;
		token = strtok(NULL, " ");
	}
    quantity_operations++;
}

void show_program(){
    for(int i =0; i < quantity_operations; i++){
        Serial.print(X_values[i],HEX);
        Serial.print(Y_values[i],HEX);
        Serial.print(operations[i],HEX);
        Serial.print(" | ");
    }
    Serial.println();
}
void loop(){
	Serial.write("Aguardando carga de dados: \n");
    quantity_operations = 0;

	while (Serial.available() == 0);
	size_t n = Serial.readBytesUntil('\n', input_buffer, sizeof(input_buffer) - 1);
    input_buffer[n] = '\0';
    
	load_program();
	show_program();
    
    Serial.println("Programa carregado com sucesso!");
    Serial.println("Precione 1 para continuar com execução e 0 para interromper a execução!...");
}

short nA(short a){
	return ~a;
}
short AoBn(short a, short b){
	return ~(a+b);
}

short nAeB(short a, short b){
	return ~a&b;
}
short zeroL(){
	return 0;
}
short AeBn(short a, short b){
	return ~(a&b);
}
short nB(short b){
	return ~b;
}
short AxB(short a,short b){
	return a^b;
}
short AenB(short a,short b){
	return a&~b;
}

short nAoB(short a, short b){
    return ~a|b;
}

short AxBn(short a, short b){
    return (a&b) + (~a & ~b);
}

short copiaB(short a, short b){
    return b;
}

short AeB(short a, short b){
    return a&b;
}

short umL(short a, short b){
    return 1;
}

short AonB(short a, short b){
    return a & ~b;
}

short AoB(short a, short b){
    return a|b;
}

short copiaA(short a, short b){
    return a;
}

