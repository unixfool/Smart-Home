#include <LiquidCrystal.h>
#include <Servo.h>
#include <IRremote.h>

const int PinReceptorIR = 2;
LiquidCrystal lcd(9, 7, 3, 4, 5, 6);
int LuzCasa = 8;
int Puerta = 10;
int MotorCC = 11;
int MicroServo = 12;
int PIRsensor = 13;
int Temp = A0;
int Luz = A1;
bool luzhabitacion = false;
bool modoauto = false;
int modopuerta = 0;
unsigned long Tiempo1 = 0;
unsigned long Tiempo2 = 0;
unsigned long Tiempo3 = 0;
int tiempocalib = 15;
bool cuartohabitado = false;
int iteracionespasadas = 0;
int angulopasado = 0;
int intspasadasvent = 0;
float temppasada = 0;

const long TimepoDeEspera = 4500;

IRrecv receptorIR(PinReceptorIR);
decode_results IRinput;

Servo Cortina;

void setup() {
Serial.begin(9600);
  
pinMode(8, OUTPUT);
receptorIR.enableIRIn();

Cortina.attach(MicroServo);
  
pinMode(MotorCC, OUTPUT);
pinMode(Puerta, OUTPUT);
pinMode(PIRsensor, INPUT);

  
lcd.begin(16, 2);
lcd.display();
  
Cortina.write(0);
  
digitalWrite(PIRsensor, LOW);
}

void loop() {
  unsigned long MilisActuales = millis();
  
  if (receptorIR.decode(&IRinput)) {
  Serial.println(IRinput.value, HEX);
    
    if (modoauto == false) { 


    if (IRinput.value == 0xFD807F) { 
    digitalWrite(LuzCasa, HIGH);
    luzhabitacion = true;
    lcd.clear();
    lcd.print("Light ON");
    delay(1000);  
    lcd.clear();
    }
    
    if (IRinput.value == 0xFD906F) {
    digitalWrite(LuzCasa, LOW);
    luzhabitacion = false;
    lcd.clear();
    lcd.print("Light OFF");
    delay(1000);  
    lcd.clear();
    }
    
    
    if (IRinput.value == 0xFD50AF) {
    Cortina.write(180);
    lcd.clear();
    lcd.print("Raised Curtains");
    delay(1500);  
    lcd.clear();
      
    Serial.print(Cortina.read());
    }
    
    if (IRinput.value == 0xFD10EF) {
    Cortina.write(0);
    lcd.clear();
    lcd.print("Curtains Lowered");
    delay(1500);  
    lcd.clear();
      
    Serial.print(Cortina.read());
    }
    
   
    if (IRinput.value == 0xFD609F) {
    digitalWrite(MotorCC, HIGH);
    lcd.clear();
    lcd.print("FAN");
    lcd.setCursor(0, 1);
    lcd.print("ON");
    delay(1500);  
    lcd.clear();
    }
    
    if (IRinput.value == 0xFD20DF) {
    digitalWrite(MotorCC, LOW);
    lcd.clear();
    lcd.print("FAN");
    lcd.setCursor(0, 1);
    lcd.print("OFF");
    delay(1500);  
    lcd.clear();  
    }

    }
    
    
    if (IRinput.value == 0xFDB04F) {
    lcd.clear();
    lcd.print("Enter");
    lcd.setCursor(0,1);
    lcd.print("Your Password");
    delay(1000);
    lcd.clear();
    modopuerta = 1;
    Tiempo1 = MilisActuales;
    }
      
    if (IRinput.value == 0xFD18E7 && modopuerta == 1) {
    modopuerta = 2;
    lcd.clear();
    lcd.print("Password (1/3)");
    lcd.setCursor(0,1);
    lcd.print("Received");
    delay(1000);
    lcd.clear();
    Tiempo2 = MilisActuales;
    }
    
    if (IRinput.value == 0xFD9867 && modopuerta == 2) {
    modopuerta = 3;
    lcd.clear();
    lcd.print("Password (2/3)");
    lcd.setCursor(0,1);
    lcd.print("Received");
    delay(1000);
    lcd.clear();
    Tiempo3 = MilisActuales;
    }
    
    if (IRinput.value == 0xFD58A7 && modopuerta == 3) {
    modopuerta = 4;
    digitalWrite(Puerta, HIGH);
    lcd.clear();
    lcd.print("Lpcl");
    lcd.setCursor(0, 1);
    lcd.print("Unlocked");
    delay(1500);  
    lcd.clear();
    }
    
    
    if (IRinput.value == 0xFD00FF) { 
      if (modoauto == false) { 
      modoauto = true; 
      lcd.clear();
      lcd.print("Automatic Mode");
      lcd.setCursor(0,1);
      lcd.print("Enabled");
      delay(2000);  
      lcd.clear();
      }
      
      else if (modoauto == true) { 
      modoauto = false; 
      lcd.clear();
      lcd.print("Automatic Mode");
      lcd.setCursor(0,1);
      lcd.print("Disabled");
      delay(2000);  
      lcd.clear();
      }
    }
    
  }  
  
 if (MilisActuales - Tiempo1 >= TimepoDeEspera && modopuerta == 1) {
 Tiempo1 = MilisActuales;
 lcd.clear();
 lcd.print("Time");
 lcd.setCursor(0, 1);
 lcd.print("is over");
 delay(1500);
 lcd.clear();
 modopuerta = 0;
 digitalWrite(Puerta, LOW);
 } 
  
 if (MilisActuales - Tiempo2 >= TimepoDeEspera && modopuerta == 2) {
 Tiempo2 = MilisActuales;
 lcd.clear();
 lcd.print("Time");
 lcd.setCursor(0, 1);
 lcd.print("is over");
 delay(1500);
 lcd.clear();
 modopuerta = 0;
  digitalWrite(Puerta, LOW);
 } 
  
 if (MilisActuales - Tiempo3 >= TimepoDeEspera && modopuerta == 3) {
 Tiempo3 = MilisActuales;
 lcd.clear();
 lcd.print("Time");
 lcd.setCursor(0, 1);
 lcd.print("is over");
 delay(1500);
 lcd.clear();
 modopuerta = 0;
 digitalWrite(Puerta, LOW);
 }  
  
 int luzdisponible = analogRead(Luz);
 
 if (modoauto == true) {
   int angulo = map(luzdisponible, 6, 679, 0, 180);
   delay(100);
   Cortina.write(angulo);
   
   if (angulopasado > angulo) { 
    lcd.clear();
    lcd.print("Lowering curtain");
    delay(400);  
    lcd.clear();
   }
   
   else if (angulopasado < angulo) {
    lcd.clear();
    lcd.print("Raising curtain");
    delay(400);  
    lcd.clear();  
   }
   
   if (iteracionespasadas >=1) {
   iteracionespasadas = 0;
   angulopasado = angulo;
   }
   
 iteracionespasadas++;
   
 
   float temperatura = analogRead(Temp);
   temperatura = temperatura / 1024;
   temperatura = temperatura * 5;
   temperatura = temperatura - 0.5;
   temperatura = temperatura * 100;
   
   if (temppasada < temperatura) { 
     if (temperatura > 25 && digitalRead(MotorCC) == LOW) {
     digitalWrite(MotorCC, HIGH);  
     lcd.clear();
     lcd.print("FAN");
     lcd.setCursor(0, 1);
     lcd.print("ON");
     delay(1000);  
     lcd.clear(); 
     }
   }
   
   else if (temppasada >= temperatura && temperatura <= 25) {
  
     if (temppasada > temperatura && digitalRead(MotorCC) == HIGH)  {
     lcd.clear();
     lcd.print("FAN");
     lcd.setCursor(0, 1);
     lcd.print("Off");
     delay(1000);  
     lcd.clear();   
     }
   digitalWrite(MotorCC, LOW); 
   }
     
   if (intspasadasvent >= 1) {
   intspasadasvent = 0;
   temppasada = temperatura;
   }
   
   intspasadasvent++;

    
     
     if (analogRead(Luz) < 50 && cuartohabitado == false) {
     	if(digitalRead(PIRsensor)== HIGH)  {
        cuartohabitado = true;
        lcd.clear();
        lcd.print("Lights ON");
        delay(2000);  
        lcd.clear();
        digitalWrite(LuzCasa, HIGH);
        delay(50);    
        }
     }
    
     else {    
       if (analogRead(Luz) >= 50) {
       digitalWrite(LuzCasa, LOW);
       }
         
 }
  
  
}
   

receptorIR.resume();

delay(500);
}
