#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*—–( Declare Constants and Pin Numbers )—–*/
#define CE_PIN 9
#define CSN_PIN 10

// NOTE: the “LL” at the end of the constant is “LongLong” type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

/*—–( Declare objects )—–*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio


int pot[4]; // 2 element array holding Joystick readings
const int motor1Pin = 4;    // RIGHT MOTOR
const int motor2Pin = 5;    //
const int enablePin = 3;    //


const int motor1BPin = 7;    // LEFT MOTOR
const int motor2BPin = 8;    //
const int enableBPin = 6;    //

int motorSpeed = 0;
int motorSpeedB = 0;

int potMax = 230;//
int potMaxB = 255; //
int potMin = 10;
int potMinB = 10;
  
void setup() {

  Serial.begin(9600);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  // set all the other pins you're using as outputs:
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // set all the other pins you're using as outputs:
  pinMode(motor1BPin, OUTPUT);
  pinMode(motor2BPin, OUTPUT);
  pinMode(enableBPin, OUTPUT);


  // set enablePin high so that motor can turn on:
  digitalWrite(enablePin, HIGH);
  digitalWrite(enableBPin, HIGH);

  // digitalWrite(stby, HIGH);
}

void loop() {

  if ( radio.available() )
  {
    // Read the data payload until we’ve received everything
    bool done = false;
    radio.read(pot, sizeof(pot));

    if (pot[0] > 525)
    {
      digitalWrite(motor1Pin, HIGH); // RIGHT
      digitalWrite(motor2Pin, LOW);  //

      motorSpeed = map(pot[0], 525, 1023, potMin, potMax);
      analogWrite(enablePin,  motorSpeed);
    }
    if (pot[0] <= 525)
    {
      digitalWrite(motor1Pin, LOW);   // RIGHT
      digitalWrite(motor2Pin, HIGH);  //

      motorSpeed = map(pot[0], 525, 0, potMin, potMax);
      analogWrite(enablePin, motorSpeed);
    }

    if (pot[1] > 497)
    {
      digitalWrite(motor1BPin, HIGH); // LEFT
      digitalWrite(motor2BPin, LOW);  //

      motorSpeedB = map(pot[1], 497, 1023, potMinB, potMaxB);
      analogWrite(enableBPin,  motorSpeedB);
    }
    if (pot[1] <= 497)
    {
      digitalWrite(motor1BPin, LOW);   // LEFT
      digitalWrite(motor2BPin, HIGH);  //

      motorSpeedB = map(pot[1], 497, 0, potMinB, potMaxB);
      analogWrite(enableBPin, motorSpeedB);
    }

    if ((motorSpeed < 20) && (motorSpeedB < 20))
    {
      motorSpeed = 0;
      motorSpeedB = 0;

    }

    Serial.print("RIGHT = ");
    Serial.print(motorSpeed);

    Serial.print(" LEFT = ");
    Serial.println(motorSpeedB);

    delay(20);

  }
}
