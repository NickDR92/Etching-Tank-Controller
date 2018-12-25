/************************************************************************
*                      Includes
************************************************************************/
#include <Wire.h>
#include <LCD.h> 
#include <LiquidCrystal_I2C.h>

/************************************************************************
*                      Variables
************************************************************************/
 // Input
#define sensorPin       A0 //the analog pin the TMP36's Vout (sense) pin is connected to the resolution is 10 mV / degree centigrade with a 500 mV offset to allow for negative temperatures
#define ButtonSlot1Pin  4
#define ButtonSlot2Pin  5
#define ButtonSlot3Pin  6
#define ButtonSlot4Pin  7

// Output
#define ValveSlot1Pin   13
#define ValveSlot2Pin   12
#define ValveSlot3Pin   11
#define ValveSlot4Pin   10

// Flags
volatile int StateSlot1 = LOW;
volatile int StateSlot2 = LOW;
volatile int StateSlot3 = LOW;
volatile int StateSlot4 = LOW;
volatile int StateValve1 = LOW;
volatile int StateValve2 = LOW;
volatile int StateValve3 = LOW;
volatile int StateValve4 = LOW;
volatile int isChanged = LOW;

// LCD
LiquidCrystal_I2C lcd(0x3f,2,1,0,4,5,6,7);  // Set the LCD I2C address 0x3F
#define BackligthPin    3 // Pin LCD

// Temperature
volatile float Temperature = 25.4;

/************************************************************************
*                      Function
************************************************************************/
// This function initializes all the io pins
void HardwareInit(){
  pinMode(ButtonSlot1Pin, INPUT);
  pinMode(ButtonSlot2Pin, INPUT);
  pinMode(ButtonSlot3Pin, INPUT);
  pinMode(ButtonSlot4Pin, INPUT);

  pinMode(ValveSlot1Pin, OUTPUT);
  pinMode(ValveSlot2Pin, OUTPUT);
  pinMode(ValveSlot3Pin, OUTPUT);
  pinMode(ValveSlot4Pin, OUTPUT);
}

// This function reads the temperature from de PT100
void TemperatureReading(){
  //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 3.3;
 voltage /= 1024.0; 
 
 // print out the voltage
 Serial.print(voltage); 
 Serial.println(" volts");
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset to degrees ((voltage - 500mV) times 100)
 Serial.print(temperatureC); 
 Serial.println(" degrees C");
 delay(1000);                                     //waiting a second 
}

// This function reads all the input states and sets the corresponding outputs
void ReadSetIO(){
  // Read inputs
  StateSlot1 = digitalRead(ButtonSlot1Pin);
  StateSlot2 = digitalRead(ButtonSlot2Pin);
  StateSlot3 = digitalRead(ButtonSlot3Pin);
  StateSlot4 = digitalRead(ButtonSlot4Pin);

  // Set output
  if(StateSlot1 == HIGH){
    isChanged = HIGH;
    if(StateValve1 == LOW){
      digitalWrite(ValveSlot1Pin, HIGH);
     StateValve1 = HIGH;
    } else {
      digitalWrite(ValveSlot1Pin, LOW);
      StateValve1 = LOW;
    }
  }
  if(StateSlot2 == HIGH){
    isChanged = HIGH;
    if(StateValve2 == LOW){
      digitalWrite(ValveSlot2Pin, HIGH);
      StateValve2 = HIGH;
    } else {
      digitalWrite(ValveSlot2Pin, LOW);
      StateValve2 = LOW;
    }
  }
  if(StateSlot3 == HIGH){
    isChanged = HIGH;
    if(StateValve3 == LOW){
      digitalWrite(ValveSlot3Pin, HIGH);
      StateValve3 = HIGH;
    } else {
      digitalWrite(ValveSlot3Pin, LOW);
      StateValve3 = LOW;
    }
  }
  if(StateSlot4 == HIGH){
    isChanged = HIGH;
    if(StateValve4 == LOW){
      digitalWrite(ValveSlot4Pin, HIGH);
      StateValve4 = HIGH;
    } else {
      digitalWrite(ValveSlot4Pin, LOW);
      StateValve4 = LOW;
    }
  }
}

// This function writes the LCD
void WriteLCD(){
  isChanged = LOW;
  lcd.clear();
  lcd.home();

  if(StateValve1 == HIGH){
    lcd.print("1:ON");
  } else{
    lcd.print("1:OFF");
  }
  lcd.setCursor(0,1);
  if(StateValve2 == HIGH){
    lcd.print("2:ON");
  } else{
    lcd.print("2:OFF");
  }
  lcd.setCursor(6,0);
  if(StateValve3 == HIGH){
    lcd.print("3:ON");
  } else{
    lcd.print("3:OFF");
  }
  lcd.setCursor(6,1);
  if(StateValve4 == HIGH){
    lcd.print("4:ON");
  } else{
    lcd.print("4:OFF");
  }

  lcd.setCursor(11,0);
  lcd.print(Temperature);
}
/************************************************************************
*                      Initialization
************************************************************************/
 void setup(){
  //Serial.begin(9600);  //Start the serial connection with the computer to view the result open the serial monitor 

  // Initialize IO
  HardwareInit();

  // Initialize LCD
  lcd.begin(16,2); 
  lcd.setBacklightPin(BackligthPin,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();
  lcd.print("Etching Tank");
  delay(1000);
  WriteLCD();
}

/************************************************************************
*                      Main
************************************************************************/
void loop(){
  ReadSetIO();
  if(isChanged == HIGH){
    WriteLCD();
  }
  delay(150);
}
