
/*-----RECEIVE-----*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int pot[4];  // 2 element array holding Joystick readings

#include <Servo.h>      // include the servo library

Servo servo1;      
Servo servo2;      

unsigned long prevMillis = 0;
long interval = 500;

int switchPin = 7;

int pos;
int pos2;

void setup()
{
  Serial.begin(9600);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  ;

  servo1.attach(5);  
  servo2.attach(6);  

  pinMode(switchPin, INPUT);
}

void loop()
{
  if (digitalRead(switchPin) == HIGH)
  {
    // Read the data payload until we've received everything
    bool done = false;
    if (!done)
    {
      int analogValue = pot[0]; // read the analog input
      int analogValue2 = pot[1]; // read the analog input

      int servoAngle = map(analogValue, 0, 1023, 70, 110);
      int servoAngle2 = map(analogValue2, 0, 1023, 70, 110);

      // Fetch the data payload
      done = radio.read( pot, sizeof(pot) );
      Serial.print("X = ");
      Serial.print(pot[0]);

      Serial.print(" Y = ");
      Serial.println(pot[1]);

      servo1.write(servoAngle);
      delay (20);     
      servo2.write(servoAngle2);
      delay (20);
    }
  }
  else
  {
    unsigned long newMillis = millis();
    if (newMillis - prevMillis > interval)
    {
      prevMillis = newMillis;

      int pos = random(80, 100);
      int pos2 = random(50, 130);

      servo1.write(pos);
      servo2.write(pos2);
    }
  }
}


