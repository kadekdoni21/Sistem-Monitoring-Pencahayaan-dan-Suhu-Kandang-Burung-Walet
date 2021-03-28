#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"  
#define ldrPin 36
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

char auth[] = "g6LkRZ3ntFN8jbDbCX0syk5CDOCCQgvu"; 
char ssid[] = "Wifi.id_Indomaret_Sagan"; 
char pass[] = "otomatisnyambung"; 

int sliderLux;
int sliderSuhu;
int ledPin = 5; 
int ledPin2 = 18; 
float t;
int tamp;
BlynkTimer timer;

BLYNK_WRITE(V4)
  {
  float valueLux = param.asInt(); 
  sliderLux=valueLux;
  }

  BLYNK_WRITE(V5)
  {
  int valueSuhu = param.asInt(); 
  sliderSuhu=valueSuhu;
  }
  
BLYNK_WRITE(V8)  
{
  int button = param.asInt();
  tamp=button;
}

WidgetLED ledLDR(V6);
WidgetLED ledSUHU(V7);

void sensor()
{
  t = dht.readTemperature();
  int sensorCahaya = analogRead(ldrPin);
  int  ldr = ((0.009758 * sensorCahaya) + 10);
  int Suhu = t;

    
  Blynk.virtualWrite(V0, Suhu);
  Blynk.virtualWrite(V1, ldr);
  Blynk.virtualWrite(V2, ldr);
  Blynk.virtualWrite(V3, Suhu);

  
  if (isnan(t)) {
    Serial.println("Sensor tidak terbaca!");
    //return;
  }
  
  Serial.println("Sistem Monitoring Pencahayaan dan Suhu Kandang Burung Walet");
  Serial.print("Temperature : "); 
  Serial.print(t); 
  Serial.println("*C\t");
  Serial.print("Kecerahan : ");
  Serial.print(ldr);
  Serial.println(" LUX\t");

  Serial.print("slider suhu : ");
  Serial.println(sliderSuhu);
  
  Serial.print("slider LUX : ");
  Serial.println(sliderLux);

  
  Serial.print("Suhu : ");
  Serial.println(Suhu);
  Serial.print("LUX : ");
  Serial.println(ldr);
  
  
  if(Suhu > sliderSuhu){
    ledSUHU.on();
    ledSUHU.setColor("#D3435C");
    //Blynk.setProperty(V7,"color","#D3435C");
  }else{
    ledSUHU.on();
    //Blynk.setProperty(V7,"color","#23C48E");
    ledSUHU.setColor("#23C48E");
  }
  
  if(ldr > sliderLux){
    ledLDR.on();
    //Blynk.setProperty(V6,"color","#D3435C");
    ledLDR.setColor("#D3435C");
  }else{
    ledLDR.on();
    //Blynk.setProperty(V6,"color","#23C48E");
    ledLDR.setColor("#23C48E");
  }
}


void setup()
{
  Serial.begin(115200);
  delay(10);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");  
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,18,128), 8080);
  dht.begin();
  
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  timer.setInterval(1000L, sensor);
}



  
void loop()
{
  Blynk.run();
  if(tamp == 1){
      timer.run();
    }else {
      Serial.println("System mati"); 
    }
}
