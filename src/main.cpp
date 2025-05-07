#include <OneWire.h>
#include <DallasTemperature.h>

// Sensor de temperatura DS18B20 no pino A2
#define ONE_WIRE_BUS A2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// LEDs
#define LED_AMARELO 13
#define LED_VERDE 12
#define LED_VERMELHO 11

// Botão
#define BOTAO_PIN 7

// Potenciômetro para simular BPM
#define POT_PIN A0
int potValue = 0;
int bpm = 0;

// Controle de estado
bool leituraAtiva = false;
bool estadoAnteriorBotao = HIGH;

void setup() {
  Serial.begin(9600);
  sensors.begin();

  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  pinMode(BOTAO_PIN, INPUT_PULLUP);
}

void loop() {
  // Leitura do botão (modo liga/desliga)
  bool estadoBotao = digitalRead(BOTAO_PIN);

  if (estadoAnteriorBotao == HIGH && estadoBotao == LOW) {
    leituraAtiva = !leituraAtiva;
    Serial.print("Leitura de temperatura: ");
    Serial.println(leituraAtiva ? "ATIVA" : "DESATIVADA");
    delay(200); // debounce
  }

  estadoAnteriorBotao = estadoBotao;

  if (leituraAtiva) {
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);

    Serial.print("Temperatura corporal: ");
    Serial.print(tempC);
    Serial.println(" °C");

    if (tempC < 10.0) {
      digitalWrite(LED_AMARELO, HIGH);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_VERMELHO, LOW);
    } else if (tempC >= 10.0 && tempC <= 10.5) {
      digitalWrite(LED_AMARELO, LOW);
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_VERMELHO, LOW);
    } else {
      digitalWrite(LED_AMARELO, LOW);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_VERMELHO, HIGH);
    }

    potValue = analogRead(POT_PIN);
    bpm = map(potValue, 0, 1023, 0, 200);
    Serial.print("BPM simulado: ");
    Serial.println(bpm);

    delay(1000); 
  } else {
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);
  }
}
