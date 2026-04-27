//Include the libraries
#include <SoftwareSerial.h>
#include "configuration.h"

SoftwareSerial BLE(10, 11);
// Timing variables to stop if no command after 500ms
unsigned long lastCommandTime = 0;
const unsigned long commandTimeout = 500; 
//Moving functions
void go_Advance() {
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, HIGH);
  digitalWrite(dir2PinR, LOW);
}

void go_Left() {
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, HIGH);
}

void go_Right() {
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL, HIGH);
  digitalWrite(dir1PinR, HIGH);
  digitalWrite(dir2PinR, LOW);
}

void go_Back() {
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL, HIGH);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, HIGH);
}

void stop_Stop() {
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, LOW);
  analogWrite(speedPinL, 0);
  analogWrite(speedPinR, 0);
}
//Function to set the motor speed
void set_Motorspeed(int speed_L, int speed_R) {
  analogWrite(speedPinL, speed_L);
  analogWrite(speedPinR, speed_R);
}

void do_Uart_Tick() {
// Checking for new data and resetting the failsafe
  if (BLE.available()) {
    char command = BLE.read();
    lastCommandTime = millis(); // reset timer on new command
//Using the number received to execute a command
    switch (command) {
      case '2': go_Advance(); set_Motorspeed(255, 255); break;
      case '4': go_Left(); set_Motorspeed(255, 255); break;
      case '6': go_Right(); set_Motorspeed(255, 255); break;
      case '8': go_Back(); set_Motorspeed(255, 255); break;
      case '5': stop_Stop(); break;
      default: break;
    }
  }

  // failsafe: stop motors if no command received
  if (millis() - lastCommandTime > commandTimeout) {
    stop_Stop();
  }
}

void setup() {
  Serial.begin(9600);
  BLE.begin(9600);

  pinMode(dir1PinL, OUTPUT);
  pinMode(dir2PinL, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  pinMode(dir1PinR, OUTPUT);
  pinMode(dir2PinR, OUTPUT);
  pinMode(speedPinR, OUTPUT);

  // ensure motors are stopped immediately on startup
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, LOW);
  analogWrite(speedPinL, 0);
  analogWrite(speedPinR, 0);
	// make sure car is stopped
  stop_Stop();               
  Drive_Status = MANUAL_DRIVE;
  Drive_Num = STOP_STOP;
	// initialize command timer
  lastCommandTime = millis(); 
}

void loop() {
	// Continuously check for and handle Bluetooth commands
  do_Uart_Tick();
}
