#include <Arduino.h>
#include <analogWrite.h>

int PinTrigger = 18; // Pino usado para disparar os pulsos do sensor
int PinEcho = 19; // pino usado para ler a saida do sensor
int vibracall = 25;
int buzzer = 23;

int freq = 2000;
int res = 8;
int TONE_PWM_CHANNEL = 1;

int tempo = 1000;
float TempEcho = 0;
//const float VelocidadeSom_mpors = 340; // em metros por segundo
const float VelocidadeSom_mporus = 0.000340; // em metros por microsegundo
float cm = 0;
// Funçao para enviar o pulso de trigger
void DisparaPulsoUltrassonico(){
  // Para fazer o HC-SR04 enviar um pulso ultrassonico, nos temos
  // que enviar para o pino de trigger um sinal de nivel alto
  // com pelo menos 10us de duraçao
  digitalWrite(PinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrigger, LOW);
  }
// Função para calcular a distancia em metros
float CalculaDistancia(float tempo_us){
  return((tempo_us*VelocidadeSom_mporus)/2);
}
void setup(){
  // Configura pino de Trigger como saída e inicializa com nível baixo
  pinMode(PinTrigger, OUTPUT);
  digitalWrite(PinTrigger, LOW);
  pinMode(PinEcho, INPUT); // configura pino ECHO como entrada

  pinMode (vibracall, OUTPUT);
  pinMode (buzzer, OUTPUT);

  ledcSetup(TONE_PWM_CHANNEL,freq, res);
  ledcAttachPin(buzzer, TONE_PWM_CHANNEL);
  
  // Inicializa a porta serial
  Serial.begin(9600);
  delay(100);
}
void loop(){
  // Envia pulso para o disparar o sensor
  
  DisparaPulsoUltrassonico();
  // Mede o tempo de duração do sinal no pino de leitura(us)
  TempEcho = pulseIn(PinEcho, HIGH);
  cm = CalculaDistancia(TempEcho)*100;

  Serial.println("Distancia em centimetros: ");
  Serial.println(cm);
  Serial.println("\n");
  analogWrite(vibracall,0); 
 

  //logica para lidar com as distancias - buzzer/vibracall
  if(cm <= 20){ 
    Serial.print("Obstaculo, cuidado!\n");
    analogWrite(vibracall,500);

    ledcWrite(TONE_PWM_CHANNEL, 10000);
    delay(100);    
    ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone 
       
  }
  // aguarda um pouco antes de começar tudo de novo
  delay(100);
}
