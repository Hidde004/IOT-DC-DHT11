#include <DHT.h>

#define DHTPIN 5  
#define DHTTYPE DHT11

#define motorIN1 2
#define motorIN2 3
#define motorENA 9

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorENA, OUTPUT);
  dht.begin();
}

void loop()
 {
  delay(2000);  

  float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.println(temperature);

    if (temperature > 20.0) {
      startMotor();
    } else {
      stopMotor();
    }
  } else {
    Serial.println("Failed to read temperature from DHT sensor!");
  }
}

void startMotor() 
{
  analogWrite(motorENA, 255);  
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
}

void stopMotor() {
  analogWrite(motorENA, 0);  
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
}
