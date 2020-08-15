//Sensor de temperatura usando o LM35
 
const int LM35 = A0; // Define o pino que lera a saída do LM35
float temperatura; // Variável que armazenará a temperatura medida
int tempInt;
 
//Função que será executada uma vez quando ligar ou resetar o Arduino
void setup() {
Serial.begin(115200); // inicializa a comunicação serial
}
 
//Função que será executada continuamente
void loop() {

temperatura = (float(analogRead(LM35))*3.3/(1023))/0.01;
tempInt = temperatura * 100;

Serial.print("Temperatura: ");
Serial.print(tempInt/100);
Serial.println(" ºC");

delay(2000);
}
