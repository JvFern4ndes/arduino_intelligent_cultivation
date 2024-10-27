#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define LED_UMIDADE_PIN 10
#define LED_TEMPERATURA_PIN 9
#define LED_UMIDADE_SOLO_PIN 8

const int sensorSoloPin = A1;
int valorSeco = 1020;
int valorMolhado = 420;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int displayMode = 0;
unsigned long lastSwitchTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicio do Sketch");

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Inicializando");
  delay(2000);
  lcd.clear();
  
  dht.begin();
  
  // Configuração dos pinos das LEDs como saídas
  pinMode(LED_UMIDADE_PIN, OUTPUT);
  pinMode(LED_TEMPERATURA_PIN, OUTPUT);
  pinMode(LED_UMIDADE_SOLO_PIN, OUTPUT);
}

void loop() {
  float temperaturaC = dht.readTemperature();
  float umidade = dht.readHumidity();

  int leituraSolo = analogRead(sensorSoloPin);
  int umidadeSoloPercentual = map(leituraSolo, valorSeco, valorMolhado, 0, 100);
  umidadeSoloPercentual = constrain(umidadeSoloPercentual, 0, 100);

  if (millis() - lastSwitchTime >= 3000) {
    displayMode = (displayMode + 1) % 3;  // Alterna entre 0, 1, e 2
    lastSwitchTime = millis();
  }

  lcd.clear();  // Limpa o display para evitar sobreposição de informações

  // Exibe as informações de acordo com o modo
  switch (displayMode) {
    case 0:
      lcd.setCursor(1, 0);
      lcd.print("Temperatura Ar");
      lcd.setCursor(5, 1);
      lcd.print(temperaturaC, 2);
      lcd.print(" C");
      break;

    case 1:
      lcd.setCursor(3, 0);
      lcd.print("Umidade: Ar");
      lcd.setCursor(5, 1);
      lcd.print(umidade, 2);
      lcd.print(" %");
      break;

    case 2:
      lcd.setCursor(1, 0);
      lcd.print("Umidade: Solo");
      lcd.setCursor(5, 1);
      lcd.print(umidadeSoloPercentual);
      lcd.print(" %");
    break;
  }

  if (umidade >= 70 && umidade <= 80) {
    digitalWrite(LED_UMIDADE_PIN, HIGH);
  } else {
    digitalWrite(LED_UMIDADE_PIN, LOW);
  }

  if (temperaturaC >= 22 && temperaturaC <= 30) {
    digitalWrite(LED_TEMPERATURA_PIN, HIGH);
  } else {
    digitalWrite(LED_TEMPERATURA_PIN, LOW);
  }

  if (umidadeSoloPercentual >= 55 && umidadeSoloPercentual <= 70) {
    digitalWrite(LED_UMIDADE_SOLO_PIN, HIGH);
  } else {
    digitalWrite(LED_UMIDADE_SOLO_PIN, LOW);
  }

  delay(3000);
}
