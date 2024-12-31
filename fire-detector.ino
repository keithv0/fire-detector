// Blynk
#define BLYNK_TEMPLATE_ID " "
#define BLYNK_TEMPLATE_NAME " "
#define BLYNK_AUTH_TOKEN " "
#include <BlynkSimpleEsp8266.h>

// Wifi
#include <ESP8266WiFi.h>
const char* ssid = "name_wifi";
const char* password = "password_wifi";

// Pin untuk LED
int red = 2; // D4 = GPIO2

// Sensor MQ2
#define sensorMQ 0 // Pin analog untuk MQ-2

//Sensor dht11
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Flame Sensor
#define FLAME_SENSOR 14

void setup() {
  // Wifi
  WiFi.begin(ssid, password) ;

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Serial
  Serial.begin(9600);
  Serial.println("NodeMCU On");

  // LED
  pinMode(red, OUTPUT);
  digitalWrite(red, HIGH); // Awalnya LED mati

  // MQ-2
  // pin analog menggunakan fungsi analogRead(sensorMQ) di loop

  // dht
  dht.begin();

  // FlameSensor
  pinMode(FLAME_SENSOR, INPUT);
}

void loop() {
  // Blynk
  Blynk.run();

  // membaca nilai analog dari sensor MQ-2
  int gasValue = analogRead(sensorMQ);

  // membaca suhu n kelembapan
  float temperature=dht.readTemperature();
  float lembap=dht.readHumidity();

  // membaca data flame sensor
  int flameValue=digitalRead(FLAME_SENSOR);


  // cek validity mq 2
  if (isnan(gasValue)) {
    Serial.println("Sensor MQ-2 tidak Terbaca");
  } else {
    Serial.print("Gas Value :");
    Serial.println(gasValue);
    Blynk.virtualWrite(V9, gasValue); //blynk
  }

  // cek validity (dht);
  if (isnan(temperature) || isnan(lembap)) {
    Serial.println("Gagal membaca data dari sensor dht");
  } else {
    Serial.print("Suhu :");
    Serial.print(temperature);
    Serial.println("C");

    Serial.print("Kelembapan :");
    Serial.print(lembap);
    Serial.println("%");
    Blynk.virtualWrite(V7, temperature); //blynk
    Blynk.virtualWrite(V8, lembap); //blynk
  }

  // cek validity flame sensor
  if (flameValue == LOW) {
    Serial.println("Api Terdeteksi");
  } else {
    Serial.println("Takde Apiiii");
  }

  Serial.println("--------------------");
  delay(2000);
}
