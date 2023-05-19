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

void setup() {
  // put your setup code here, to run once:
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

  // Code fürs Folgen der Linie
  if((analogRead(IR_L) <= 500) && (analogRead(IR_R) <= 500)){
    forward();
    //Serial.println("vor");
  } else if((analogRead(IR_L) <= 500) && (analogRead(IR_R) >= 500)){
    right();
    //Serial.println("rechts");
  } else if((analogRead(IR_L) >= 500) && (analogRead(IR_R) <= 500)){
    left();
    //Serial.println("links");
  } else if((analogRead(IR_L) >= 500) && (analogRead(IR_R) >= 500)){
    stop();
    //Serial.println("stop");
  }

}



void forward(){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, HIGH);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 255);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, HIGH);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 255);
  /*
  delay(10);
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 150);
  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 150);
  // Code fürs abladen der Palette
  delay(10);
  */
}

void left(){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, HIGH);
  analogWrite(GM1, 255);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, HIGH);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 255);
/*
  delay(15);
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 150);
  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 150);
  // Code fürs abladen der Palette
  delay(15);
  */

}

void right(){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, HIGH);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 255);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, HIGH);
  analogWrite(GM2, 255);
  /*
  delay(15);
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 255);
  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 255);
  // Code fürs abladen der Palette
  delay(15);
  */
}

void stop(){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 150);
  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 150);
}