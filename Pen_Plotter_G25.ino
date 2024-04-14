#include <SCMD.h>
#include <SCMD_config.h>

#include <dummy.h>

#include <WiFi.h>
//#include <esp_system.h>
#include <Arduino.h>
#include <stdint.h>
#include "SCMD.h"
#include "SCMD_config.h" //Contains #defines for common SCMD register names and values.
#include "Wire.h"

SCMD myMotorDriver; //This creates the main object of one motor driver and connected peripherals.
const char* ssid     = "One";
const char* password = "12345678";


WiFiServer server(80); // Port 80

void comb1();
void comb2();
void comb3();
void comb4();
void comb5();
void speeds();
void timings();
int randomNumberArray[5]; 
int i=0;
int indexx=0;
int randomIndex;
int emerg=0;
bool notfound = true;
bool c= true;
bool d=false;
void (*combArray[])() = {comb1, comb2, comb3, comb4, comb5};
String state = "Pen plotter is ready";
String b;
String coordinates;
int speed;
String x, y;
int x_coord,y_coord;
int lmt1 = 21;
int lmt2 = 17;
int lmt3 = 16;
int lmt4 = 19;
int EMR=14;
unsigned long start=millis();
int lmt1read;
int lmt2read;
int lmt3read;
int lmt4read;
int px=225;int nx=225;int py=225;int ny=225;int nes=225;int nws=225;int ses=225;int sws=225;
int pxtt=300; int nxtt=300; int pytt=300; int nytt=300; int nett=300; int nwtt=300; int set=300; int swtt=300;
int wait30 = 30000; // time to reconnect when connection is lost.

void pos_x();
void neg_x();
void pos_y();
void neg_y();
void ne();
void nw();
void se();
void sw();
void stp();
void home();

void setup() {
  Serial.begin(9600);
  pinMode(lmt1, INPUT_PULLUP);
  pinMode(lmt2, INPUT_PULLUP);
  pinMode(lmt3, INPUT_PULLUP);
  pinMode(lmt4, INPUT_PULLUP);
  pinMode(EMR, INPUT_PULLUP);

  randomSeed(analogRead(A0));
  

// Connect WiFi net.
  Serial.println();
  Serial.print("Connecting with ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected with WiFi.");
 
  // Start Web Server.
  server.begin();
  Serial.println("Web Server started.");
 
  Serial.print("This is IP to connect to the WebServer: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());

  myMotorDriver.settings.commInterface = I2C_MODE;

  //  set address if I2C configuration selected with the config jumpers
  myMotorDriver.settings.I2CAddress = 0x5D; //config pattern is "1000" (default) on board for address 0x5D

  //  set chip select if SPI selected with the config jumpers
  myMotorDriver.settings.chipSelectPin = 10;

  Serial.println(myMotorDriver.begin());

  // *****initialize the driver get wait for idle*****//
  while ( myMotorDriver.begin() != 0xA9 ) //Wait until a valid ID word is returned
  {
    Serial.println( "ID mismatch, trying again" );
    delay(1000);
  }
  Serial.println( "ID matches 0xA9" );
  Serial.println(myMotorDriver.ready());

  //  Check to make sure the driver is done looking for peripherals before beginning
  Serial.println("Waiting for enumeration...");
  while ( myMotorDriver.ready() == false );
  Serial.println("Done.");
  Serial.println();

  while ( myMotorDriver.busy() );
  myMotorDriver.enable(); //Enables the output driver hardware
  Serial.println( "not busy" ); 

  #define LEFT_MOTOR 0
  #define RIGHT_MOTOR 1

}



void sw() { 
  // Perform the motor actions
  myMotorDriver.setDrive( LEFT_MOTOR, 0, 240);//nw dir
  myMotorDriver.setDrive( RIGHT_MOTOR, 1, 0);
}


void pos_y(){
  myMotorDriver.setDrive( LEFT_MOTOR, 1, 240);//-y dir
  myMotorDriver.setDrive( RIGHT_MOTOR, 0, 240);
}

void neg_y(){
  myMotorDriver.setDrive( LEFT_MOTOR, 0, 240);//+y dir
  myMotorDriver.setDrive( RIGHT_MOTOR, 1, 240);
}
void neg_y_hm(){
  myMotorDriver.setDrive( LEFT_MOTOR, 0, 200);//-x dir
  myMotorDriver.setDrive( RIGHT_MOTOR, 1, 200);
}

void ne(){
  myMotorDriver.setDrive( LEFT_MOTOR, 1, 240);//se dir
  myMotorDriver.setDrive( RIGHT_MOTOR, 0, 0);
}

void nw(){
  
  myMotorDriver.setDrive(LEFT_MOTOR, 1, 0);    // sw dir
  myMotorDriver.setDrive(RIGHT_MOTOR, 0, 240);
}

void neg_x(){
  
  myMotorDriver.setDrive( LEFT_MOTOR, 0, 240);//-x dir
  myMotorDriver.setDrive( RIGHT_MOTOR, 0, 240);
}

void pos_x(){
  myMotorDriver.setDrive( LEFT_MOTOR, 1, 240);//+x direction
  myMotorDriver.setDrive( RIGHT_MOTOR, 1, 240);
}

void neg_x_hm(){
  myMotorDriver.setDrive( LEFT_MOTOR, 0, 200);//+y dir
  myMotorDriver.setDrive( RIGHT_MOTOR, 0, 200);
}

void stp(){
  myMotorDriver.setDrive( LEFT_MOTOR, 0, 0); //Stop motor
  myMotorDriver.setDrive( RIGHT_MOTOR, 0, 0);
}

void se(){
  
  myMotorDriver.setDrive( LEFT_MOTOR, 0, 0);//ne dir
  myMotorDriver.setDrive( RIGHT_MOTOR, 1, 150);
  
}

void home(){
  int lmt1state = digitalRead(lmt1);
  int lmt2state = digitalRead(lmt4);
  while(lmt1state!=1){
    neg_x_hm();
    lmt1state = digitalRead(lmt1);
    delay(5);
  }
  Serial.println( "left" );
  pos_x();delay(200);stp();delay(600);
  while(lmt2state!=1){
    neg_y_hm();
    lmt2state = digitalRead(lmt4);
    delay(5);}
  Serial.println( "bottom" );
  pos_y();delay(200);stp();delay(600);}

void speeds(){
  px=225; nx=225; py=225; ny=225;  nes=225;  nws=225;  ses=225;  sws=225; 
}

void timings(){
  // pxt=300; nxt=300; pyt=300; nyt=300; net=300; nwt=300; set=300; swt=300;
}

void loop() {
  if ((WiFi.status() != WL_CONNECTED) && (millis() > wait30)) {
    Serial.println("Trying to reconnect WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    wait30 = millis() + 30000;
  } 
  // Check if a client has connected..
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  Serial.print("New client: ");
  Serial.println(client.remoteIP());
   
  // while(!client.available()){ delay(1); }

  // Read the information sent by the client.
  String req = client.readStringUntil('\r');
  Serial.println(req);
 
  

 
// for manual movement.
  if (req.indexOf("mmne") != -1) 
    {start= millis(); while((millis()-start<nett) && (digitalRead(lmt2)==0) && (digitalRead(lmt3)==0)){ne();}
     stp();   
    }

  if (req.indexOf("mmnw") != -1) 
    {start= millis(); while((millis()-start<nwtt) && (digitalRead(lmt1)==0)&& (digitalRead(lmt3)==0)){nw();}
     stp();   
    }

  if (req.indexOf("mmn") != -1) 
    {start= millis(); while((millis()-start<1) && (digitalRead(lmt3)==0)){pos_y();}
     stp();   
    }

  if (req.indexOf("mmnn") != -1) 
    {start= millis(); while((millis()-start<pytt) && (digitalRead(lmt3)==0)){pos_y();}
     stp(); 
    }

  
  if (req.indexOf("mmsw") != -1) 
    {start= millis(); while((millis()-start<swtt) && (digitalRead(lmt1)==0) && (digitalRead(lmt4)==0)){sw();}
     stp();  
     }

  if (req.indexOf("mmse") != -1) 
    {start= millis(); while((millis()-start<2000) && (digitalRead(lmt2)==0) && (digitalRead(lmt4)==0)){se();}
     stp();   
    }

  if (req.indexOf("mms") != -1) 
    {start= millis(); while((millis()-start<1) && (digitalRead(lmt4)==0)){neg_y();}
     stp();  
     } 

  if (req.indexOf("mmss") != -1) 
    {start= millis(); while((millis()-start<nytt) && (digitalRead(lmt4)==0)){neg_y();}
     stp();  
     }   

  if (req.indexOf("mme") != -1) 
    {start= millis(); while((millis()-start<pxtt) && (digitalRead(lmt2)==0)){pos_x();}
     stp();  
    }

  if (req.indexOf("mmw") != -1) 
    {start= millis(); while((millis()-start<nxtt) && (digitalRead(lmt1)==0)){neg_x();}
     stp();  
    }

// for home position
  if (req.indexOf("home") != -1) 
 {
  home();
  Serial.println("HOME POSITION");
  state="home position";
 }

    // drawing Nikolaus haus
  if (req.indexOf("draw") != -1 ) 
 {for(int plot=0;plot<1;plot++){
  Serial.println("executing pos_x");
  // home();pos_x();delay(500);stp();delay(600);pos_y();delay(400);stp();delay(600);
  start= millis(); while((millis()-start<1000) && (digitalRead(lmt1)+digitalRead(lmt2)+digitalRead(lmt3)+digitalRead(lmt4)==0) && (digitalRead(EMR)==1) && emerg==0){nw();if (req.indexOf("emer") != -1){emerg=1;state="Emergency stop pressed";}}  lmt1read=digitalRead(lmt1);lmt2read=digitalRead(lmt2); lmt3read=digitalRead(lmt3);lmt4read=digitalRead(lmt4);if((lmt1read==1) || (lmt2read==1) || (lmt3read==1) || (lmt4read==1) || (digitalRead(EMR)==0) || emerg==1){break;}stp(); delay(1000);
  start= millis(); while((millis()-start<800) && (digitalRead(lmt1)+digitalRead(lmt2)+digitalRead(lmt3)+digitalRead(lmt4)==0) && (digitalRead(EMR)==1) && emerg==0){pos_x();if (req.indexOf("emer") != -1){emerg=1;state="Emergency stop pressed";}}lmt1read=digitalRead(lmt1); lmt2read=digitalRead(lmt2); lmt3read=digitalRead(lmt3); lmt4read=digitalRead(lmt4);if((lmt1read==1) || (lmt2read==1) || (lmt3read==1) || (lmt4read==1) || (digitalRead(EMR)==0) || emerg==1){break;}stp(); delay(1000);
  start= millis(); while((millis()-start<400) && (digitalRead(lmt1)+digitalRead(lmt2)+digitalRead(lmt3)+digitalRead(lmt4)==0) && (digitalRead(EMR)==1) && emerg==0){nw();if (req.indexOf("emer") != -1){emerg=1;state="Emergency stop pressed";}}lmt1read=digitalRead(lmt1); lmt2read=digitalRead(lmt2); lmt3read=digitalRead(lmt3); lmt4read=digitalRead(lmt4);if((lmt1read==1) || (lmt2read==1) || (lmt3read==1) || (lmt4read==1) || (digitalRead(EMR)==0) || emerg==1){break;}stp(); delay(1000);
  start= millis(); while((millis()-start<800) && (digitalRead(lmt1)+digitalRead(lmt2)+digitalRead(lmt3)+digitalRead(lmt4)==0) && (digitalRead(EMR)==1) && emerg==0){sw();if (req.indexOf("emer") != -1){emerg=1;state="Emergency stop pressed";}}lmt1read=digitalRead(lmt1); lmt2read=digitalRead(lmt2); lmt3read=digitalRead(lmt3); lmt4read=digitalRead(lmt4);if((lmt1read==1) || (lmt2read==1) || (lmt3read==1) || (lmt4read==1) || (digitalRead(EMR)==0) || emerg==1){break;}stp(); delay(1000);
  start= millis(); while((millis()-start<380) && (digitalRead(lmt1)+digitalRead(lmt2)+digitalRead(lmt3)+digitalRead(lmt4)==0) && (digitalRead(EMR)==1) && emerg==0){neg_y();if (req.indexOf("emer") != -1){emerg=1;state="Emergency stop pressed";}}lmt1read=digitalRead(lmt1); lmt2read=digitalRead(lmt2); lmt3read=digitalRead(lmt3); lmt4read=digitalRead(lmt4);if((lmt1read==1) || (lmt2read==1) || (lmt3read==1) || (lmt4read==1) || (digitalRead(EMR)==0) || emerg==1){break;}stp(); delay(1000);
  start= millis(); while((millis()-start<550) && (digitalRead(lmt1)+digitalRead(lmt2)+digitalRead(lmt3)+digitalRead(lmt4)==0) && (digitalRead(EMR)==1) && emerg==0){ne();if (req.indexOf("emer") != -1){emerg=1;state="Emergency stop pressed";}}lmt1read=digitalRead(lmt1); lmt2read=digitalRead(lmt2); lmt3read=digitalRead(lmt3); lmt4read=digitalRead(lmt4);if((lmt1read==1) || (lmt2read==1) || (lmt3read==1) || (lmt4read==1) || (digitalRead(EMR)==0) || emerg==1){break;}stp(); delay(1000);
  // start= millis(); while((millis()-start<300) && (digitalRead(lmt1)+digitalRead(lmt2)+digitalRead(lmt3)+digitalRead(lmt4)==0) && (digitalRead(EMR)==1) && emerg==0){neg_y();if (req.indexOf("emer") != -1){emerg=1;state="Emergency stop pressed";}}lmt1read=digitalRead(lmt1); lmt2read=digitalRead(lmt2); lmt3read=digitalRead(lmt3); lmt4read=digitalRead(lmt4);if((lmt1read==1) || (lmt2read==1) || (lmt3read==1) || (lmt4read==1) || (digitalRead(EMR)==0) || emerg==1){break;}stp(); delay(1000);
  // start= millis(); while((millis()-start<350) && (digitalRead(lmt1)+digitalRead(lmt2)+digitalRead(lmt3)+digitalRead(lmt4)==0) && (digitalRead(EMR)==1) && emerg==0){neg_x();if (req.indexOf("emer") != -1){emerg=1;state="Emergency stop pressed";}}lmt1read=digitalRead(lmt1); lmt2read=digitalRead(lmt2); lmt3read=digitalRead(lmt3); lmt4read=digitalRead(lmt4);if((lmt1read==1) || (lmt2read==1) || (lmt3read==1) || (lmt4read==1) || (digitalRead(EMR)==0) || emerg==1){break;}stp(); delay(1000);
  state = "Nikolaus Haus completed";c=false;d=true;emerg=0;}Serial.println("out of loop");
  if(c){state="Interrupted";while(digitalRead(EMR)==1){}home();}
  // if(d==true){while(digitalRead(EMR)==1){}home();}
  c=true;
  d=false;
  // pos_y();delay(350);stp();delay(1000);neg_x();delay(185);stp();  delay(1000);neg_y();delay(350);stp();  delay(1000);
  // Serial.println("executing ne");
  // ne();delay(720);stp();  delay(1000);nw();delay(280);stp();delay(1000);sw();delay(615);stp();  delay(1000);se();delay(800);stp();delay(1000);

  // pos_x();delay(280);stp();delay(1000);pos_y();delay(350);stp();delay(1000);sw();delay(615);stp();delay(1000);pos_y();delay(350);stp();delay(1000);
  // pos_x();delay(280);stp();delay(1000);nw();delay(280);stp();delay(1000);sw();delay(615);stp();delay(1000);se();delay(800);stp();delay(1000);

  // pos_x();delay(280);stp();delay(1000);pos_y();delay(350);stp();delay(1000);nw();delay(280);stp();delay(1000);sw();delay(615);stp();delay(1000);
  // pos_x();delay(280);stp();delay(1000);sw();delay(1000);stp();delay(1000);pos_y();delay(350);stp();delay(1000);se();delay(800);stp();delay(1000);

  // pos_x();delay(280);stp();delay(1000);nw();delay(540);stp();delay(1000);ne();delay(250);stp();delay(1000);se();delay(250);stp();delay(1000);
  // sw();delay(615);stp();  delay(1000);pos_y();delay(350);stp();delay(1000);pos_x();delay(280);stp();delay(1000);neg_y();delay(350);stp();delay(1000); 
  

  // for (int i = 0; i < sizeof(combArray) / sizeof(combArray[0]); i++){ 
  //     randomIndex = random(0, sizeof(combArray) / sizeof(combArray[0])); // Exit the loop since the value is already found

  //    for (int j = 0; j < sizeof(combArray) / sizeof(combArray[0]); j++){
  //      if (randomNumberArray[j] == randomIndex) 
  //     {
  //       notfound=false;

  //       }
     
  //     }
   
  //     if (notfound){
  //       combArray[randomIndex]();
  //       randomNumberArray[indexx]=randomIndex;
  //       indexx++;

  //       break;}
      
  //     if(indexx==4){
  //       Serial.println("Array is full. Elements: ");
  //       for (int l=0; l<5;l++){
  //         Serial.print(randomNumberArray[l]);
  //         Serial.print(" ");
  //       }
  //       Serial.println();
  //       indexx=0;
  //       memset(randomNumberArray,0,sizeof(randomNumberArray));
  //       }

  //   notfound=true;
  //    }

  }

    // set coordinates
	if (req.indexOf("+") != -1  ){
    req.replace("+", " ");          
    req.replace(" HTTP/1.1", "");   
    req.replace("GET /", ""); 
    coordinates=req;
    Serial.println(coordinates);
    int spaceIndex = coordinates.indexOf(' ');

  if (spaceIndex != -1) {
    home();
    x = coordinates.substring(0, spaceIndex);
    y = coordinates.substring(spaceIndex + 1);
    x_coord = x.toInt();
    y_coord = y.toInt();
    Serial.print("x_coord: ");
    Serial.println(x_coord);
    Serial.print("y_coord: ");
    Serial.println(y_coord);
    int edelay=x_coord*(200/1.5);
    int ndelay=y_coord*(200/1.5);
    pos_x();delay(edelay);stp();delay(500);
    pos_y();delay(ndelay);stp();delay(500);
    
  }

  }

  if (req.indexOf("defspeed") != -1  ){
    speeds();timings();
    state="Default speed set";
  }

  if (req.indexOf("speed") != -1  ){
    req.replace("speed", "");          
    req.replace(" HTTP/1.1", "");   
    req.replace("GET /", ""); 
    speed=req.toInt(); 
    Serial.print("Speed is set to ");
    Serial.println(speed);
    px=speed; nx=speed; py=speed; ny=speed;  nes=speed;  nws=speed;  ses=speed;  sws=speed;
    // pxt=(300/225)*speed; nxt=(300/225)*speed; pyt=(300/225)*speed; nyt=(300/225)*speed; 
    // net=(300/225)*speed; nwt=(300/225)*speed; set=(300/225)*speed; swt=(300/225)*speed;
    state=req;
  }

  if (req.indexOf("pxt") != -1  ){
  req.replace("pxt", "");          
  req.replace(" HTTP/1.1", "");   
  req.replace("GET /", ""); 
  pxtt=req.toInt(); 
  Serial.print("pxt is:  ");
  Serial.println(pxtt);
  state=req;
  }

  if (req.indexOf("nxt") != -1  ){
  req.replace("nxt", "");          
  req.replace(" HTTP/1.1", "");   
  req.replace("GET /", ""); 
  nxtt=req.toInt(); 
  Serial.print("nxt is:  ");
  Serial.println(nxtt);
  state=req;
  }

  if (req.indexOf("pyt") != -1  ){
  req.replace("pyt", "");          
  req.replace(" HTTP/1.1", "");   
  req.replace("GET /", ""); 
  pytt=req.toInt(); 
  Serial.print("pyt is:  ");
  Serial.println(pytt);
  state=req;
  }

  if (req.indexOf("nyt") != -1  ){
  req.replace("nyt", "");          
  req.replace(" HTTP/1.1", "");   
  req.replace("GET /", ""); 
  nytt=req.toInt(); 
  Serial.print("nyt is:  ");
  Serial.println(nytt);
  state=req;
  }

  if (req.indexOf("net") != -1  ){
  req.replace("net", "");          
  req.replace(" HTTP/1.1", "");   
  req.replace("GET /", ""); 
  nett=req.toInt(); 
  Serial.print("net is:  ");
  Serial.println(nett);
  state=req;
  }

  if (req.indexOf("nwt") != -1  ){
  req.replace("nwt", "");          
  req.replace(" HTTP/1.1", "");   
  req.replace("GET /", ""); 
  nwtt=req.toInt(); 
  Serial.print("nwt is:  ");
  Serial.println(nwtt);
  state=req;
  }
  
  if (req.indexOf("swt") != -1  ){
  req.replace("swtt", "");          
  req.replace(" HTTP/1.1", "");   
  req.replace("GET /", ""); 
  swtt=req.toInt(); 
  Serial.print("swt is:  ");
  Serial.println(swtt);
  state=req;
  }

  if (req.indexOf("ipset") != -1  ){
    Serial.println("Connected to HMI");
    state="Pen plotter is ready";
  }

  //  WEB PAGE. ////////////////////////////

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println(state);

  Serial.print("Client disconnected: ");
  Serial.println(client.remoteIP());
  client.flush();
  client.stop();
  if ((state == "Nikolaus Haus completed") || (state == "home position")){
      state="Pen plotter is ready";
  } 
}
