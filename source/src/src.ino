#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define MOTOR 3
#define BUTTON 8
#define MAX_DISTANCE 400
#define DIST_THRESHOLD 30  // distância limite para modo 1

unsigned long lastMotorTime = 0;
int offInterval = 500; // tempo entre ativações, muda com base na distância
int onDuration = 150;   // tempo fixo de ativação (vibração) em ms

bool motorActive = false;
unsigned long motorStartTime = 0;

int modoAtual = 0; // 0 = vibração por distância, 1 = threshold contínuo, 2 = repouso
bool botaoAnterior = HIGH;

unsigned long lastSensorTime = 0;
int sensorInterval = 100; // lê o sensor a cada 100ms
int qtdAmostraMediaSensor = 5; //qtd de amostra para a média da medida

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MOTOR, OUTPUT);
  digitalWrite(MOTOR, LOW);

  Serial.begin(9600);
  Serial.println("O Arduino foi iniciado!");
}

long medirDistanciaCM() {
  // Disparo
  delay(10);
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Leitura do tempo de eco
  long duration = pulseIn(ECHO_PIN, HIGH, MAX_DISTANCE * 58);
  if (duration == 0) return MAX_DISTANCE; // se fora do alcance, retorna o máximo

  return duration * 0.034 / 2;
}

int medidaMediaDistanciaCM(){
  long soma = 0;
  int validas = 0;

  for (int i = 0; i < qtdAmostraMediaSensor; i++) {
    int leitura = medirDistanciaCM();
    if (leitura > 0) { // ignora leituras inválidas
      soma += leitura;
      validas++;
    }
    delay(10); // espera 20ms entre cada leitura
  }
  if (validas == 0) return 0; //evitar divisão por zero
  return soma/validas;
}

void loop() {
   bool botaoAtual = digitalRead(BUTTON);
  if (botaoAnterior == HIGH && botaoAtual == LOW) {
    modoAtual = (modoAtual + 1) % 3;
    Serial.print("Modo trocado para: ");
    Serial.println(modoAtual);
    delay(300); // debounce
  }
  botaoAnterior = botaoAtual;

  if (modoAtual == 2) {
    digitalWrite(MOTOR, LOW); // Garante que o motor está desligado
    return; // Sai da sub-rotina loop para não executar mais comandos
  }


  unsigned long currentMillis = millis();

  // === Leitura do sensor a cada 100ms ===
  static int distancia = MAX_DISTANCE;
  if (currentMillis - lastSensorTime >= sensorInterval) {
    lastSensorTime = currentMillis;
    distancia = medidaMediaDistanciaCM();

    Serial.print("Distância: ");
    Serial.print(distancia);
    Serial.println(" cm");

    // Atualiza intervalo de pausa com base na distância
    offInterval = map(distancia, 5, 100, 50, 1000);
    offInterval = constrain(offInterval, 50, 1000);
  }

  if (modoAtual == 0) {
  // Verifica se o motor está desligado e se já passou o tempo do intervalo OFF
    if (!motorActive && currentMillis - lastMotorTime >= offInterval) {
      motorActive = true;                    // Ativa o estado de vibração
      motorStartTime = currentMillis;        // Salva o tempo de início da vibração
      digitalWrite(MOTOR, HIGH);             // Liga o motor de vibração
    }

    // Verifica se o motor está ligado e se já passou o tempo do pulso ON
    if (motorActive && currentMillis - motorStartTime >= onDuration) {
      motorActive = false;                   // Desativa o estado de vibração
      lastMotorTime = currentMillis;         // Salva o tempo em que o motor foi desligado
      digitalWrite(MOTOR, LOW);              // Desliga o motor de vibração
    }

    return; // Encerra a execução para este modo
  }

  if (modoAtual == 1) {
  // Verifica se a distância medida é válida e menor ou igual ao limite definido
    if (distancia > 0 && distancia <= DIST_THRESHOLD) {
      digitalWrite(MOTOR, HIGH); // Liga o motor continuamente
    } else {
      digitalWrite(MOTOR, LOW);  // Desliga o motor
    }
  }

}