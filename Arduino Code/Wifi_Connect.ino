//BOT 1
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
Servo myservo;

#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Rightx          GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)
#define SER   5                                               //GPIO(D1)
int i = 0;
const char* ssid = "AndroidAP";               //Wifi Name

const char* passkey = "12345678";      //Wifi Passkey

//mqtt server domain & topic
const char* mqtt_server = "nc7cec0d-internet-facing-c87ec016a157b43d.elb.ap-south-1.amazonaws.com";
const int mqtt_port = 1883;
const char* topic = "1";

//mqtt broker userID & pass
const char* username = "Test02";
const char* password = "123456789";

void goAhead() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void goBack() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void goRight() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);


  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  delay(340);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  delay(100);
}
void goLeft() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);


  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  delay(100);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  delay(100);}
void stopBot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
void servo() {
  myservo.write(180);
  delay(500);
  myservo.write(0);
  delay(500);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.println("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, passkey);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topicstr = topic;
  char sub[1000];
  int c = 0;
  Serial.print("Message arrived [ ");
  Serial.print(topic);
  Serial.print("]");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
 
  //Switch Case
  char store = (char)payload[0];
  switch (store) {
    case 'F': 
      {
        goAhead();
        break;
      } 
    case 'R': 
      {
        goRight();
        delay(10);
        stopBot();
        break;
      }
    case 'L': 
      {
        stopBot();
        goLeft();
        stopBot();
        break;
      }
    case 'Z': 
      {
        stopBot();
        servo();
        break;
      }   
    case 'S': 
      {
        stopBot();
        break;
      }  
    case 'B': 
      {
        stopBot();
        goBack();
        break;
      }      
  }
}



WiFiClient espClient;
PubSubClient client(espClient);


void reconnect() {
  //Loop until reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT Connection...");
    //create a random  client ID
    String clientId = "Test01";
    //Attempt to connect
    if (client.connect(clientId.c_str(), username, password)) {
      Serial.println("Connected");
      client.publish("Led", "MQTT SUCCESSFULLY CONNECTED", true);
      client.subscribe(topic);
    }
    else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println("Try again in 5 seconds");
      //wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {
  // put your setup code here, to run once:
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(SER, OUTPUT);
  myservo.attach(5);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
