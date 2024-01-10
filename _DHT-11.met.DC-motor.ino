#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

char ssid[] = "IoTatelierF2144";
char pass[] = "IoTatelier";
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
const char broker[] = "192.168.144.1";
const char topic[] = "hiddezijlstra/Temp";
const int port = 1883;
const char publishTopic[] = "hiddezijlstra/Temp";

#include <DHT.h>

#define DHTPIN 5    
#define DHTTYPE DHT11

#define motorIN1 2
#define motorIN2 3
#define motorENA 9

#define redPin 13
#define greenPin 12

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(5000);
  }
  
  bool MQTTconnected = false;
  while (!MQTTconnected) {
    if (!mqttClient.connect(broker, port))
      delay(1);
    else
      MQTTconnected = true;
    }
    
    mqttClient.onMessage(onMqttMessage);
    mqttClient.subscribe(topic);
    Serial.println(topic);

  Serial.begin(9600);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorENA, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  dht.begin();
}

void loop() {
  delay(2000);  

  float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.println(temperature);

    
    if (temperature > 20) {
      startMotor();
      turnOnGreenLED();
    } else {
      stopMotor();
      turnOnRedLED();
    }
  } else {
    Serial.println("Failed to read temperature from DHT sensor!");
  }

  mqttClient.beginMessage("hiddezijlstra/Temp");
  mqttClient.print(temperature);
  mqttClient.endMessage();

  mqttClient.poll();
}

void startMotor() {
  analogWrite(motorENA, 255);  
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
}

void stopMotor() {
  analogWrite(motorENA, 0);  
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
}

void turnOnRedLED() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
}

void turnOnGreenLED() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
}

  void onMqttMessage(int messageSize) 
  {
  while (mqttClient.available()) 
  {
    Serial.print((char)mqttClient.read());
    }
    
  Serial.println();
  Serial.println();
  }
