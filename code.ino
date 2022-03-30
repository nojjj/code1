#include <SoftwareSerial.h>
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

SoftwareSerial mySerial(12, 11); // RX, TX
//SoftwareSerial bluetooth(2, 3); // RX, TX//

int base = 1500;
String recu = "";
int tete = 1500;
int frequency = 0;
int r = 0, v = 0, b = 0;
int couleur = 0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);
  //bluetooth.begin(38400);//
  //nous avions tester pour le bluetooth pour l'application//

  Serial.println("Démarrage robot");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  <// ici, nous avons,les instructions, afin que le robot se mette a la bonne disposition au début//
  mySerial.println("#0P1500T2000");
  mySerial.println("#2P1500T2000");
  mySerial.println("#1P1500T2000");
  mySerial.println("#3P1500T2000");
  delay(2000);
}


void loop() {
  // rouge
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // ce qui s'affiche sur le moniteur série 
  Serial.print("R= ");//on va afficher la valeur de R 
  Serial.print(frequency);//fréquence associé au rouge 
  Serial.print("  ");
  r = frequency;
  delay(100);

  // vert
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  Serial.print("G= ");
  Serial.print(frequency);
  Serial.print("  ");
  v = frequency;
  delay(100);

  // bleu
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  frequency = pulseIn(sensorOut, HIGH);
  Serial.print("B= ");
  Serial.print(frequency);
  b = frequency;
  Serial.println("  ");

  if ( (r - v) <= 6 && (r - b) <= 6 && (v - b) <= 6 ) {
    Serial.println("recherche couleur");
    couleur = 0;
  }
  else if (r < v && r < b) {
    Serial.println("rouge");
    couleur = 1;
  } else if (v < r && v < b) {
    Serial.println("vert");
    couleur = 2;
  } else if (b < r && b < v) {
    Serial.println("bleu");
    couleur = 3;
  }
  delay(1000);




  /*if (bluetooth.available()) {
    recu = Serial.write(bluetooth.read());
    if (Serial.available() > 0) {
    recu = Serial.write(bluetooth.read());
    }*/

  if (Serial.available() > 0) {
    recu = Serial.readStringUntil('\n');  
    /*on récupère ce que l'utilisateur ecrit sur le moniteur série */
  }
  if (recu == "start") {
    Serial.println("début");
    mySerial.println("#1P1190T4000#3P1830T6000");
    delay (6000);
    mySerial.println("#0P1100T6000");
    delay(10000);
    recu = "";
    base = 1100;
  }


  if (recu == "rouge") {
/* on s'arrete à 2200, la distance entre 2200 et 2500 est reservé au place récupérer*/
    if (base < 2200) { 
      if (couleur == 1) {
        Serial.println("Rouge : trouvé");
        delay(3000);
        base = base + 60;
        /*on avance afin de pouvoir mettre la ventouse en face de la piece a recuperer */
        mySerial.println("#0P" + String(base) + "T1000");
        delay (2000);
        mySerial.println("#1P1140T5000");
        delay(5000); //on laisse le temps pour que l'on récupere la ventouse 
        mySerial.println("#1P1250T5000");
        delay(5000); //on remonte suffisament pour ne pas entrer en contact avec les autres pieces 
        mySerial.println("#0P2500T6000");
        delay(10000); /// on laisse le temps de déposer la piece
        mySerial.println("#1P1190T5000");

        mySerial.println("#0P" + String(base) + "T6000");  //on retourne à notre position où la blayage s'était arrêté pour récupérer la piece 
        delay(10000);
      }
      //mySerial.println("#1P1180T4000");//
      //sinon on continu le balayage ==> ce n'est pas la couleur recherché 
      mySerial.println("#0P" + String(base) + "T1000");
      Serial.println("#0P" + String(base) + "T1000");
      base = base + 50;
      delay (1000);

    }
  }

  if (recu == "vert") {

    if (base < 2200) {
      if (couleur == 2) {
        Serial.println("Vert : trouvé");
        delay(3000);
        /*mySerial.println("#1P1160T5000");
          delay(2000);*/
        base = base + 60;
        Serial.println(base);

        mySerial.println("#0P" + String(base) + "T1000");
        delay (2000);
        mySerial.println("#1P1140T5000");
        delay(5000); //aspirons la piece
        mySerial.println("#1P1250T5000");
        delay(5000); //ON REMONTE
        mySerial.println("#0P2500T6000");
        delay(10000); /// on laisse le temps de recuperer la piece
        mySerial.println("#1P1190T5000");

        mySerial.println("#0P" + String(base) + "T6000");
        delay(10000);
      }
      //mySerial.println("#1P1180T4000");//
      mySerial.println("#0P" + String(base) + "T1000");
      Serial.println("#0P" + String(base) + "T1000");
      base = base + 50;
      delay (1000);

    }
  }


  if (recu == "bleu") {

    if (base < 2200) {
      if (couleur == 3) {
        Serial.println("Bleu : trouvé");
        delay(3000);

        base = base + 60;
        Serial.println(base);

        mySerial.println("#0P" + String(base) + "T1000");
        delay (2000);
        mySerial.println("#1P1140T5000");
        delay(5000); //aspirons la piece
        mySerial.println("#1P1250T5000");
        delay(5000);
        mySerial.println("#0P2500T6000");
        delay(10000); 
        mySerial.println("#1P1190T5000");

        mySerial.println("#0P" + String(base) + "T6000");
        delay(10000);
      }
      //mySerial.println("#1P1180T4000");//
      mySerial.println("#0P" + String(base) + "T1000");
      Serial.println("#0P" + String(base) + "T1000");
      base = base + 50;
      delay (1000);

    }
  }



}
