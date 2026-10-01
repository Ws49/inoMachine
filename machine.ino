
//ins
const size_t LEN_OP_P = 100;
uint16_t memory[LEN_OP_P];
    
char input_buffer [400];
size_t quantity_operations;

//outs
short F0 = 10;
short F1 = 11;
short F2 = 12;
short F3 = 13;
short PIN_SP = 9; 
short EP = 8;

void setup(){
	Serial.begin(9600);
	Serial.write("Iniciando a Machine...\n");
  	Serial.setTimeout(1000);
	
	pinMode(F0, OUTPUT);
	pinMode(F1, OUTPUT);
	pinMode(F2, OUTPUT);
	pinMode(F3, OUTPUT);
	pinMode(PIN_SP, OUTPUT);
	pinMode(EP, OUTPUT);
	
	digitalWrite(F0, LOW);
	digitalWrite(F1, LOW);
	digitalWrite(F2, LOW);
	digitalWrite(F3, LOW);
	digitalWrite(PIN_SP, LOW);
	digitalWrite(EP, LOW);

	for(int i =0; i < LEN_OP_P; i++){
		memory[i] = 0;
	}
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

    unsigned short index = 4;
	char * token = strtok(input_buffer," ");
	while(token && index <= 99){
		uint16_t nibble2 = 0x0000;
		uint16_t nibble = 0x0000;

		memory[index] = toHex(token[0]);
		memory[index] = memory[index] << 8;

        nibble2 = toHex(token[1]);
		nibble2 = nibble2 << 4;

        nibble = toHex(token[2]);

		memory[index] += nibble2;
		memory[index] += nibble;

        quantity_operations++;
        index++;

		token = strtok(NULL, " ");
	}

} 

void printHex3(uint16_t value) {
    if (value < 0x100){
		Serial.print("0");
	}
	
	if (value < 0x10)  {
		Serial.print("0");
	}

    Serial.print(value, HEX);
}

void show_program(){
	Serial.print(">> ");
	printHex3(memory[0]);
	Serial.print(" | ");

	printHex3(memory[1]);
	Serial.print(" | ");

	printHex3(memory[2]);
	Serial.print(" | ");

	printHex3(memory[3]);
	Serial.print(" | ");

	
    for(int i = 0; i < quantity_operations; i++){
      	printHex3(memory[i + 4]);
        Serial.print(" | ");
    }

    Serial.println();
}

uint8_t exec_operation(uint8_t index_operation){
	
	uint8_t result = 0;

	switch (index_operation)
	{
	case 0:
		result = nA(memory[2]);
		break;
	case 1:
		result = AoBn(memory[2],memory[3]);
		break;
	case 2:
		result = nAeB(memory[2],memory[3]);
		break;			
	case 3:
		result = zeroL();
		break;
	case 4:
		result = AeBn(memory[2],memory[3]);
		break;
	case 5:
		result = nB(memory[3]);
		break;						
	case 6:
		result = AxB(memory[2],memory[3]);
		break;
	case 7:
		result = AenB(memory[2],memory[3]);
		break;
	case 8:
		result = nAoB(memory[2],memory[3]);
		break;	
	case 9:
		result = AxBn(memory[2],memory[3]);
		break;
	case 10:
		result = copia(memory[3]);
		break;									
	case 11:
		result = AeB(memory[2],memory[3]);
		break;									
	case 12:
		result = umL();
		break;													
	case 13:
		result = AonB(memory[2],memory[3]);
		break;									
	case 14:
		result = AoB(memory[2],memory[3]);
		break;									
	case 15:
		result = copia(memory[2]);
		break;									
																	
	default:
		break;
	}

	return result;
}

void show_leds(){ 
    digitalWrite(F3, (memory[1] & 0x8) ? HIGH : LOW);
    digitalWrite(F2, (memory[1] & 0x4) ? HIGH : LOW);
    digitalWrite(F1, (memory[1] & 0x2) ? HIGH : LOW);
    digitalWrite(F0, (memory[1] & 0x1) ? HIGH : LOW);
}

void loop(){
	Serial.write("Aguardando carga de dados: \n");
    quantity_operations = 0;
	memory[0] = 4;
	memory[1] = 0;
	memory[2] = 0;
	memory[3] = 0;


	digitalWrite(F0, LOW);
	digitalWrite(F1, LOW);
	digitalWrite(F2, LOW);
	digitalWrite(F3, LOW);
	digitalWrite(PIN_SP, LOW);
	digitalWrite(EP, LOW);

	while (Serial.available() == 0);
	size_t n = Serial.readBytesUntil('\n', input_buffer, sizeof(input_buffer) - 1);
    input_buffer[n] = '\0';
	
	load_program();
	show_program();
    
    Serial.println("Programa carregado com sucesso!");

	digitalWrite(PIN_SP, HIGH);
	do{
    	delay(4000);
		digitalWrite(PIN_SP, LOW);
		memory[2] = (memory[memory[0]] & 0x0F00) >> 8;
		memory[3] = (memory[memory[0]] & 0x00F0) >> 4;
		
		memory[1] = exec_operation(memory[memory[0]] & 0x000F);
		show_program();
		show_leds();

		memory[0]++;
	}while(memory[0] - 4 < quantity_operations);

	digitalWrite(EP, HIGH);
	Serial.println("Programa Encerrado!");
	delay(4000);

}

uint8_t nA(uint8_t a){
	return ( ~a) & 0x0F;
}
uint8_t AoBn(uint8_t a, uint8_t b){
	return ( ~(a+b)) & 0x0F;
}

uint8_t nAeB(uint8_t a, uint8_t b){
	return ( ~a&b) & 0x0F;
}
uint8_t zeroL(){
	return ( 0) & 0x0F;
}
uint8_t AeBn(uint8_t a, uint8_t b){
	return ( ~(a&b)) & 0x0F;
}
uint8_t nB(uint8_t b){
	return ( ~b) & 0x0F;
}
uint8_t AxB(uint8_t a,uint8_t b){
	return ( a^b) & 0x0F;
}
uint8_t AenB(uint8_t a,uint8_t b){
	return ( a&~b) & 0x0F;
}

uint8_t nAoB(uint8_t a, uint8_t b){
    return ( ~a|b) & 0x0F;
}

uint8_t AxBn(uint8_t a, uint8_t b){
    return ( (a&b) + (~a & ~b)) & 0x0F;
}

uint8_t AeB(uint8_t a, uint8_t b){
    return ( a&b) & 0x0F;
}

uint8_t umL(){
    return ( 1) & 0x0F;
}

uint8_t AonB(uint8_t a, uint8_t b){
    return ( a & ~b) & 0x0F;
}

uint8_t AoB(uint8_t a, uint8_t b){
    return ( a|b) & 0x0F;
}

uint8_t copia(uint8_t x){
    return ( x) & 0x0F;
}

