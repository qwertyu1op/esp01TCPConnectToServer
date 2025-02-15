#include <string>

String response="";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}
void waitResponse(){
  delay(4000);
  while(Serial.available()>0){
      
      char rxData=(char)Serial.read();
      response+=rxData;//response will have the str
      Serial.println(response);
    }
  if(response=="OK"){
    Serial.println(response);
  }
  else{
    Serial.println("ERROR");
  }
    
}

void cipSendData(int dataSize, char* data){
  Serial.println("AT+CIPSEND=");
  Serial.println(dataSize);
  Serial.println("\r\n");
  waitResponse();
  if(response=="OK"){
    response="";
    for(int index=0;index<dataSize;index++){
      response+=data;
    }
    Serial.println(response);
  }
  else{
    Serial.println("ERROR");
  }

}

void loop() {
  
  
  
  /*
  Serial.println("AT\r\n");
  waitResponse();
  if(response=="O"){
    Serial.println("AT+RST\r\n");
    waitResponse();
    Serial.println("AT+CWMODE=1\r\n");
    waitResponse();
    Serial.println("AT+CWJAP?\r\n");
    waitResponse();
  }
  //Serial.println(response);
  //delay(4000);
  
  
  waitResponse();
  if(response=="No AP"){
    Serial.println("AT+CWJAP=\"SSID\",\"PASS\"");
    waitResponse();
    
    if(response=="WiFi Connected"){
      Serial.println("AT+CIPSTART=\"TCP\",\"IP\",80");
      waitResponse();
      cipSendData(6, "ping");
    }//eo wifi connected
    else{
      Serial.println("ERROR");
    }
  }//eo no ap
  */
}
