

#include <Servo.h>        //includo le librerie necessarie
#include "HX711.h"
Servo Servo1;             //indico il nome del servo e della cella di carico
HX711 scale;

int en1=13;               //inizializzo tutti i pin dei 2 ponti H
int en2=7;
int in11=12;
int in12=11;
int in21 =6;
int in22 =5;

int t1;                 //inizializzo le variabili necessarie per il secondo carrello
int t2;
bool pp=false;


const int LOADCELL_DOUT_PIN = 2;    //indico i pin del chip HX711
const int LOADCELL_SCK_PIN = 3;
long int reading;                   //la variabile che salverà il valore letto dalla cella di carico



void setup() {
Servo1.attach (10);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

pinMode(A0,INPUT);                  //indico tutte le modalità dei pin 
pinMode(9,INPUT_PULLUP);            //il pin 9 si occupa del pulsante del secchio ed è stato
pinMode(in21,OUTPUT);               //messo come input pullup per comodità per la realizzazione del circuito
pinMode(in22,OUTPUT); 
pinMode(en2,OUTPUT); 
Serial.begin(9600);

Servo1.write (0);
}

void carrello(){            //la funzione carrello gestisce i due input del ponte H
                            //per far andare il carrello dal secchio all'altoforno e poi ritornare
   
  digitalWrite(en2,HIGH); 
  digitalWrite(in21,HIGH);
  digitalWrite(in22,LOW);
  delay(800);
  digitalWrite(en2,LOW); 
  digitalWrite(in21,LOW);
  digitalWrite(in22,LOW);
  delay(1000);
  digitalWrite(en2,HIGH); 
  digitalWrite(in21,LOW);
  digitalWrite(in22,HIGH);
  delay(700);
  digitalWrite(en2,LOW); 
  digitalWrite(in21,LOW);
  digitalWrite(in22,LOW);   

}



   

  

int bilancia(){             //la funzione serve per leggere il valore di peso letto dalla bilancia
    reading = scale.read();
   // Serial.print("HX711 reading: ");
   // Serial.println(reading);
    return reading;
  }
  
void secchio(){             //la funzione si occupa di far aprire il secchio

  Servo1.write( 90);
  delay(1000);
  Servo1.write( 0);
  carrello();              //viene richiamata la funzione carrello così che quando viene premuto il pulsante il carrello si muova subito
  
  
  }
  


float tempAF(){                                //la funzione tempAF misura la temperatura dell'altoforno tramite una termoresistenza   
  int tempin;

  tempin=analogRead(A0);                       //legge il valore di tensione del circuito della termoresistenza

    float tempVolts = tempin * 5.0 / 1024.0;   //semplici proporzioni per convertire il valore di tensione in una temperatura
    int tempC = (tempVolts - 0.5) * 10.0;      //una formula molto approssimativa per convertire in Celsius
   
    Serial.write(tempC);                       //scrive sulla seriale il valore di temperatura in celsius
     //Serial.print(tempC); 
    delay(100);
  }

void carrelloaltoforno(){               //la funzione si occupa del movimento del secondo carrello
  digitalWrite(en1,HIGH); 
  digitalWrite(in11,HIGH);
  digitalWrite(in12,LOW);
  delay(800);
  digitalWrite(en1,LOW); 
  digitalWrite(in11,LOW);
  digitalWrite(in12,LOW);
  delay(1000);
  digitalWrite(en1,HIGH); 
  digitalWrite(in11,LOW);
  digitalWrite(in12,HIGH);
  delay(800);
  digitalWrite(en1,LOW); 
  digitalWrite(in11,LOW);
  digitalWrite(in12,LOW);    
  }



void loop() {




bilancia();                 //richiama la funzione bilancia
if(digitalRead(9)==LOW&&reading>260000){    //se viene premuto il pulsante ed è presente un carrello
t1=millis;                                  //viene salvato il tempo quando viene premuto il pulsante
secchio();                                  //viene richiamata la funzione secchio che lo fa aprire
pp=true;                                    
 }
 if(pp==true){                              //se il pulsante è stato già premuto
  t2=millis();                              //viene salvato il tempo trascorso

  if((t2-t1)>=2000){                        //nel caso siano passati 20 secondi
    carrelloaltoforno();                    //richiama la funzione per far partire il secondo carrello
    pp=false;
 
    }
   
    }
                            
tempAF();


}
