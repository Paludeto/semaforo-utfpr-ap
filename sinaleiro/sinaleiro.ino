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

void setup() {
  
  pinMode(LED_BUILTIN, LOW);
  pinMode(VERDE_VIA, OUTPUT);
  pinMode(VERMELHO_VIA, OUTPUT);
  pinMode(AMARELO_VIA, OUTPUT);
  pinMode(VERMELHO_PEDESTRE, OUTPUT);
  pinMode(VERDE_PEDESTRE, OUTPUT);
  pinMode(BOTAO_PEDESTRE, INPUT);
  
}

void loop() {

  //semáforo da via sempre verde, semáforo de pedestre sempre vermelho enquanto o botão não estiver apertado
  digitalWrite(VERDE_VIA, HIGH);
  digitalWrite(VERMELHO_PEDESTRE, HIGH);

  //variável que armazena o estado atual do botão
  estadoBotao = digitalRead(BOTAO_PEDESTRE);

  //se o estado atual do botão for HIGH e o estado anterior LOW, estadoAnterior armazena o estado atual e a função semáforo é chamada
  if (estadoBotao == HIGH && estadoAnteriorBotao == LOW && emLatencia == false) {
    estadoAnteriorBotao = estadoBotao;
    semaforo();
  }

  //checa se botão foi pressionado durante modo de latência
  if (estadoBotao == HIGH && estadoAnteriorBotao == LOW && emLatencia == true) {
    if (digitalRead(BOTAO_PEDESTRE) == HIGH) {
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
  if (estadoBotao == LOW && estadoAnteriorBotao == HIGH) {
    estadoAnteriorBotao = estadoBotao;
  }

}

void semaforo() {

  unsigned long tempoAnterior = millis();
  
  //desliga o verde da via e acende o amarelo por 3 segundos
  while (millis() - tempoAnterior < 3000) {
    digitalWrite(VERDE_VIA, LOW);
    digitalWrite(AMARELO_VIA, HIGH);
  }

  //desliga o amarelo da via e acende o vermelho por 2 segundos
  tempoAnterior = millis();
  while (millis() - tempoAnterior < 2000) {
    digitalWrite(AMARELO_VIA, LOW);
    digitalWrite(VERMELHO_VIA, HIGH);
  }
  
  //desliga o vermelho da via e acende o verde por 5 segundos
  tempoAnterior = millis();
  while (millis() - tempoAnterior < 5000) {
    digitalWrite(VERMELHO_PEDESTRE, LOW);
    digitalWrite(VERDE_PEDESTRE, HIGH);
  }

  //pisca o led verde 3 vezes em um intervalo de tempo de 6 segundos
  
  for (int i = 0; i < 3; i++) {

    tempoAnterior = millis();
    while (millis() - tempoAnterior < 1000) {
      digitalWrite(VERDE_PEDESTRE, LOW);
    }
    
    tempoAnterior = millis();
    while (millis() - tempoAnterior < 1000) {
      digitalWrite(VERDE_PEDESTRE, HIGH);
    }
    
  }

  //apaga os leds previamente acesos para retornar ao loop neste estado
  digitalWrite(VERMELHO_VIA, LOW);
  digitalWrite(VERDE_PEDESTRE, LOW);
  
  //seta a variável emLatencia para true, para indicar que o semáforo entrará em modo de latência após a execução de seu código
  //cronometra o tempo de início do modo de latência
  tempoInicioLatencia = millis();
  emLatencia = true;
  
}
