#include <FirebaseESP8266.h>
#include <DHT.h>
#include  <ESP8266WiFi.h>

#define FIREBASE_HOST "https://iotlab-ea87e-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "1GOKXU3AEhEARIznSlvCnuZkr6By5Qu6tDTfo7oO"
#define WIFI_SSID "MinhDat_2.4GHz" // Thay đổi tên wifi của bạn
#define WIFI_PASSWORD "24022001" // Thay đổi password wifi của bạn
#define DHTPIN 14    // Chân dữ liệu của DHT 11 , với NodeMCU chân D5 GPIO là 14
#define DHTTYPE DHT11   // DHT 11
#define Led D0
#define Sound D1
#define Relay D7
DHT dht(DHTPIN, DHTTYPE);
FirebaseData fbdo;

float h, t;
int l;
void setup() {
  pinMode(A0,INPUT);
  pinMode(Led, OUTPUT);
  pinMode(Sound, OUTPUT);
  pinMode(Relay, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Dang ket noi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  dht.begin();
  Serial.println ("");
  Serial.println ("Da ket noi WiFi!");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void Get_active(int val_gas, int val_led, int val_sound)
{
  digitalWrite(Led, val_led);
  digitalWrite(Sound, val_sound);
  digitalWrite(Relay, val_gas);
}
int Button_fan()
{
  int val;
  if (Firebase.getInt(fbdo, "/eq1/fan")) val = fbdo.intData();
  return val;
}
int Button_led()
{
  int val;
  if (Firebase.getInt(fbdo, "/eq2/led")) val = fbdo.intData();
  return val;
}
int Button_sound()
{
  int val;
  if (Firebase.getInt(fbdo, "/eq3/sound")) val = fbdo.intData();
  return val;
}
void Sensor( float Temp, float Hum, float Gas){
  Firebase.setFloat( fbdo,"/Set/Nhiet do", Temp);
  Firebase.setFloat ( fbdo,"/Set/Do am", Hum);
  Firebase.setFloat ( fbdo,"/Set/Khi gas", Gas);  
}

void Display_serial()
{ 
  h = dht.readHumidity();
  t = dht.readTemperature();  // Đọc nhiệt độ theo độ C
  l = map(analogRead(A0), 0, 1024, 0, 100);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Sensor(t,h,l);
  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print("*C  ");
  Serial.print("Do am: ");
  Serial.print(h);
  Serial.print("%  ");        
  Serial.print("Khi GAS: ");
  Serial.print(l);
  Serial.println("%  ");
}

void loop() {
  int dig_gas, dig_led, dig_sound;
  Display_serial();
  dig_gas = Button_fan();
  dig_led = Button_led();
  dig_sound = Button_sound();
  Get_active(dig_gas, dig_led, dig_sound);
}
