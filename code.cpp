#include<DHT.h>
#include<DHT_U.h>
#define IR1 D1
#define IR2 D0
#define LED D4
#define FAN D6
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
int flag_fan = 0; //to check fan is on or off
float temp;
int persons_in_house;
int person_count = 0;
float thing_dht;
bool thing_lights;
bool thing_fan;
#include "thingProperties.h"
void setup(){
	//Initialize serial and wait for port to open:
	Serial.begin(9600);
	//this delay gives the chance to wait for a serial monitor without blocking if none is found
	delay(1500);
	//Defined in thingProperties.h
	initProperties();
	//Connect to Arduino IoT Cloud
	ArduinoCloud.begin(ArduinoIoTPreferredConnection);
	setDebugMessageLevel(2);
	ArduuinoCloud.printDebugInfo();
	pinMode(IR1,INPUT);
	pinMode(IR2,INPUT);
	pinMode(LED,OUTPUT);
	pinMode(DHTPIN,INPUT);
	pinMode(FAN,OUTPUT);
	dht.begin();
}
void loop(){
	ArduinoCloud.update();
	thing_dht = temp = dht.readTemperature();
	if(digitalRead(IR1)==HIGH){
		while(digitalRead(IR2)==HIGH) delay(100);
		percon_count++;
		Serial.println("WELCOME");
		if(person_count==1){
			digitalWrite(LED,HIGH);
			Serial.println("LIGHT IS ON");
			delay(1000);
		}
	}
	if(person_count>=1 && temp>18 && flag_fan==0){
		digitalWrite(FAN,HIGH);
		Serial.print("TEMPERATURE IS ");
		Serial.println(temp);
		Serial.println("FAN IS ON");
		flag_fan = 1;
	}
	if(digitalRead(IR2)==HIGH){
		while(digitalRead(IR2)==LOW) delay(100);
		Serial.println("GOODBYE");
		person_count--;
		Serial.print("PERSONS IN ROOM : ");
		Serial.println(person_count);
	    if(person_count==0){
			digitalWrite(LED,LOW);
			Serial.println("LIGHT IS OFF");
			delay(1000);
			digitalWrite(FAN,LOW);
			Serial.println("FAN IS OFF");
			flag_fan = 0;
		}
	}
	persons_in_house = person_count;
}
void onThingFanChange(){
	digitalWrite(FAN,thing_fan);
}
void onThingLightsChange(){
	digitalWrite(LED,thing_lights);
}