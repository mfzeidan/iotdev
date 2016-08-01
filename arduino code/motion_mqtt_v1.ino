
/*
     * PIR sensor tester
     */


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define wifi_ssid "Verizon-MiFi6620L-E739" //fill this
#define wifi_password "015207b8" //fill this
#define mqtt_server "162.243.161.166" //fill this
#define mqtt_user "" // no fill necessary
#define mqtt_password "" // no fill necessary
#define topic1 "test"
#define mqtt_client_name "EAS1" //this needs to be unique for each unit
    //int ledPin = 13;                // choose the pin for the LED
    int inputPin = 5  ;               // choose the input pin (for PIR sensor)
    int pirState = LOW;             // we start, assuming no motion detected
    int val = 0;                    // variable for reading the pin status
    int t_count = 0;
    int move_count = 0;
    int empty_bit = 0;
    int calibrationTime =60;
    int delay_time = 1000;
    int red_delay = 3000;
    //longer delay for when motion is detected to allow for a more proper reset

WiFiClient espClient;
PubSubClient client(espClient);
    
    void setup() {
      //pinMode(ledPin, OUTPUT); 
      setup_wifi();
      client.setServer(mqtt_server, 1883);
      
      // declare LED as output
      Serial.println(" done");
      Serial.begin(9600);
      pinMode(inputPin, INPUT);     // declare sensor as input
      //digitalWrite(inputPin, HIGH); 
      for(int i = 0; i < calibrationTime; i++){
        Serial.print(".");
        delay(1000);
      }
      Serial.println(" done");
      Serial.println("SENSOR ACTIVE");
      delay(50);
    }

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(mqtt_client_name)) { //* See //NOTE below
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void pubMQTT(String topic,String topic_val){
    Serial.print("Newest topic " + topic + " value:");
    Serial.println(String(topic_val).c_str());
    client.publish(topic.c_str(), String(topic_val).c_str(), true);
}


String message = "";
     
void loop(){
  
  if (!client.connected()) {
    reconnect();
  }
      
      val = digitalRead(inputPin);  // read input value
      t_count++;
      Serial.println(t_count);
      if (val == HIGH) {            // check if the input is HIGH
        
        move_count++;
        if(move_count > 3)
        {
          message = "EAS1:Red";
          pubMQTT(topic1, message);
          Serial.println("motion detected message sent");
          Serial.println("send red");
          t_count = 0;
          move_count = 0;
          empty_bit = 0;
          delay(red_delay);
        }
        delay(delay_time);
      } else {
        
        Serial.println("undetected");
        if(t_count == 60)
        {
          Serial.println("send green");
          message = "EAS1:Green";
          pubMQTT(topic1, message);
          t_count = 0;
          empty_bit = 1;
        }
        else if(t_count == 30 && empty_bit == 0)
        {
          Serial.println("send yellow");
          message = "EAS1:Yellow";
          pubMQTT(topic1, message);
          move_count =0;
        }
        delay(delay_time);
          
      }
    }

