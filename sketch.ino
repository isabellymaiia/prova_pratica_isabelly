#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

const int buttonPin = 18;  // Pino utilizado para controle do botão
unsigned long buttonLastPressed = 0;  // Controle de debounce
const unsigned long debounceDelay = 200; // Delay para debounce

const int ldrPin = 4;  // Pino utilizado para controle do ldr
int threshold=600;
int timesPressed = 0; // variável para contar o número de vezes que o botão foi

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT_PULLUP); // Initialize the pushbutton pin as an input

  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600);
  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(100);
  //   Serial.print(".");
  // }
  Serial.println("Conectado!");

}

void loop() {

  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus<=threshold){
    Serial.print("its dark turn on led");
    Serial.println(ldrstatus);
    digitalWrite(led_amarelo, HIGH);
    delay(1000);
    digitalWrite(led_amarelo, LOW);
    delay(1000);
    
  }else{
    Serial.print("its bright turn off light");
    Serial.println(ldrstatus);
    digitalWrite(led_verde, HIGH);
    delay(3000);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, HIGH);
    delay(2000);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_vermelho, HIGH);
    if (digitalRead(buttonPin) == LOW) {
    unsigned long currentTime = millis();
    if ((currentTime - buttonLastPressed) > debounceDelay) {
      buttonLastPressed = currentTime;

      timesPressed =+ 1;
      Serial.println("Número de vezes que o botão foi pressionado:");
      Serial.println(timesPressed);
    }
  }

  if(WiFi.status() == WL_CONNECTED && timesPressed == 3 && digitalRead(led_vermelho) == HIGH){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("Requisição não enviada");
    delay(5000);
    digitalWrite(led_vermelho, LOW);
  }
    
    }

    
} 