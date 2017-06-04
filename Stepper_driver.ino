/* This code was written to control a 'two phase bipolar' stepper motor (type number: STH-3D1126-02),
with driver PBL3775 from an HP C5110A flatbed scanner.
* --------------------------------------------------------------------------------
Copyright (C) 2017 F. Ramakers.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    For info, mail: framakers@hotmail.com (feb 2017).
* --------------------------------------------------------------------------------

 *  WARNING:
 *  Do everything at your own risk. I do not accept any resposibility. I did not add warnings to this text, if you need them then this project is not for you.
 *  If you hate lists of warnings in projects because you know how to keep yourself safe, and if you agree you are responsible for your own actions: have fun with it!
 *
 *  DRIVER MODES
 *  The driver supports 3 modes:
 *  1) Full step mode (implemented)
 *  2) Half step mode (implemented)
 *  3) Modified half step mode (NOT implemented. I don't know how to do this (feed 140% of 5V to the driver?). Would love to hear from you, if you do know how: framakers@hotmail.com)
 *
 *  MORE INFO:
 *  See "README.TXT for more info!
 *  PHASE: direction of current through spool. HIGH = one direction, LOW = opposite direction.
 *  Disable: no current through spool. HIGH = spool disabled; LOW enabled.
 *  VRef = reference voltage.
 *
 * CREDITS/ FEEDBACK:
 * This code was written by F. Ramakers.
 * Feedback welcome, feel free to mail: framakers@hotmail.com (feb 2017).
 * Github version 1.0 (V3.0 eigen nummering).
 */
const int ledPin = 13;      // LED from scanner attached to this pin
const int buttonPin = 12;   // push button (signal wire) from scanner attached to this pin

const int disable1Pin = 3;  // connect to pin 10 (of the 3775 driver IC)
const int disable2Pin = 4;  // connect to 13
const int phase1Pin = 5;    // connect to 9
const int phase2Pin = 6;    // connect to 14
const int vRef1Pin = 7;     // connect to 7
const int vRef2Pin = 8;     // connect to 16

int tellerFullStepMode = 4; // step counter fullStep mode
int tellerHalfStepMode = 8; // step counter halfStep mode
int buttonState = 0;        // 0 = button not pressed, 1 = button pressed.

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  pinMode(disable1Pin, OUTPUT);
  pinMode(disable2Pin, OUTPUT);
  pinMode(phase1Pin, OUTPUT);
  pinMode(phase2Pin, OUTPUT);
  pinMode(vRef1Pin, OUTPUT);
  pinMode(vRef2Pin, OUTPUT);

  startPosition();          // Do digitalWrite for all 6 pins, so current output (HIGH/ LOW) is known.

  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, 1);
    delay(200);
    digitalWrite(ledPin, 0);
    delay(200);
  }

  // TEST RUN
  fullStep(200, 4);
  delay(500);
  halfStep(400, 4);
  delay(500);
  fullStepBack(200, 4);
  delay(500);
  halfStepBack(400, 4);
  delay(500);
}

void loop() {

  /*  STEPPER METHODS:
   *  fullStep(numberOfSteps, delayBetweenSteps); run in full step mode, forward
   *  halfStep(numberOfSteps, delayBetweenSteps); run in half step mode, forward
   *  fullStepBack(int numberOfSteps, int delayBetweenSteps); run in full step mode, backwards (oppsite direction of 'fullStep')
   *  halfStepBack(int numberOfSteps, int delayBetweenSteps); run in half step mode, backwards (oppsite direction of 'fullStep')
   *
   *  200 full steps = 400 half steps = 1 rotation
   *  'delayBetweenSteps' is in milliseconds
   *  bigger 'delayBetweenSteps' = slower speed.
   *
   *  EXAMPLES:
   *  fullStep(200, 10);      // parameters: numberOfSteps, delayBetweenSteps
   *  halfStepBack(400, 10);  // parameters: numberOfSteps, delayBetweenSteps
   *
  */

  // RESPOND TO BUTTON
  buttonState = digitalRead(buttonPin);
  if (buttonState == 1) {     // 1 = button is pressed
    halfStep(1, 5);
  } else {
    halfStepBack(1, 5);
  }
//  delay(5); // added for safety, for first test. Delay can be removed if all goes well.
}

void fullStep(int numberOfSteps, int delayBetweenSteps) {   // FULL STEP MODE

  digitalWrite(disable1Pin, LOW); // disabled is always low in Full Step Mode.
  digitalWrite(disable2Pin, LOW);

  digitalWrite(ledPin, !digitalRead(ledPin)); // led toggle (turns LED on and off with each step)

  for (int i = 0; i < numberOfSteps ; i++) {    // counting makes sure the stepper will continue from last step it took (important if step method is called >1 time)
    if (tellerFullStepMode < 4) {
      tellerFullStepMode++;
    } else {
      tellerFullStepMode = 1;
    }

    if (tellerFullStepMode == 1) {          // step 1
      digitalWrite(phase1Pin, HIGH);
      //digitalWrite(phase2Pin, HIGH);
    } else if (tellerFullStepMode == 2) {   // step 2
      //digitalWrite(phase1Pin, HIGH);
      digitalWrite(phase2Pin, LOW);
    } else if (tellerFullStepMode == 3) {   // step 3
      digitalWrite(phase1Pin, LOW);
      //digitalWrite(phase2Pin, LOW);
    } else {                                // step 4
      //digitalWrite(phase1Pin, LOW);
      digitalWrite(phase2Pin, HIGH);
    }
    delay(delayBetweenSteps);               // this delay determines the speed of the motor
  }
}

void fullStepBack(int numberOfSteps, int delayBetweenSteps) {   // FULL STEP MODE, OPPOSITE DIRECTION

  digitalWrite(disable1Pin, LOW); // disabled is always low in Full Step Mode.
  digitalWrite(disable2Pin, LOW);

  digitalWrite(ledPin, !digitalRead(ledPin));  // led toggle (turns LED on and off with each step)

  for (int i = 0; i < numberOfSteps ; i++) {    // counting backwards, makes the steps go in reversed order. This will make the motor spin in the opposite direction.
    if (tellerFullStepMode > 1) {
      tellerFullStepMode--;
    } else {
      tellerFullStepMode = 4;
    }

    if (tellerFullStepMode == 1) {          // step 1
      digitalWrite(phase1Pin, HIGH);
      //digitalWrite(phase2Pin, HIGH);
    } else if (tellerFullStepMode == 2) {   // step 2
      //digitalWrite(phase1Pin, HIGH);
      digitalWrite(phase2Pin, LOW);
    } else if (tellerFullStepMode == 3) {   // step 3
      digitalWrite(phase1Pin, LOW);
      //digitalWrite(phase2Pin, LOW);
    } else {                                // step 4
      //digitalWrite(phase1Pin, LOW);
      digitalWrite(phase2Pin, HIGH);
    }
    delay(delayBetweenSteps);               // this delay determines the speed of the motor
  }
}

void halfStep(int numberOfSteps, int delayBetweenSteps) {  // HALF STEP MODE

  digitalWrite(ledPin, !digitalRead(ledPin)); // led toggle (aan en uit)

  for (int i = 0; i < numberOfSteps ; i++) {
    // snelheid *= 2 // twice the speed, because twice as many steps as in full step mode

    if (tellerHalfStepMode < 8) {    // counting makes sure the stepper will continue from last step it took (important if step method is called >1 time)
      tellerHalfStepMode++;
    } else {
      tellerHalfStepMode = 1;
    }

    if (tellerHalfStepMode == 1) {         // step 1a
      digitalWrite(phase1Pin, HIGH);
      digitalWrite(disable1Pin, HIGH);
      digitalWrite(phase2Pin, HIGH);
      digitalWrite(disable2Pin, LOW);
    } else if (tellerHalfStepMode == 2) {  // step 1b
      digitalWrite(disable1Pin, LOW);
    } else if (tellerHalfStepMode == 3) {  // step 2a
      digitalWrite(phase1Pin, HIGH);
      digitalWrite(disable1Pin, LOW);
      digitalWrite(phase2Pin, LOW);
      digitalWrite(disable2Pin, HIGH);
    } else if (tellerHalfStepMode == 4) {  // step 2b
      digitalWrite(disable2Pin, LOW);
    } else if (tellerHalfStepMode == 5) {  // step 3a
      digitalWrite(phase1Pin, LOW);
      digitalWrite(disable1Pin, HIGH);
      digitalWrite(phase2Pin, LOW);
      digitalWrite(disable2Pin, LOW);
    } else if (tellerHalfStepMode == 6) {  // step 3b
      digitalWrite(disable1Pin, LOW);
    } else if (tellerHalfStepMode == 7) {  // step 4a
      digitalWrite(phase1Pin, LOW);
      digitalWrite(disable1Pin, LOW);
      digitalWrite(phase2Pin, HIGH);
      digitalWrite(disable2Pin, HIGH);
    } else {                              // step 4b
      digitalWrite(disable2Pin, LOW);
    }
    delay(delayBetweenSteps);               // this delay determines the speed of the motor
  }
}

void halfStepBack(int numberOfSteps, int delayBetweenSteps) {  // HALF STEP MODE, OPPOSITE DIRECTION

  digitalWrite(ledPin, !digitalRead(ledPin)); // led toggle (aan en uit)

  for (int i = 0; i < numberOfSteps ; i++) {
    // snelheid *= 2 // twice the speed, because twice as many steps as in full step mode

    if (tellerHalfStepMode > 1) {    // counting backwards, makes the steps go in reversed order. This will make the motor spin in the opposite direction.
      tellerHalfStepMode--;
    } else {
      tellerHalfStepMode = 8;
    }

    if (tellerHalfStepMode == 1) {         // step 1a
      digitalWrite(phase1Pin, HIGH);
      digitalWrite(disable1Pin, HIGH);
      digitalWrite(phase2Pin, HIGH);
      digitalWrite(disable2Pin, LOW);
    } else if (tellerHalfStepMode == 2) {  // step 1b
      digitalWrite(disable1Pin, LOW);
    } else if (tellerHalfStepMode == 3) {  // step 2a
      digitalWrite(phase1Pin, HIGH);
      digitalWrite(disable1Pin, LOW);
      digitalWrite(phase2Pin, LOW);
      digitalWrite(disable2Pin, HIGH);
    } else if (tellerHalfStepMode == 4) {  // step 2b
      digitalWrite(disable2Pin, LOW);
    } else if (tellerHalfStepMode == 5) {  // step 3a
      digitalWrite(phase1Pin, LOW);
      digitalWrite(disable1Pin, HIGH);
      digitalWrite(phase2Pin, LOW);
      digitalWrite(disable2Pin, LOW);
    } else if (tellerHalfStepMode == 6) {  // step 3b
      digitalWrite(disable1Pin, LOW);
    } else if (tellerHalfStepMode == 7) {  // step 4a
      digitalWrite(phase1Pin, LOW);
      digitalWrite(disable1Pin, LOW);
      digitalWrite(phase2Pin, HIGH);
      digitalWrite(disable2Pin, HIGH);
    } else {                              // step 4b
      digitalWrite(disable2Pin, LOW);
    }
    delay(delayBetweenSteps);               // this delay determines the speed of the motor
  }
}

void disableStepper() {
  digitalWrite(disable1Pin, HIGH); // disable high = motor (spool) turned OFF.
  digitalWrite(disable2Pin, HIGH);
}

void startPosition() {
  digitalWrite(disable1Pin, HIGH); // disable high = motor (spool) turned OFF.
  digitalWrite(disable2Pin, HIGH);

  digitalWrite(phase1Pin, HIGH);// phase high: because this is the state needed for the first step the stepper needs to take
  digitalWrite(phase2Pin, HIGH);

  // vRef is always set to HIGH (100%) in "Full Step Mode" and "Half Step Mode".
  // Note: in "modified half step mode, it is set to 140%, I don't know if this is possible with Arduino.
  digitalWrite(vRef1Pin, HIGH); // vRef pin is always HIGH.
  digitalWrite(vRef2Pin, HIGH);
}
