#include "OneButton.h"
OneButton encoderButton(4, true);
OneButton Star(6, true);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
const int stepsPerRevolution = 2048;  
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
LiquidCrystal_I2C lcd(0x27,16,2); 
#define Start 6 
int hours = 0;
int minutes = 0;
int seconds = 0;
boolean timeState = false;
#define encoderPinA 2 
#define encoderPinB 3 
int HMS = 1;
int encoderPos = 0; 
unsigned int lastReportedPos = 1; 
static boolean rotating = false;
boolean A_set = false;
boolean B_set = false;

void setup() {
 
pinMode(Start,INPUT);
digitalWrite(Start,HIGH);
pinMode(Start, INPUT_PULLUP);
attachInterrupt(0, doEncoderA, CHANGE); 
attachInterrupt(1, doEncoderB, CHANGE); 
pinMode(encoderPinA, INPUT_PULLUP);
pinMode(encoderPinB, INPUT_PULLUP);
Serial.begin(9600); 
lcd.init();
lcd.clear();
lcd.noBacklight(); 
lcd.setCursor(0, 0);
lcd.print("SET TIME:");
lcd.setCursor(4, 1);
lcd.print("00:00:00"); 
encoderButton.attachClick(click1);
encoderButton.attachLongPressStart(longPressStart1);
encoderButton.attachLongPressStop(longPressStop1);
encoderButton.attachDuringLongPress(longPress1);
 
} 

void click1 ()
{
HMS = HMS + 1;
if (HMS == 4)
{
HMS = 1;
}
delay(500);
}

void loop() {
encoderButton.tick();
Star.tick();
delay(10);
rotating = true; 
encoderPos = constrain(encoderPos, -1, 1);
if (lastReportedPos != encoderPos) {
if (HMS == 1){ 
  hours = hours + encoderPos;
hours = constrain(hours, 0, 24);
}
else if (HMS == 2) {
minutes = minutes + encoderPos;
minutes = constrain(minutes, 0, 60);
}
else if (HMS == 3) {
seconds = seconds + encoderPos;
seconds = constrain(seconds, 0, 60);
}

lcd.setCursor(4, 1);
if (hours <= 9)
{
lcd.print("0");
}
lcd.print(hours);
lcd.print(":");
if (minutes <= 9)
{
lcd.print("0");
}
lcd.print(minutes);
lcd.print(":");
if (seconds <= 9)
{
lcd.print("0");
}
lcd.print(seconds);
encoderPos = 0;
lastReportedPos = encoderPos;

}
if (digitalRead(Start) == LOW) { 
lcd.setCursor(0, 0);
lcd.print("COUNTDOWN TIMER:");

timeState = true;
while (timeState == true) {
if (minutes == 0 && hours >= 1) {
minutes = 60;
hours = hours - 1;
}
if (seconds == 0 && minutes >= 1) {
seconds = 60;
minutes = minutes - 1;
}
else if (minutes == 0 && hours == 0 && seconds == 0) { 
while (timeState == true) {
myStepper.setSpeed(10);
myStepper.step(stepsPerRevolution); {
timeState = false;
seconds = 1;
lcd.clear(); 
lcd.noBacklight(); 
lcd.setCursor(0, 0);
lcd.print("SET TIME:");
lcd.setCursor(4, 1);
lcd.print("00:00:00");

break;
}
}
}  
delay(992); 
seconds = seconds - 1;

lcd.setCursor(4, 1);
if (hours <= 9)
{
lcd.print("0");
}
lcd.print(hours);
lcd.print(":");
if (minutes <= 9)
{
lcd.print("0");
}
lcd.print(minutes);
lcd.print(":");
if (seconds <= 9)
{
lcd.print("0");
}
lcd.print(seconds);
}
}
}

void doEncoderA() {

if ( rotating ) delay (1); 

if ( digitalRead(encoderPinA) != A_set ) { 
A_set = !A_set;

if ( A_set && !B_set )
encoderPos = 1;

rotating = false; 
}
}

void doEncoderB() {
if ( rotating ) delay (1);
if ( digitalRead(encoderPinB) != B_set ) {
B_set = !B_set;

if ( B_set && !A_set )
encoderPos = -1;

rotating = false;
}
}

void longPressStart1() {
   lcd.backlight(); 
}

void longPress1() { 
} 

void longPressStop1() {
}
