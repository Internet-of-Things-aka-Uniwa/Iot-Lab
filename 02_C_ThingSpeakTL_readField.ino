#include <SoftwareSerial.h>        // Lib Software Serial
SoftwareSerial espSerial(6, 7);    // Pin 6 and 7 act as RX and TX. Connect them to TX and RX of ESP-01      
#define DEBUG true
String mySSID = "linksys_1";       // WiFi SSID
String myPWD = "";                 // WiFi Password
//String myAPI = "";               // Write API Key
String myReadAPI = "";             // Read API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELDa = "field2";        // ALERT field
String myCHANNEL = "";             // ThingSpeak Channel ID

unsigned int delayVal=15000;
String response = "";
String res_feeds = "";

String messager;
String x01;

void setup()
{
  Serial.begin(9600);
  espSerial.begin(9600);                                              // IN CASE OF ERROR, change espSerial to 9600
  
  espData("AT+RST", 1000, DEBUG);                                      // ESP-01 Reset
  espData("AT+CWMODE=1", 1000, DEBUG);                                 // Mode=1 => client
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   // Connect to WiFi network
  while(!espSerial.find("WIFI GOT IP"))                                // Wait for connection
  {          
      Serial.print(".");
      delay(1000);
      Serial.print(".");
      delay(1000);
      Serial.print(".");
      delay(1000);
  }
  Serial.println("Connected!");
  delay(1000);
}

  void loop()
  {

      messager = "GET /channels/"+myCHANNEL+"/feeds.json?api_key="+myReadAPI+"&results=1";
      espData("AT+CIPMUX=1", 1000, DEBUG);
      espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
      espData("AT+CIPSTO=10",1000, DEBUG);
      espData("AT+CIPSEND=0,"+String(messager.length()+4), 1000, DEBUG);
      espSerial.find(">"); 
      espSerial.println(messager);
      Serial.println("Starting to read...");
      espData("AT+CIPCLOSE=0",1000,DEBUG);

      delay(1000);
      Serial.println(response);
      int ret_Len=response.length();
      
      int pos = response.indexOf("[");
      res_feeds = response.substring(pos, ret_Len);
      Serial.print(res_feeds);
      pos = res_feeds.indexOf(myFIELDa);

      x01 = res_feeds.substring(pos+9, pos+10);
      Serial.println("ALERT field value:" +x01);

      ret_Len=0; 
      x01="";
      pos=0;   
      res_feeds = "";
      delay(delayVal);
    }
   
  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  response="";
  espSerial.println(command);
  long int time = millis();
  
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
   // Serial.print(response);                   // IN CASE OF ERROR, uncomment
  }
  return response;
}
  
