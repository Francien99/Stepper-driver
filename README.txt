/* This code was written to control a 'two phase bipolar' stepper motor (type number: STH-3D1126-02),
with driver PBL3775 from an HP C5110A flatbed scanner. See below license for a more simple way to say this.

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

If the first line of this document makes no sense at all: 
I had an old flatbed scanner and wanted to see if I could move the motor inside with my Arduino. 
This is the code I have used. I have also tried to describe how I took it apart 
(although this is probably hard to understand without pictures. If I do have time, I might add them). 
This scanner contains a 'two phase bipolar' stepper motor (type number: STH-3D1126-02), it is a HP C5110A flatbed scanner.
The PCB in the scanner contains driver PBL3775. 
The Arduino sends signals to this driver, which will in turn make the stepper motor move. 
The rest of the scanner no longer works, so this code is usefull if you would like to take the stepper motor + driver out of the scanner in order to use it in another project.

 I have used an Arduino UNO, I expect it to work with other types of arduino, but this has not been tested.
 
 PURPOSE OF THIS CODE:
 1) move the stepper as desired with an arduino
 2) to learn about driving stepper motors (with arduino)
 3) for fun the fun of it.
 4) If you do find an actual use, or did something cool with it, please let me know! Framakers@hotmail.com
 
  TAKING STUFF APART:
  To locate the stepper: remove lid. Screw top part (with the glass plate) of the scanner. It's the big (black, square) stepper motor that moves the big black scan head up and down.
  To locate the driver: remove the metal casing on top of both PCB's. Look for the big black IC with the number "PBL3775" on it, it's located near the plug towards the stepper.
  Warning: High voltage (230 volt!) on the (orange) PCB. This is the PCB with the power cable attached. It also has the driver on it and has got the stepper motor attached to it.
  (needles to say, you unplugged the power cord before even opening the scanner... if not, please forget about doing this project al together.)
  Unplug the push button (on front of the scanner) and LED from the green PCB, they can be attached to the arduino.
  Remove the green PCB (with all the small IC's, it's the one NOT connected to the power cord).
  Remove the plug between both PCB's. The socket is where the wires from your arduino can be connected.
  I refer to this socket as "the plug" (with 14 pins) !!!
  Once the jumper wires are attached to the plug (see below), the metal casing can be reinstalled on top of the PCB (for your own safety).
  You might want to add a bit of tape to the side of the casing, to protect your jumper wires from the sharp edge.
  You might also want to remove the black, rubber belt so the stepper can be tested without the black scanner head running against the top/ bottom of the scanner.

  WARNING:
  Do everything at your own risk. I do not accept any resposibility. I did not add warnings to this text, if you need them then this project is not for you.
  If you hate lists of warnings in projects because you know how to keep yourself safe, and if you agree you are responsible for your own actions: have fun with it!

  DRIVER MODES
  The driver supports 3 modes:
  1) Full step mode (implemented)
  2) Half step mode (implemented)
  3) Modified half step mode (NOT implemented. I don't know how to do this (feed 140% of 5V to the driver?). Would love to hear from you, if you do know how: framakers@hotmail.com)

  MORE INFO:
  See the "Ericsson PBL3775-1 (or PBL3775/1) datasheet" for the driver. I found it here: http://www.datasheetcatalog.com/datasheets_pdf/P/B/L/3/PBL3775-1.shtml
  PHASE: direction of current through spool. HIGH = one direction, LOW = opposite direction.
  Disable: no current through spool. HIGH = spool disabled; LOW enabled.
  VRef = reference voltage.
  This explains Vref, PHASE and Disable in more detail + lots of usefull info.

  WIRES:
  "The plug" (with 14 pins) has got two rows of 7 pins each. Male headers, the arduino can be connected here.
  Here is (more or less... ;-) lot's of imagination required!) a picture of it:

                         ------
   VRef 2        = 16 = | x  x | = 13 = Disable 2        ---> connect those two pins to Arduino pins as defined in the code
   PHASE 2       = 14 = | x  x | = 7  = VRef 1           ---> connect those two pins to Arduino pins as defined in the code
   Disable 1     = 10 = | x  x | = 9  = PHASE 1          ---> connect those two pins to Arduino pins as defined in the code
   GND           =    = | x  x | =    = GND              ---> those two pins do not need to be connected
   24 V (output) =    = | x  x | =    = 12 V (output)    ---> those two pins should NOT be connected!
   GND           =    = | x  x | =    = GND              ---> connect one of the GND pins to Arduino GND
   5 V (output)  =    = | x  x | =    = 5V (output)      ---> one of these can be used to power Arduino, connect to Arduino Vin.
                        --------
 CREDITS/ FEEDBACK:
 This code was written by and is property of Francien Ramakers.
 Feedback welcome, please mail: framakers@hotmail.com (feb 2017).
 Github version 1.0 (V3.0 eigen nummering).
 
