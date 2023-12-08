/* NOMES:
  GABRIEL PALUDETO
  CARLOS EDUARDO ASSIS
  TARSILA PIMENTEL 
  ERIC DURIGON
*/

//pins do semáforo da via
#define VERMELHO_VIA 13
#define AMARELO_VIA 12
#define VERDE_VIA 11

//pins do semáforo e botão de pedestre
#define VERMELHO_PEDESTRE 10
#define VERDE_PEDESTRE 9
#define BOTAO_PEDESTRE 2

//variáveis para armazenar e comparar estados do botão
byte estadoBotao = 0;
byte estadoAnteriorBotao = 0;
bool emLatencia = false;
bool botaoLatencia = false;
unsigned long tempoInicioLatencia;
unsigned long millisInicio;
unsigned long millisAtual;

void setup() {
  pinMode(LED_BUILTIN, LOW);
  pinMode(VERDE_VIA, OUTPUT);
  pinMode(VERMELHO_VIA, OUTPUT);
  pinMode(AMARELO_VIA, OUTPUT);
  pinMode(VERMELHO_PEDESTRE, OUTPUT);
  pinMode(VERDE_PEDESTRE, OUTPUT);
  pinMode(BOTAO_PEDESTRE, INPUT);
  millisInicio = millis();
}

void loop() {

  //semáforo da via sempre verde, semáforo de pedestre sempre vermelho enquanto o botão não estiver apertado
  digitalWrite(VERDE_VIA, HIGH);
  digitalWrite(VERMELHO_PEDESTRE, HIGH);

  //variável que armazena o estado atual do botão
  estadoBotao = digitalRead(BOTAO_PEDESTRE);

  //se o estado atual do botão for HIGH e o estado anterior LOW, estadoAnterior armazena o estado atual e a função semáforo é chamada
  if (estadoBotao == LOW && estadoAnteriorBotao == HIGH && emLatencia == false) {
    estadoAnteriorBotao = estadoBotao;
    semaforo();
  }

  //checa se botão foi pressionado durante modo de latência
  if (estadoBotao == LOW && estadoAnteriorBotao == HIGH && emLatencia == true) {
    if (digitalRead(BOTAO_PEDESTRE) == LOW) {
      botaoLatencia = true;
    }
  }

  //se está em modo de latência e o intervalo de tempo foi maior ou igual a 5 segundos, checar se o botão foi pressionado para setar emLatencia para falso, botaoLatencia para falso e executar a função semaforo()
  if (emLatencia && millis() - tempoInicioLatencia >= 5000) {
    if (botaoLatencia) {
      emLatencia = false;
      botaoLatencia = false;
      semaforo();
    }
  }

  //se o botão não estiver pressionado e o estado anterior for HIGH (e.g. após primeira execução), setar variável de estado anterior para LOW
  if (estadoBotao == HIGH && estadoAnteriorBotao == LOW) {
    estadoAnteriorBotao = estadoBotao;
  }

}

void semaforo() {

  //desliga o verde da via e acende o amarelo por 3 segundos
  digitalWrite(VERDE_VIA, LOW);
  digitalWrite(AMARELO_VIA, HIGH);
  delay(3000);
  

  //desliga o amarelo da via e acende o vermelho por 2 segundos
  digitalWrite(AMARELO_VIA, LOW);
  digitalWrite(VERMELHO_VIA, HIGH);
  delay(2000);
  
  //desliga o vermelho da via e acende o verde por 5 segundos
  digitalWrite(VERMELHO_PEDESTRE, LOW);
  digitalWrite(VERDE_PEDESTRE, HIGH);
  delay(5000);
   

  //pisca o led verde 3 vezes em um intervalo de tempo de 6 segundos
  
  for (int i = 0; i < 3; i++) {

    digitalWrite(VERDE_PEDESTRE, LOW);
    delay(1000);
    
    
    digitalWrite(VERDE_PEDESTRE, HIGH);
    delay(1000);   
    
  }

  //apaga os leds previamente acesos para retornar ao loop neste estado
  digitalWrite(VERMELHO_VIA, LOW);
  digitalWrite(VERDE_PEDESTRE, LOW);
  
  //seta a variável emLatencia para true, para indicar que o semáforo entrará em modo de latência após a execução de seu código
  emLatencia = true;
  
}
