#include <SoftwareSerial.h>       // Serial library
SoftwareSerial espSerial(6, 7);   // Connect (Arduino RX: pin 6) to (ESP-01: TX pin)
                                  // and (Arduino TX: pin 7) to (ESP-01: RX pin)
#define DEBUG true
String mySSID = "";               // WiFi SSID
String myPWD = "";                // WiFi Password
String myAPI = "";                // Read API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1";        // Field 1: field to change value
int sendVal;


void setup()
{
  Serial.begin(9600);
  espSerial.begin(9600);
  
  espData("AT+RST", 1000, DEBUG);                                      // ESP-01 Reset
  espData("AT+CWMODE=1", 1000, DEBUG);                                 // Mode=1 => WiFi client
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   // Connect to WiFi network
  delay(1000);
  
}

  void loop()
  {    
    sendVal = random(1000);                            // Send a random number between 1 and 1000
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sendVal);
    espData("AT+CIPMUX=1", 1000, DEBUG);               // Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
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
    //Serial.print(response);
  }
  return response;
}
  
  
