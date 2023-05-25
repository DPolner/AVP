// Pin verteilen

// Gleichstrommotor 1 ansteuern
#define GM1 6   // muss PWM Pin sein
#define GM1in1 9
//#define GM1in1 30 // gelb
#define GM1in2 32

// Gleichstrommotor 2 ansteuern
#define GM2 5   // muss PWM Pin sein
#define GM2in1 4
//#define GM2in1 28 //blau
//#define GM2in2 7
#define GM2in2 26 //lila

// Infrarotsensoren Pins festlegen
#define IR_L A1
#define IR_R A0

// Farbsensorik Pins festlegen
#define S0_R 27
#define S1_R 29
#define S2_R 31
#define S3_R 33
#define sensorOut_R 35
#define LED_R 37

#define S0_L 38
#define S1_L 41
#define S2_L 43
#define S3_L 45
#define sensorOut_L 47
#define LED_L 49

//farben speichern
int redfrequency_L = 0;
int greenfrequency_L = 0;
int bluefrequency_L = 0;

int redfrequency_R = 0;
int greenfrequency_R = 0;
int bluefrequency_R = 0;

//Endrichtung
// 1 = blau| 2 = rot| 3 = gruen
int taster = 2; 

void setup() {

  //Setup für Farbsensorik LINKS
  pinMode(S0_L, OUTPUT);
  pinMode(S1_L, OUTPUT);
  pinMode(S2_L, OUTPUT);
  pinMode(S3_L, OUTPUT);
  pinMode(sensorOut_L, INPUT);
  pinMode(LED_L, OUTPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0_L,HIGH);
  digitalWrite(S1_L,LOW);

  //Setup für Farbsensorik RECHTS
  pinMode(S0_R, OUTPUT);
  pinMode(S1_R, OUTPUT);
  pinMode(S2_R, OUTPUT);
  pinMode(S3_R, OUTPUT);
  pinMode(sensorOut_R, INPUT);
  pinMode(LED_R, OUTPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0_R,HIGH);
  digitalWrite(S1_R,LOW);

  //Setup für Motor 1
  pinMode(GM1, OUTPUT);
  pinMode(GM1in1, OUTPUT);
  pinMode(GM1in2, OUTPUT);

  // Setup für Motor 2
  pinMode(GM2, OUTPUT);
  pinMode(GM2in1, OUTPUT);
  pinMode(GM2in2, OUTPUT);

  // Setup für IR Sensorik
  pinMode(IR_L, INPUT);
  pinMode(IR_R, INPUT);
  TCCR0B = TCCR0B & B11111000 | B00000010 ;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Code für Transport der Palette

  // Code für Auswahl wohin FTS sich bewegen muss

  // Code für Farberkennung
  abbiegen();

  // Code fürs Folgen der Linie

  //linieFolgen();
  
}

void linieFolgen()  {
  if((analogRead(IR_L) <= 500) && (analogRead(IR_R) <= 500)){
    forward();
    //Serial.println("vor");
  } else if((analogRead(IR_L) <= 500) && (analogRead(IR_R) >= 500)){
    right(100);
    //Serial.println("rechts");
  } else if((analogRead(IR_L) >= 500) && (analogRead(IR_R) <= 500)){
    left(100);
    //Serial.println("links");
  } else if((analogRead(IR_L) >= 500) && (analogRead(IR_R) >= 500)){
    stop();
    //Serial.println("stop");
  }
}

void abbiegen() {
  if (taster == farbeErkennen_L()) {
    stop();
    delay(100);
    if  (taster == farbeErkennen_L())  {
      turnLeft();
      linieFolgen();
      linieFolgen();
    } else {
      linieFolgen();
    }
  } else if (taster == farbeErkennen_R()) {
    stop();
    delay(100);
    if  (taster == farbeErkennen_R())  {
      turnRight();
      linieFolgen();
      linieFolgen();
    } else  {
      linieFolgen();
    }
  } else {
    linieFolgen();
  }
}


int farbeErkennen_L() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2_L,LOW);
  digitalWrite(S3_L,LOW);
  digitalWrite(LED_L,LOW); //LOW=Switch ON the 4 LEDs, HIGH=Switch off the 4 LEDs

  // Reading the output frequency
  redfrequency_L = pulseIn(sensorOut_L, LOW);

  delay(100);
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2_L,HIGH);
  digitalWrite(S3_L,HIGH);
  // Reading the output frequency
  greenfrequency_L = pulseIn(sensorOut_L, LOW);
  // Printing the value on the serial monitor
 
  delay(100);
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2_L,LOW);
  digitalWrite(S3_L,HIGH);
  // Reading the output frequency
  bluefrequency_L = pulseIn(sensorOut_L, LOW);

  if (redfrequency_L<20 && greenfrequency_L<20 && bluefrequency_L<20) {
    //Serial.println("NO COLOR DETECTION LEFT");  Test
    return 0;
  } else if (redfrequency_L > 40 && greenfrequency_L > 40 && bluefrequency_L > 40)  {
    return 0;
    //Serial.println("SChwarz COLOUR LEFT");  //Test
  } else if (bluefrequency_L<20 & redfrequency_L>20) {
    //Serial.println("BLUE COLOUR LEFT");  //Test
    return 1;
  } else if (redfrequency_L<20) {
    //Serial.println("RED COLOUR LEFT");  //Test
    return 2;
  } else if (redfrequency_L>30) {
    //Serial.println("GREEN COLOUR LEFT");  // Test
    return 3;
  } else {
    //Serial.println("FAILURE LEFT"); //Test
    return 0;
  } 
}

int farbeErkennen_R(){

  // Setting red filtered photodiodes to be read
  digitalWrite(S2_R,LOW);
  digitalWrite(S3_R,LOW);
  digitalWrite(LED_R,LOW); //LOW=Switch ON the 4 LEDs, HIGH=Switch off the 4 LEDs

  // Reading the output frequency
  redfrequency_R = pulseIn(sensorOut_R, LOW);

  delay(100);
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2_R,HIGH);
  digitalWrite(S3_R,HIGH);
  // Reading the output frequency
  greenfrequency_R = pulseIn(sensorOut_R, LOW);
  // Printing the value on the serial monitor
 
  delay(100);
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2_R,LOW);
  digitalWrite(S3_R,HIGH);
  // Reading the output frequency
  bluefrequency_R = pulseIn(sensorOut_R, LOW);


  if (redfrequency_R < 20 && greenfrequency_R < 20 && bluefrequency_R < 20) {
    //Serial.println("NO COLOR DETECTION RIGHT"); //Test
    return 0;
  } else if (redfrequency_R > 40 && greenfrequency_R > 40 && bluefrequency_R > 40)  {
    //Serial.println("SChwarz COLOUR LEFT");  //Test
    return 0;
  } else if (bluefrequency_R < 20 & redfrequency_R > 20) {
    //Serial.println("BLUE COLOUR RIGHT");  //Test
    return 1;
  } else if (redfrequency_R < 20) {
    //Serial.println("RED COLOUR RIGHT");  //Test
    return 2;
  } else if (redfrequency_R > 30) {
    //Serial.println("GREEN COLOUR RIGHT"); //Test
    return 3;
  } else {
    //Serial.println("FAILURE RIGHT");
    return 0;
  } 

}

void turnLeft(){
  while((analogRead(IR_L) <= 500)) {
    left(100);
  }
  while((analogRead(IR_R) <= 500)) {
    left(100);
  }
}

void turnRight(){
    while((analogRead(IR_R) <= 500)) {
    right(100);
  }
  while((analogRead(IR_L) <= 500)) {
    right(100);
  }
}

void forward(){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, HIGH);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 110);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, HIGH);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 110);
}

void left(int speed){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, HIGH);
  analogWrite(GM1, speed);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, HIGH);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, speed);
}

void right(int speed){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, HIGH);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, speed);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, HIGH);
  analogWrite(GM2, speed);
}

void stop(){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 255);
  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 255);
}