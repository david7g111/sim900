#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // Configura el puerto serial para el SIM GSM
// variables
char incoming_char=0; //Variable que guarda los caracteres que envia el SIM GSM
int salir = 0;
String msm = "";
int onoff = 2;
int rsa = 3;
int hdmi = 4;

void setup() {
  //setyp pins
  pinMode(onoff, INPUT_PULLUP);
  pinMode(rsa, INPUT_PULLUP);
  pinMode(hdmi, INPUT_PULLUP);
  pinMode(onoff, OUTPUT);
  pinMode(rsa, OUTPUT);
  pinMode(hdmi, OUTPUT);
  // put your setup code here, to run once:
  SIM900.begin(19200); //Configura velocidad serial para el SIM
  //delay(25000);
  //SIM900.print("AT+IPR=19200\r");
  delay(10000); //Retardo para que encuentra a una RED
  Serial.begin(19200); //Configura velocidad serial para el Arduino
  Serial.println("RADIO"); //Mensaje OK en el arduino, para saber que todo va bien.
  //Serial.println(test());
  if(test()==false){
     startPhone();
     Serial.println("BOOT UP");
    }
    
  modo_recibe_mensaje();
 delay(1000);
}
int i;
char command;
int tester = 0;
void loop() {
    msm = "";
    while(SIM900.available()>0){
       incoming_char=SIM900.read(); 
       msm.concat(incoming_char);
       Serial.print(incoming_char); 
    };
    // parse the message
    if(msm.indexOf("CMT")>0){ // check if is message
      i = msm.indexOf("@567");
      if(i>0){// check tha password
        // get payload
        i=i+4;
        command = msm.charAt(i);
        // make actions
        if(command =='S'){ // DEFAULT
          Serial.println("SATELLITE");
          digitalWrite(rsa,HIGH);
          // TOOGLE HDMI 
          digitalWrite(hdmi,LOW);
          delay(500);
          digitalWrite(hdmi,HIGH);
          
          }else if(command == 'L'){
          Serial.println("LOCAL");
          digitalWrite(rsa,LOW);
          // TOOGLE HDMI 
          digitalWrite(hdmi,LOW);
          delay(500);
          digitalWrite(hdmi,HIGH); // default state
          }
          else if(command == 'A'){ // POWER ON equipments
          Serial.println("POWER ON");
          digitalWrite(onoff, LOW);
          }
          else if(command == 'B'){ // POWER OFF
          Serial.println("POWER OFF");
          digitalWrite(onoff, HIGH);
          
          }
          else if(command == 'R'){
          Serial.println("REPEAT HDMI");
          digitalWrite(hdmi,LOW);
          delay(500);
          digitalWrite(hdmi,HIGH); // default state
          
          }
          else if(command == 'T'){
          Serial.println("TURN OFF ALL");
          digitalWrite(hdmi,LOW);
          delay(500);
          digitalWrite(hdmi,HIGH); // default state
          
          digitalWrite(rsa,HIGH);
          delay(1000);
          digitalWrite(onoff, HIGH);

          
          }
        
        }
      }
    if(tester > 500){
       if(test()==false){
          startPhone();
          Serial.println("BOOT UP");
          tester = 0;
         }
      }
    tester +=1;
    Serial.println(tester);
    delay(2000);
}

void startPhone(){
  digitalWrite(9,HIGH);
  delay(1000);
  digitalWrite(9,LOW);
  delay(5000);
}

//Funcion para mandar mensaje de texto
void mensaje_sms(){
    SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
    delay(100);
    SIM900.println("AT+CMGS=\"33XXXXXXXX\""); // recipient's mobile number, in international format
    delay(100);
    SIM900.println("Saludos desde HetPro"); // message to send
    delay(100);
    SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
    delay(100);
    SIM900.println();
    delay(5000); // Tiempo para que se envie el mensaje
    Serial.println("SMS sent successfully");
}

void espera_mensaje(){
    salir = 1;
    while(salir==1){
        if(SIM900.available()>0){
            incoming_char=SIM900.read(); //Get the character from the cellular serial port.
            Serial.print(incoming_char); //Print the incoming character to the terminal.
            salir = 0;
        }
    }
}


void modo_recibe_mensaje(){
    //Configura el modo texto para enviar o recibir mensajes
    SIM900.print("AT+CMGF=1\r"); // set SMS mode to text
    delay(100);
    SIM900.print("AT+CNMI=2,2,0,0,0\r");
    // blurt out contents of new SMS upon receipt to the GSM shield's serial out
    delay(500);
    while(SIM900.available()>0){
       incoming_char=SIM900.read(); //Get the character from the cellular serial port.
       Serial.print(incoming_char); //Print the incoming character to the terminal.
      }
    delay(1000);
}
bool test(){
      String value="";
      char ch;
      SIM900.print("AT\r");
      delay(500);
      while(SIM900.available()>0){
         ch = SIM900.read();
         //Serial.print(ch);
         value.concat(ch); //Get the character from the cellular serial port.
      }
      //Serial.println("");
      Serial.println("TEST");
      Serial.println(value);
      if(value.indexOf("OK")>0){
        return true;
      }else{
        return false;
      }
  }
