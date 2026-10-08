const int ledVermelho0 = 10;
const int ledVermelho1 = 12;
const int ledVermelho2 = 2;
const int ledVermelho3 = 3;
const int ledVermelho4 = 4;
const int ledVerde5 = 5;
const int ledVerde6 = 6;
const int ledVerde7 = 7;
const int ledVerde8 = 8;
const int ledVerde9 = 9;
const int pinoBuzzer = 11;
const int pinoBotao = 13;

const int leds[] = {10, 12, 2, 3, 4, 5, 6, 7, 8, 9};

int fase = 0;
unsigned long tempoAnterior = 0;
unsigned long tempoInicioReacao = 0;
int passoVermelho = 0;
long duracaoEsperaAleatoria = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(ledVermelho0, OUTPUT);
  pinMode(ledVermelho1, OUTPUT);
  pinMode(ledVermelho2, OUTPUT);
  pinMode(ledVermelho3, OUTPUT);
  pinMode(ledVermelho4, OUTPUT);
  pinMode(ledVerde5, OUTPUT);
  pinMode(ledVerde6, OUTPUT);
  pinMode(ledVerde7, OUTPUT);
  pinMode(ledVerde8, OUTPUT);
  pinMode(ledVerde9, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);
  
  pinMode(pinoBotao, INPUT_PULLUP);
}

void loop() {
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    if (comando == "COMEMORAR") {
      executarComemoracaoTop7();
    }
  }

  unsigned long tempoAtual = millis();

  if (fase == 0) {
    if (digitalRead(pinoBotao) == LOW) {
      Serial.println("INICIAR_RODADA");
      fase = 1;
      passoVermelho = 0;
      tempoAnterior = tempoAtual;
      delay(300);
    }
  }

  else if (fase == 1) {
    if (tempoAtual - tempoAnterior >= 1000) {
      tempoAnterior = tempoAtual;
      
      if (passoVermelho == 0) { digitalWrite(ledVermelho0, HIGH); tone(pinoBuzzer, 2200); delay(100); noTone(pinoBuzzer); }
      else if (passoVermelho == 1) { digitalWrite(ledVermelho1, HIGH); tone(pinoBuzzer, 2200); delay(100); noTone(pinoBuzzer); }
      else if (passoVermelho == 2) { digitalWrite(ledVermelho2, HIGH); tone(pinoBuzzer, 2200); delay(100); noTone(pinoBuzzer); }
      else if (passoVermelho == 3) { digitalWrite(ledVermelho3, HIGH); tone(pinoBuzzer, 2200); delay(100); noTone(pinoBuzzer); }
      else if (passoVermelho == 4) { 
        digitalWrite(ledVermelho4, HIGH); tone(pinoBuzzer, 2200); delay(100); noTone(pinoBuzzer);
        duracaoEsperaAleatoria = random(1000, 5000); 
        tempoAnterior = tempoAtual;
        fase = 2; 
      }
      passoVermelho++;
    }
    
    if (digitalRead(pinoBotao) == LOW) {
      Serial.println("QUEIMA");
      tone(pinoBuzzer, 1000); delay(400); noTone(pinoBuzzer);
      apagarTudo();
      tempoAnterior = millis();
      fase = 5;
    }
  }

  else if (fase == 2) {
    if (tempoAtual - tempoAnterior >= (unsigned long)duracaoEsperaAleatoria) {
      apagarVermelhos();
      ligarVerdes();
      tone(pinoBuzzer, 2200); delay(200); noTone(pinoBuzzer);
      tempoInicioReacao = millis();
      fase = 3; 
    }
    
    if (digitalRead(pinoBotao) == LOW) {
      Serial.println("QUEIMA");
      tone(pinoBuzzer, 1000); delay(400); noTone(pinoBuzzer);
      apagarTudo();
      tempoAnterior = millis();
      fase = 5;
    }
  }

  else if (fase == 3) {
    if (digitalRead(pinoBotao) == LOW) {
      unsigned long tempoReacao = millis() - tempoInicioReacao;
      apagarVerdes();
      tone(pinoBuzzer, 3000); delay(150); noTone(pinoBuzzer);
      
      Serial.print("TEMPO:");
      Serial.println(tempoReacao);
      
      tempoAnterior = millis();
      fase = 4; 
    }
  }

  else if (fase == 4 || fase == 5) {
    if (tempoAtual - tempoAnterior >= 1500) {
      apagarTudo();
      fase = 0; 
    }
  }
}

void apagarVermelhos() {
  digitalWrite(ledVermelho0, LOW); digitalWrite(ledVermelho1, LOW); digitalWrite(ledVermelho2, LOW); digitalWrite(ledVermelho3, LOW); digitalWrite(ledVermelho4, LOW);
}

void ligarVerdes() {
  digitalWrite(ledVerde5, HIGH); digitalWrite(ledVerde6, HIGH); digitalWrite(ledVerde7, HIGH); digitalWrite(ledVerde8, HIGH); digitalWrite(ledVerde9, HIGH);
}

void apagarVerdes() {
  digitalWrite(ledVerde5, LOW); digitalWrite(ledVerde6, LOW); digitalWrite(ledVerde7, LOW); digitalWrite(ledVerde8, LOW); digitalWrite(ledVerde9, LOW);
}

void apagarTudo() {
  apagarVermelhos();
  apagarVerdes();
}

void tocarNotaEMoverLeds(int frequencia, int duracaoMs) {
  apagarTudo();
  for (int i = 0; i < 3; i++) {
    int pinoAleatorio = leds[random(0, 10)];
    digitalWrite(pinoAleatorio, HIGH);
  }

  if (frequencia > 0) {
    tone(pinoBuzzer, frequencia);
  } else {
    noTone(pinoBuzzer);
  }
  
  delay(duracaoMs);
  noTone(pinoBuzzer);
}

void executarComemoracaoTop7() {
  int melodia[] = {
    659, 659, 659, 659,   
    523, 659, 784,        
    392                   
  };

  int tempos[] = {
    120, 120, 120, 180,
    120, 120, 250,
    400
  };

  int totalNotas = sizeof(melodia) / sizeof(melodia[0]);

  for (int i = 0; i < totalNotas; i++) {
    tocarNotaEMoverLeds(melodia[i], tempos[i]);
    delay(40); 
  }

  for (int p = 0; p < 2; p++) {
    apagarTudo();
    delay(100);
    ligarVerdes();
    digitalWrite(ledVermelho0, HIGH);
    digitalWrite(ledVermelho1, HIGH);
    digitalWrite(ledVermelho2, HIGH);
    digitalWrite(ledVermelho3, HIGH);
    digitalWrite(ledVermelho4, HIGH);
    delay(200);
    apagarTudo();
  }
}