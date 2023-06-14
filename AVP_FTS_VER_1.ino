// Pin verteilen

// Gleichstrommotor 1 ansteuern | Motorshield 1 Motor vorne links
#define GM1 6   // muss PWM Pin sein
#define GM1in1 9
#define GM1in2 32

// Gleichstrommotor 2 ansteuern | Motorshield 1 Motor vorne rechts
#define GM2 5   // muss PWM Pin sein
#define GM2in1 4  // muss PWM Pin sein
#define GM2in2 26 

// Gleichstrommotor 3 ansteuern | Motorschield 2 Motor hinten links
#define GM3 8   // muss PWM Pin sein
#define GM3in1 7  // muss PWM Pin sein
#define GM3in2 40

// Gleichstrommotor 4 ansteuern | Motorschield 2 Motor hinten rechts
#define GM4 11   // muss PWM Pin sein
#define GM4in1 10 // muss PWM Pin sein
#define GM4in2 42

// Förderbandmotor ansteuern  | Motorschield 3 
#define GMF 3
#define GMFin1 2
#define GMFin2 52

// Infrarotsensoren Pins festlegen
// IR Sensoren die vorne sind
#define IR_v_L A1
#define IR_v_R A0

// IR Sensoren die hinten sind
#define IR_h_L A15
#define IR_h_R A14

// IR Sensoren die am Förderer sind
#define IRF_v A2
#define IRF_h A13

// Farbsensorik Pins festlegen
// rechter Farbsensor
#define S0_R 27
#define S1_R 29
#define S2_R 31
#define S3_R 33
#define sensorOut_R 35
#define LED_R 37

// linker Farbsensor
#define S0_L 38
#define S1_L 41
#define S2_L 43
#define S3_L 45
#define sensorOut_L 47
#define LED_L 49

// Pins für Taster festlegen
#define Taster_vorwaerts 23
#define Taster_ruckwaerts 25
#define Taster_rot 24
#define Taster_gruen 28
#define Taster_blau 22
#define Taster_silber 30

//farben speichern
int redfrequency_L = 0;
int greenfrequency_L = 0;
int bluefrequency_L = 0;

int redfrequency_R = 0;
int greenfrequency_R = 0;
int bluefrequency_R = 0;

// Endrichtung
// 1 = blau| 2 = rot| 3 = gruen | 4 = grau
int Zielstation = 0; 

// Fahrtrichtung
// 1 = vorwärts | 2 = rückwärts
int Fahrtrichtung = 0;
// keine Leerfahrt = 0 | Leerfahrt = 1
int Leerfahrt = 0;
void setup() {

  // Steup für Taster
  pinMode(Taster_vorwaerts, INPUT_PULLUP);
  pinMode(Taster_ruckwaerts, INPUT_PULLUP);
  pinMode(Taster_rot, INPUT_PULLUP);
  pinMode(Taster_gruen, INPUT_PULLUP);
  pinMode(Taster_blau, INPUT_PULLUP);
  pinMode(Taster_silber, INPUT_PULLUP);

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

  //Setup für Motor 3
  pinMode(GM3, OUTPUT);
  pinMode(GM3in1, OUTPUT);
  pinMode(GM3in2, OUTPUT);

  // Setup für Motor 4
  pinMode(GM4, OUTPUT);
  pinMode(GM4in1, OUTPUT);
  pinMode(GM4in2, OUTPUT);

  // Setup für Förderbandmotor
  pinMode(GMF, OUTPUT);
  pinMode(GMFin1, OUTPUT);
  pinMode(GMFin2, OUTPUT);

  // Setup für IR Sensorik
  pinMode(IR_v_L, INPUT);
  pinMode(IR_v_R, INPUT);
  pinMode(IR_h_L, INPUT);
  pinMode(IR_h_R, INPUT);
  pinMode(IRF_v, INPUT);
  pinMode(IRF_h, INPUT);

  TCCR0B = TCCR0B & B11111000 | B00000010 ;
  Serial.begin(9600);
}

void loop() {

  while (Fahrtrichtung == 0) {
    if (digitalRead(Taster_vorwaerts) == LOW) { //Fahrtrichtung auslesen
      Fahrtrichtung = 1;
      Leerfahrt = 1;
      break;
    } else if (digitalRead(Taster_ruckwaerts) == LOW) {
      Fahrtrichtung = 2;
      Leerfahrt = 1;
      delay(10);
      break;
    } else if (analogRead(IRF_v) < 500){
      Fahrtrichtung = 2;
      foerdern_aufnahme();
      break;
    } else if (analogRead(IRF_h) < 500){
      Fahrtrichtung = 1;
      foerdern_aufnahme();
      break;
    }
    else {
      Fahrtrichtung = 0;
    }
  }

  while (Zielstation == 0)  {
    if (digitalRead(Taster_rot) == LOW) { //Zielstation auslesen
      Zielstation = 2;
      break;
    } else if (digitalRead(Taster_gruen) == LOW) {
      Zielstation = 3;
      delay(1000);
      break;
    } else if (digitalRead(Taster_blau) == LOW) {
      Zielstation = 1;
      delay(1000);
      break;
    } else if (digitalRead(Taster_silber) == LOW) {
      Zielstation = 4;
      delay(1000);
      break;
    } else {
      Zielstation = 0;
    }
  }

  //foerdern();
  abbiegen();
}


void linieFolgen()  {
  if(Fahrtrichtung == 1){
    if((analogRead(IR_v_L) <= 500) && (analogRead(IR_v_R) <= 500)){
      forward();
    } else if((analogRead(IR_v_L) <= 500) && (analogRead(IR_v_R) >= 500)){
      right(255);
    } else if((analogRead(IR_v_L) >= 500) && (analogRead(IR_v_R) <= 500)){
      left(255);
    } else if((analogRead(IR_v_L) >= 500) && (analogRead(IR_v_R) >= 500)){
      stop(1);
    }
  } else if(Fahrtrichtung == 2){
      if((analogRead(IR_h_L) <= 500) && (analogRead(IR_h_R) <= 500)){
        backward();
      } else if((analogRead(IR_h_L) <= 500) && (analogRead(IR_h_R) >= 500)){
        right(255);
      } else if((analogRead(IR_h_L) >= 500) && (analogRead(IR_h_R) <= 500)){
        left(255);
      } else if((analogRead(IR_h_L) >= 500) && (analogRead(IR_h_R) >= 500)){
        stop(1);
      }
    }
  }


void abbiegen() {
if (Fahrtrichtung == 1){
  if (Zielstation == farbeErkennen_L()) {
    stop(0);
    delay(100);
    if  (Zielstation == farbeErkennen_L())  {
      turnLeft();
      linieFolgen();
      linieFolgen();
      linieFolgen();
      linieFolgen();
    } else {
      linieFolgen();
    }
  } else if (Zielstation == farbeErkennen_R()) {
    stop(0);
    delay(100);
    if  (Zielstation == farbeErkennen_R())  {
      turnRight();
      linieFolgen();
      linieFolgen();
      linieFolgen();
      linieFolgen();
    } else  {
      linieFolgen();
    }
  } else {
    linieFolgen();
  }
} else if (Fahrtrichtung == 2) {
  if (Zielstation == farbeErkennen_L()) {
    stop(0);
    delay(100);
    if  (Zielstation == farbeErkennen_L())  {
      turnRight();
      linieFolgen();
      linieFolgen();
      linieFolgen();
      linieFolgen();
    } else {
      linieFolgen();
    }
  } else if (Zielstation == farbeErkennen_R()) {
    stop(0);
    delay(100);
    if  (Zielstation == farbeErkennen_R())  {
      turnLeft();
      linieFolgen();
      linieFolgen();
      linieFolgen();
      linieFolgen();
    } else  {
      linieFolgen();
    }
  } else {
    linieFolgen();
  }
}
}

void foerdern_aufnahme(){
    while(true) {
      if (Fahrtrichtung == 1) {
        if (analogRead(IRF_v) >= 500) {
          f_forward();
        } else if (analogRead(IRF_v) < 500){
          f_stop();
          break;
        }
      } else if ( Fahrtrichtung == 2) {
          if (analogRead(IRF_h) >= 500) {
            f_backward();
          } else if (analogRead(IRF_h) < 500){
            f_stop();
            break;
        }
      } else {
        break;
      }
    }
}
void foerdern_abgabe(){
  while(true){
    if (Fahrtrichtung == 1) {
      if (analogRead(IRF_v) < 500) {
        delay(1000);
        f_forward();
      } else if (analogRead(IRF_v) > 500) {
        f_stop();
        break;
      }
    } else if (Fahrtrichtung == 2) {
      if (analogRead(IRF_h) < 500) {
        delay(1000);
        f_backward();
      } else if (analogRead(IRF_h) > 500){
        f_stop();
        break;
      }
    } else {
      break;
    }
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

  // linker sensor
  if (redfrequency_L<15 && greenfrequency_L<15 && bluefrequency_L<15) { // keine Farbe 
    return 0;
  } else if (redfrequency_L > 40 && greenfrequency_L > 40 && bluefrequency_L > 40)  { // schwarze Farbe
    return 0;
  } else if (bluefrequency_L < 20 && redfrequency_L > 30 && greenfrequency_L > 22) {  // blaue Farbe
    return 1;
  } else if (bluefrequency_L > 25 && redfrequency_L < 20 && greenfrequency_L > 35) {  // rote Farbe
    return 2;
  } else if (bluefrequency_L > 25 && redfrequency_L > 35 && greenfrequency_L > 22) {  // grüne Farbe
    return 3;
  } else if (bluefrequency_L < 22 && redfrequency_L < 25 && greenfrequency_L < 25 && bluefrequency_L > 12 && redfrequency_L > 15 && greenfrequency_L > 15) {  // graue/ silberne Farbe
    return 4;
  } else {  // Fehler/ keine Farbe
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

  // rechter Sensir
  if (redfrequency_R < 15 && greenfrequency_R < 15 && bluefrequency_R < 15) { // keine Farbe
    return 0;
  } else if (redfrequency_R > 40 && greenfrequency_R > 40 && bluefrequency_R > 40)  { // schwarze Farbe
    return 0;
  } else if (bluefrequency_R < 20 && redfrequency_R > 30 && greenfrequency_R > 23) {  // blaue Farbe
    return 1;
  } else if (bluefrequency_R > 25 && redfrequency_R < 20 && greenfrequency_R > 35) {  // rote Farbe
    return 2;
  } else if (bluefrequency_R > 25 && redfrequency_R > 35 && greenfrequency_R > 22) {  // grüne Farbe
    return 3;
  } else if (bluefrequency_R < 25 && redfrequency_R < 25 && greenfrequency_R < 25 && bluefrequency_R > 14 && redfrequency_R > 15 && greenfrequency_R > 15)  { // graue/ silberne Farbe
    return 4;
  } else {  // Fehler/ keine Farbe
    return 0;
  } 

}

void turnLeft(){
  if (Fahrtrichtung == 1) {
    while((analogRead(IR_v_L) <= 500)) {
      left(255);
    }
    while((analogRead(IR_v_R) <= 500)) {
      left(255);
    }
  } else if (Fahrtrichtung == 2) {
    while((analogRead(IR_h_L) <= 500)) {
      left(255);
    }
    while((analogRead(IR_h_R) <= 500)) {
      left(255);
    }
  }
}

void turnRight(){
  if (Fahrtrichtung == 1) {
    while((analogRead(IR_v_R) <= 500)) {
      right(255);
    }
    while((analogRead(IR_v_L) <= 500)) {
      right(255);
    }
  } else if (Fahrtrichtung == 2) {
      while((analogRead(IR_v_R) <= 500)) {
      right(255);
    }
    while((analogRead(IR_v_L) <= 500)) {
      right(255);
    }
  }
}

void forward(){
  int speed;
  if (Leerfahrt ==1) {  // Geschwindigkeit während Leerfahrt
    speed = 90;
  } else if (Leerfahrt == 0) {  // Geschwindigkeit während beladener Fahrt
    speed = 100;
  }
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, HIGH);
  analogWrite(GM1, speed);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, HIGH);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, speed);

  // Motor 3 wird angesteuert
  digitalWrite(GM3in1, LOW);
  digitalWrite(GM3in2, HIGH);
  analogWrite(GM3, speed);

  //Motor 4 wird angesteuert
  digitalWrite(GM4in1, HIGH);
  digitalWrite(GM4in2, LOW);
  analogWrite(GM4, speed);
}

void backward(){
  int speed;
  if (Leerfahrt == 1) { // Geschwindigkeit während Leerfahrt
    speed = 90;
  } else if (Leerfahrt == 0) {  // Geschwindigkeit während beladener Fahrt
    speed = 100;
  }
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, HIGH);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, speed);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, HIGH);
  analogWrite(GM2, speed);

  // Motor 3 wird angesteuert
  digitalWrite(GM3in1, HIGH);
  digitalWrite(GM3in2, LOW);
  analogWrite(GM3, speed);

  //Motor 4 wird angesteuert
  digitalWrite(GM4in1, LOW);
  digitalWrite(GM4in2, HIGH);
  analogWrite(GM4, speed);
}

void left(int speed){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, HIGH);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, speed);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, HIGH);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, speed);

  // Motor 3 wird angesteuert
  digitalWrite(GM3in1, HIGH);
  digitalWrite(GM3in2, LOW);
  analogWrite(GM3, speed);

  //Motor 4 wird angesteuert
  digitalWrite(GM4in1, HIGH);
  digitalWrite(GM4in2, LOW);
  analogWrite(GM4, speed);
}

void right(int speed){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, HIGH);
  analogWrite(GM1, speed);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, HIGH);
  analogWrite(GM2, speed);

  // Motor 3 wird angesteuert
  digitalWrite(GM3in1, LOW);
  digitalWrite(GM3in2, HIGH);
  analogWrite(GM3, speed);

  //Motor 4 wird angesteuert
  digitalWrite(GM4in1, LOW);
  digitalWrite(GM4in2, HIGH);
  analogWrite(GM4, speed);
}

void stop(int s){
  // Motor 1 wird angesteuert
  digitalWrite(GM1in1, LOW);
  digitalWrite(GM1in2, LOW);
  analogWrite(GM1, 255);

  //Motor 2 wird angesteuert
  digitalWrite(GM2in1, LOW);
  digitalWrite(GM2in2, LOW);
  analogWrite(GM2, 255);

  // Motor 3 wird angesteuert
  digitalWrite(GM3in1, LOW);
  digitalWrite(GM3in2, LOW);
  analogWrite(GM3, 255);

  //Motor 4 wird angesteuert
  digitalWrite(GM4in1, LOW);
  digitalWrite(GM4in2, LOW);
  analogWrite(GM4, 255);
  if (s == 1) {
    if (Leerfahrt == 0) {
      foerdern_abgabe();
      Leerfahrt = 0;
      Zielstation = 0;
      Fahrtrichtung = 0;
    }
    Zielstation = 0;
    Fahrtrichtung = 0;
  }
}

void f_forward(){
  digitalWrite(GMFin1, LOW);
  digitalWrite(GMFin2, HIGH);
  analogWrite(GMF, 120);
}

void f_stop(){
  digitalWrite(GMFin1, LOW);
  digitalWrite(GMFin2, LOW);
  analogWrite(GMF, 120);
}

void f_backward(){
  digitalWrite(GMFin1, HIGH);
  digitalWrite(GMFin2, LOW);
  analogWrite(GMF, 120);
}