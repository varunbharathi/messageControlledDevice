#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);  // (Rx,Tx  > Tx,Rx) 

char incomingByte; 
String inputString;
int relay = 8; 
String number="+918778918665";
// Output for Relay Control
int state=0;

void setup() 
{
      pinMode(relay, OUTPUT);
      digitalWrite(relay, LOW); // Initial state of the relay
      Serial.begin(9600);
      mySerial.begin(9600);
      pinMode(12,OUTPUT);
      digitalWrite(12,HIGH); 

     while(!mySerial.available()){
        mySerial.println("AT");
        delay(1000); 
        Serial.println("Connecting...");
        }
      Serial.println("Connected!"); 
     mySerial.println("AT+CSQ");
      delay(1000);
      mySerial.println("AT+CMGF=1");  //Set SMS to Text Mode 
      delay(1000);  
      mySerial.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
      delay(1000);
      mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read Unread Messages
     }

void loop()
{  
  if(mySerial.available()){
      delay(100);

      // Serial Buffer
      while(mySerial.available()){
        incomingByte = mySerial.read();
        inputString += incomingByte; 
        }

        delay(10);      

        Serial.println(inputString);
        inputString.toUpperCase(); // Uppercase the Received Message

        //turn RELAY ON or OFF
        if (inputString.indexOf("ON") > -1)
        {
          digitalWrite(relay, HIGH);
          Serial.println("light on");
          state=1;
          }
        if (inputString.indexOf("OFF") > -1)
        {
          digitalWrite(relay, LOW);
          Serial.println("light off");
          state=0;
          }
        if (inputString.indexOf("STATE") > -1)
        
        {
            if(state==1)
            {
              sendmessageon();
            }
            else if(state==0)
            {
              sendmessageoff();
            }
        }

        delay(50);

        //Delete Messages & Save Memory
        if (inputString.indexOf("OK") == -1){
        mySerial.println("AT+CMGDA=\"DEL ALL\"");

        delay(1000);}

        inputString = "";
  }
}
void sendmessageon()
{
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+919790015561 aaqQ\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("State is on"); //text content
  updateSerial();
  mySerial.write(26);
   mySerial.println("AT+CMGF=1");  //Set SMS to Text Mode 
      delay(1000);  
      mySerial.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
      delay(1000);
}
void sendmessageoff()
{
   mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+919790015561\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("state is off"); //text content
  updateSerial();
  mySerial.write(26);
   mySerial.println("AT+CMGF=1");  //Set SMS to Text Mode 
      delay(1000);  
      mySerial.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
      delay(1000);
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
