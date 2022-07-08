#include <Pixy2.h>
#include <SPI.h>

// Nambahin komen gak jelas

// This is the main Pixy object
Pixy2 pixy;

int selenoid = 53;

int trigPin = 10;
int echoPin = 11;
long duration;
int distance;

//pwmA, kananA, kananB, kiriA, kiriB, pwmB, belakangA, belakangB
int myPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
int baseSpeed = 130;

int cont = 0;
int signature, x, y, width, height;
float cx, cy, area;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pixy.init();
  pinMode(selenoid, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  for (int i = 0; i < 6; i++) {
    pinMode(myPins[i], OUTPUT);
  }
  analogWrite(myPins[0], 255);
  analogWrite(myPins[5], 255);
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  int jarak = distance;

  float turnn = pixyCheck();
  Serial.print("jarak: "); Serial.print(jarak);
  digitalWrite(selenoid, HIGH);
  
  if (turnn >= 0 && turnn < 0.50) {
    turnn = 0;
    if (jarak <= 2) {
      mundur();
    }
    else if (jarak > 2 && jarak < 6) {
      berhenti();
      delay(500);
      //tendang
      digitalWrite(selenoid, LOW);
      delay(1000);
    }
    else {
      maju();
    }
  }
  else if (turnn < 0) {
    puterKanan();
  }
  else if (turnn > 0) {
    puterKiri();
  }
  else {
    maju();
  }
  delay(1);
  Serial.print(" turn: "); Serial.print(turnn);
}

float pixyCheck() {
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];
  //grab blocks!
  blocks = pixy.ccc.getBlocks();

  //if there are detected blocks, print them!
  if (blocks) {
    signature = pixy.ccc.blocks[0].m_signature;
    height = pixy.ccc.blocks[0].m_height;
    width = pixy.ccc.blocks[0].m_width;
    x = pixy.ccc.blocks[0].m_x;
    y = pixy.ccc.blocks[0].m_y;
    cx = (x + (width / 2));
    cy = (y + (height / 2));
    cx = mapfloat(cx, 0, 320, -1, 1);
    cy = mapfloat(cy, 0, 200, 1, -1);
    area = width * height;

    Serial.print(" sig: ");     Serial.print(signature);
    Serial.print(" x: ");      Serial.print(x);
    Serial.print(" y: ");      Serial.print(y);
    Serial.print(" Width: ");  Serial.print(width);
    Serial.print(" Height: "); Serial.print(height);
    Serial.print(" cx: ");     Serial.print(cx);
    Serial.print(" cy: ");     Serial.print(cy);
    Serial.print(" area: ");   Serial.println(area);
  }
  else {
    cont += 1;
    if (cont == 100) {
      cont = 0;
      cx = 0;
    }
  }
  return cx;
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void maju() {
  digitalWrite(myPins[1], HIGH);
  digitalWrite(myPins[2], LOW);
  digitalWrite(myPins[3], LOW);
  digitalWrite(myPins[4], HIGH);
  digitalWrite(myPins[6], LOW);
  digitalWrite(myPins[7], LOW);
}
void mundur() {
  digitalWrite(myPins[1], LOW);
  digitalWrite(myPins[2], HIGH);
  digitalWrite(myPins[3], HIGH);
  digitalWrite(myPins[4], LOW);
  digitalWrite(myPins[6], LOW);
  digitalWrite(myPins[7], LOW);
}
void puterKanan() {
  digitalWrite(myPins[1], HIGH);
  digitalWrite(myPins[2], LOW);
  digitalWrite(myPins[3], HIGH);
  digitalWrite(myPins[4], LOW);
  digitalWrite(myPins[6], HIGH);
  digitalWrite(myPins[7], LOW);
}
void puterKiri() {
  digitalWrite(myPins[1], LOW);
  digitalWrite(myPins[2], HIGH);
  digitalWrite(myPins[3], LOW);
  digitalWrite(myPins[4], HIGH);
  digitalWrite(myPins[6], LOW);
  digitalWrite(myPins[7], HIGH);
}
void berhenti() {
  digitalWrite(myPins[1], LOW);
  digitalWrite(myPins[2], LOW);
  digitalWrite(myPins[3], LOW);
  digitalWrite(myPins[4], LOW);
  digitalWrite(myPins[6], LOW);
  digitalWrite(myPins[7], LOW);
}
