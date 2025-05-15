/* --------------------------------------------------------------------------------------------------------------------------------------------------------

Este software é protegido por direitos autorais e leis internacionais. Qualquer cópia não autorizada, distribuição ou uso deste software, total ou parcial,
será considerada uma violação dos direitos autorais e sujeita a medidas legais. 
Conforme estipulado pela Lei de Direitos Autorais, Lei nº 9.610/98, a pirataria de software é estritamente proibida e sujeita a penalidades legais. A cópia
não autorizada deste software constitui uma violação dos direitos de propriedade intelectual, passível de processo civil e criminal.
Ressaltamos que qualquer tentativa de reprodução, distribuição ou uso não autorizado deste software será monitorada e tratada com rigor dentro dos limites 
da lei.

-------------------------------------------------------------------------------------------------------------------------------------------------------- */

#include <IRremote.h>              // Biblioteca do sistema IR.
#include <RCSwitch.h>              // Biblioteca do sistema RF.
#include <EEPROM.h>                // Biblioteca EEPROM (Memoria).
  
// --------------------------------------------------------------------------------------------------------------------------------------------------------  

#define MotorSubir 4               // Acionamento para o Motor Subir.
#define MotorDescer 11             // Acionamento para o Motor Descer.

#define FreioMotor 17              // Acionamento para o Freio Motor.

#define SensorAbre 5               // Fim de curso que identifica o Lift Aberto.
#define SensorFecha 10             // Fim de curso que identifica o Lift Fechado.

int RECV_PIN = 16;                 // Pino IR.

int MotorS = HIGH;                 // Estado inicial do MotorS = MotorSubir (4).
int MotorD = HIGH;                 // Estado inicial do MotorD = MotorDescer (11).

int FreioM = HIGH;                 // Estado inicial do FreioM = FreioMotor (17).

int TMotor = 0;                    // Estado inicial do TMotor.

int ultimoCmdTil = 0;

// --------------------------------------------------------------------------------------------------------------------------------------------------------  

unsigned long int tempobotao = 0;             // Variavel que guarda o momento. 
unsigned long int TempoAbreFecha = 0;         // Variavel que guarda o momento.

unsigned long int TBotao = 0;                 // Variavel que guarda o momento do botão.
unsigned long int TAbrirFechar = 0;           // Variavel que guarda o momento do "Abrir" e "Fechar" do sistema.

unsigned long int IgnoreFC = 0;               // Variável para armazenar o tempo do último comando do fim de curso.

unsigned long tempoMotorAtivado = 0;          // Variável para armazenar o tempo de ativação do motor.

bool motorEmMovimento = false;                // Flag para indicar se o motor está em movimento.

// --------------- IR --------------------------------------------------- IR  --------------------------------------------------- IR ----------------------

float armazenavalor;                          // Variavel valor do IR.

IRrecv irrecv(RECV_PIN);
decode_results results;

// --------------- RF --------------------------------------------------- RF  --------------------------------------------------- RF ----------------------

RCSwitch mySwitch = RCSwitch();

// -------------------------------------------------------------------------------------------------------------------------------------------------------- 

void setup()
{
  pinMode(MotorSubir, OUTPUT);          // Define o pino que vai para o motor como uma saida.
  pinMode(MotorDescer, OUTPUT);         // Define o pino que vai para o motor como uma saida.
  
  pinMode(FreioMotor, OUTPUT);          // Define o pino que vai para o freio motor como uma saida.
  
  pinMode(SensorAbre, INPUT);           // Define o pino que vai para o fim de curso "abrerto" como uma entrada.
  pinMode(SensorFecha, INPUT);          // Define o pino que vai para o fim de curso "fechado" como uma entrada.         

  digitalWrite(MotorSubir, MotorS);     // Define o estado inicial do Motor com acionamento para Subir.
  digitalWrite(MotorDescer, MotorD);    // Define o estado inicial do Motor com acionamento para Descer.
  digitalWrite(FreioMotor, FreioM);     // Define o estado inicial do Freio Motor.
  

  irrecv.enableIRIn();                  // Inicializa o receptor IR.

  mySwitch.enableReceive(0);            // Indica que a interrupção 0 está associada ao pino 2 do microcontrolador.

  Serial.begin(115200);                 // Taxa de atualização da Serial.
} 

// --------------------------------------------------------------------------------------------------------------------------------------------------------  

void loop() {
 Sensores();
 
// --------------- LOOP DO SERIAL --------------------------------------------------- LOOP DO SERIAL ------------------------------------------------------
// Utilizado apenas ao conectar no computador!

      if (Serial.available() > 0) {
        char letra = Serial.read();
            if (letra == 'i'){                 // Comando para "SUBIR" o LIFT via SERIAL.
                    Subir();
                    Serial.print("Comando para SUBIR o LIFT via SERIAL foi acionado");
                    
            } else if (letra == 'o'){          // Comando para "DESCER" o LIFT via SERIAL.
                    Descer();
                    Serial.print("Comando para DESCER o LIFT via SERIAL foi acionado"); 
                    
            } else if (letra == 'p'){          // Comando para "PARAR" o LIFT via SERIAL.
                    PararGeral();
                    Serial.print("Comando para PARAR o LIFT via SERIAL foi acionado");
       } 
}
// --------------- LOOP DO RF --------------------------------------------------- LOOP DO RF --------------------------------------------------------------

    if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();
     if (value == 0){
     Serial.print("Codigo desconhecido");
     } else {
      
      if (mySwitch.getReceivedValue() == 1200041) {             // Comando para "SUBIR" o LIFT via RF.
           if (millis() - TBotao >= 500){
               Subir();  
           }   
      } else if (mySwitch.getReceivedValue() == 1200042) {      // Comando para "DESCER" o LIFT via RF.
           if (millis() - TBotao >= 500){
               Descer();     
           }
      } else if (mySwitch.getReceivedValue() == 1200043) {      // Comando para "PARAR" o LIFT via RF.
           if (millis() - TBotao >= 500){
               PararGeral();
           }     
      }        
   }
         
    Serial.print("Recebido");
    Serial.println( mySwitch.getReceivedValue() ); //Informa o valor do codigo RF.
    
    mySwitch.resetAvailable();
  }

// --------------- LOOP DO IR --------------------------------------------------- LOOP DO IR -------------------------------------------------------------- 
 
  if (irrecv.decode(&results)) {
    
    Serial.print("Valor lido : ");
    Serial.println(results.value, HEX);
    armazenavalor = (results.value);
    
    if (armazenavalor == 0x202708F || armazenavalor == 0x6EDFE961)            // Comando para "SUBIR" o LIFT via IR.
    {
      if (millis() - TBotao >= 500){
          Subir();
      }
    }
    
    else if (armazenavalor == 0x202D02F || armazenavalor == 0x76B366E3 )      // Comando para "DESCER" o LIFT via IR.
    {
      if (millis() - TBotao >= 500){
          Descer();
      }
    }
    
    else if (armazenavalor == 0x923F150B || armazenavalor == 0x202B04F)       // Comando para "PARAR" o LIFT via IR.
    {
      if (millis() - TBotao >= 500){
          PararGeral();
      }      
    }
    
    irrecv.resume();
  }
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------- 

// Usado para fazer o motor ligar no sentido de SUBIR o equipamento.
void Subir() {
             
 TAbrirFechar = millis();
 TBotao = millis();
 IgnoreFC = millis();  // Inicia o temporizador para ignorar o fim de curso por 1125ms.

 FreioM = LOW;
 digitalWrite(FreioMotor, FreioM);

 MotorS = LOW;       // Acionado o Motor Subir.                
 MotorD = HIGH;      // Desacionado o Motor Descer.
 
 delay(800); // Tempo de internalo entre o acionamento do motor apor o freio ser liberado.

 digitalWrite(MotorSubir, MotorS); 
 digitalWrite(MotorDescer, MotorD);                    

  TMotor = 1;

 motorEmMovimento = true;          // Indica que o motor está em movimento.
 tempoMotorAtivado = millis();     // Marca o tempo de ativação do motor.
             
  Serial.println("Comando para SUBIR o LIFT foi acionado");     
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------- 

// Usado para fazer o motor ligar no sentido de DESCER o equipamento.
void Descer() {
             
 TAbrirFechar = millis();
 TBotao = millis();
 IgnoreFC = millis();  // Inicia o temporizador para ignorar o fim de curso por 1125ms.

 FreioM = LOW;
 digitalWrite(FreioMotor, FreioM);

 MotorS = HIGH;     // Desacionado o Motor Subir.                              
 MotorD = LOW;      // Acionado o Motor Descer.   
 
 delay(800); // Tempo de internalo entre o acionamento do motor apor o freio ser liberado.

 digitalWrite(MotorSubir, MotorS);
 digitalWrite(MotorDescer, MotorD);                           

  TMotor = 1;

 motorEmMovimento = true;          // Indica que o motor está em movimento.
 tempoMotorAtivado = millis();     // Marca o tempo de ativação do motor.
             
  Serial.println("Comando para DESCER o LIFT foi acionado");       
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------- 

// Usado para parar toda a logica de funcionamento.
void PararGeral() {
 tempobotao = millis();

 MotorS = HIGH;     // Desacionado o Motor Subir.                             
 MotorD = HIGH;     // Desacionado o Motor Descer.                              
 FreioM = HIGH;     // Desacionado o Freio Motor. 
         
  digitalWrite(MotorSubir, MotorS);               
  digitalWrite(MotorDescer, MotorD);               
  digitalWrite(FreioMotor, FreioM);

 TMotor = 0;
 
 motorEmMovimento = false;  // Para indicar que o motor não está mais em movimento.

 Serial.println("Comando para PARAR o LIFT foi acionado");
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

// Usado para parar toda a logica de funcionamento atravez dos fim de curso.
void ParadaFC() {
 tempobotao = millis();

 MotorS = HIGH;     // Desacionado o Motor Subir.                             
 MotorD = HIGH;     // Desacionado o Motor Descer.                              
 FreioM = HIGH;     // Desacionado o Freio Motor. 
         
  digitalWrite(MotorSubir, MotorS);               
  digitalWrite(MotorDescer, MotorD);               
  digitalWrite(FreioMotor, FreioM);

 TMotor = 0;

 motorEmMovimento = false;  // Para indicar que o motor não está mais em movimento.

 //Serial.println("Foi encerrado o funcionamento pelo fim de curso");
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

// Usado para controlar e saber o estado dos dois Fim de Curso, seja eles o Aberto, quanto o Fechado.
void Sensores() { 
    
       // Ignora o funcionamento dos sensores de fim de curso por 1125ms após o acionamento de um comando.
 if (millis() - IgnoreFC > 1125) {

       // Verifica o estado do Fim de curso ABERTO.
  if (digitalRead(SensorAbre) == LOW ) {
       //Serial.println("Fim de curso ABERTO está acionado");
        ParadaFC(); // Para se o sensor de ABERTURA for acionado.
       //delay(800);               
  }

       // Verifica o estado do Fim de curso FECHADO.
  if (digitalRead(SensorFecha) == LOW ) {
       //Serial.println("Fim de curso FECHADO está acionado");
        ParadaFC(); // Para se o sensor de FECHAMENTO for acionado.
       //delay(800);               
   }

         // Verifica se o motor está ligado por mais de 20 segundos sem acionar nenhum fim de curso.
  if (motorEmMovimento && (millis() - tempoMotorAtivado >= 20000)) {
    PararGeral();
    Serial.println("Motor foi desligado automaticamente após 20 segundos sem atingir o fim de curso.");
  
  }
 }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------
