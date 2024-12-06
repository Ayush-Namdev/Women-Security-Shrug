#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS GPS;
SoftwareSerial Sgps(2, 3);
SoftwareSerial Sgsm(10,11);
int button=4;
const int flexPin = A0;
float Last_lat = 0;
float Last_lon = 0; 

void setup()
{
  Serial.begin(9600);
  Sgps.begin(9600);
  Sgsm.begin(9600);
  pinMode(button,INPUT_PULLUP);
  pinMode(A0,INPUT);
  digitalWrite(A0,HIGH);
}

void loop(){
  bool newData = false;
  int flexValue;
    flexValue = analogRead(flexPin);
  while (Sgps.available())
    {
      char c = Sgps.read();
      if (GPS.encode(c)) 
        newData = true;
    }
    if (newData == true){
      FetchLocation();
      
    }

    if(flexValue<890){
      SendMessage();
    }

    if(digitalRead(button)==LOW){
      SendMessage();
    }
}


void FetchLocation(){
  GPS.f_get_position(&Last_lat, &Last_lon);
  
 
}

void SendMessage()
{
Sgsm.flush();
  Serial.flush();
 
  Serial.println("SMS Sending Start..");
 Sgsm.print("AT+CMGF=1"); 
 delay(200);
 Sgsm.write(13);
 delay(1500);
 Sgsm.print("AT+CMGS="); 
 Sgsm.write(0x22);
 Sgsm.print("7007436424");
 Sgsm.write(0x22);
 Sgsm.write(13); 
 delay(300);
 Sgsm.print("I am Not Safe.. I am at this location. Please Help me..");
      Sgsm.print("https://www.google.com/maps/?q=");
      Sgsm.print(Last_lat, 6);
      Sgsm.print(",");
      Sgsm.print(Last_lon, 6);
 delay(1500);
 Sgsm.write(26);
 Serial.println("SMS Sending End..");
 delay(1000); 
}
