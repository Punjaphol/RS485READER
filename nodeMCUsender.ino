/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>

const char* ssid     = <SSID>;
const char* password = "<PWD>";

#define APPID   "APPID"
#define KEY     "APPKEY"
#define SECRET  "APPSECRET"
#define ALIAS  "TOPICNAME"

#define INDICATOR D8

boolean OUT_PUT=false;
WiFiClient client;


int RETRY_TIMEOUT=  200;//60sec

String line="";
#define TIMER003_SET  -550000; //3 sec
float TIMER003 = TIMER003_SET ; //3 sec
boolean IS_TIMER3SEC_READY() { ///////////// timer 3 sec/////////////
  if (TIMER003 > 0) {
    TIMER003 = TIMER003_SET; return true;
  } else {
    TIMER003++; return false;
  }
}
String GET_MAC() {
  String MAC_ID="";
  uint8_t  MAC_softAP[]          = {0, 0, 0, 0, 0, 0};                 //not sure why we have to pass a MAC address to get a MAC address.
  uint8_t  MAC_STA[] = {0, 0, 0, 0, 0, 0}; //- See more at: http://www.esp8266.com/viewtopic.php?f=29&t=3587#sthash.jsrBVMQr.dpuf
  uint8_t* MAC  = WiFi.softAPmacAddress(MAC_softAP);                   //get MAC address of softAP interface
 // Serial.print("MAC[AP]:");
  for (int i = 0; i < sizeof(MAC) + 2; ++i) {                                                       //this line needs cleaning up.
    //Serial.print(":");
   // Serial.print(MAC[i], HEX);
   // MAC_ID+=":" + String(MAC[i], HEX);
    MAC_softAP[i] = MAC[i];                                         //copy back to global variable
  }
 // Serial.println();
 // Serial.print("MAC[STA]");
  MAC  = WiFi.macAddress(MAC_STA);                   //get MAC address of STA interface
  for (int i = 0; i < sizeof(MAC) + 2; ++i) {
   // Serial.print(":");
   // Serial.print(MAC[i], HEX);
    MAC_STA[i] = MAC[i];                                            //copy back to global variable
    String t=String(MAC[i], HEX);
    t=(t.length()==2)?t:"0" + t ;
    MAC_ID+=":" + t;
  }// - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=3587#sthash.jsrBVMQr.dpuf
  return MAC_ID;
}


MicroGear microgear(client);


/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);//#AMB|Sens|hu|#
}


void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
   //  Serial.println("Reseted MQTT Connection");
   //  client.stop();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("MQTT Connected");
  // Set the alias of this microgear ALIAS
  microgear.setAlias(ALIAS);
   Serial.flush();
   Serial.flush();  
}

void setup() {
  pinMode(INDICATOR,OUTPUT);

  microgear.on(MESSAGE, onMsghandler);
  microgear.on(ABSENT, onLostgear);
  microgear.on(CONNECTED, onConnected);
  Serial.begin(9600);
  Serial.print("Starting...MAC=");
  //   WiFi.config(local_ip,gateway,subnet);   //setค่าไปยังโมดูล
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  Serial.println(GET_MAC());
  
  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);
}

void loop() 
{
           // check wifi connection
            while (WiFi.status() != WL_CONNECTED) {
                      Serial.println("Connecting..."+(String)ssid);
                      if (WiFi.begin(ssid, password)) {
                        int c=0;
                        while (WiFi.status() != WL_CONNECTED) {
                             delay(1300);
                              if(c>RETRY_TIMEOUT){Serial.print("RST");Serial.print('\r');}
                              Serial.print(">"+(String)c);digitalWrite(INDICATOR,SWAP());
                              c++;
                        }
                        Serial.println("WiFi connected");
                        Serial.println("IP address: ");
                        Serial.println(WiFi.localIP());
                      }
            }
            
    //check microgear
            if (!microgear.connected()) {
                 Serial.println("Connecting.... MQTT ");
                 microgear.connect(APPID);digitalWrite(INDICATOR,HIGH);
                       int x=0;
                        while (!microgear.connected()) {
                          delay(1900);
                            if(x>RETRY_TIMEOUT){Serial.print("RST");Serial.print('\r');}
                          x++;
                          Serial.println("Waiting microgear " + (String)x);
                        }      
                        Serial.flush();  
                        Serial.println("Net pie Connected");   
            }
           
  //recieve value      
  if(Serial.available() > 0){
      digitalWrite(INDICATOR,SWAP());
     // check incomingline
      while (Serial.available() > 0) {
          line="";
         line = Serial.readStringUntil('#'); 
      }
        
              if (line.length() > 3) {
                    digitalWrite(INDICATOR,SWAP());
                    microgear.loop();
                    microgear.chat(ALIAS, line);
              }
  }else{
     digitalWrite(INDICATOR,SWAP());
     delay(100);
  };
  
  // loop bind microger
              microgear.loop();
}

boolean SWAP(){
  if(OUT_PUT)
  {
    OUT_PUT=false;
  }else{
    OUT_PUT=true;
  }
  return OUT_PUT;
}



