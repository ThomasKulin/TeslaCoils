#include <Bounce2.h>
#include <Encoder.h>
#include <EEPROM.h>
#include <LiquidMenu.h>
#include "LCD_Menu_Handler.h"
#include "Encoder_Handler.h"

#define TIMER_OFF_PERIOD 5  //period in uS before Timer1 triggers the interrupt (200KHz or 480 instructions at 96MHz)
#define MAX_ON_TIME 1000.0
#define NOTE_MULTIPLIER 1024

//on time for each note
unsigned int note_on_time = 1;

//define pins
const byte tone1pin = 3;
const byte tone2pin = 4;
const byte tone3pin = 5;
const byte LEDPin = 13;
const byte triggerPin = 12;
const byte togglePin = 11;
const byte enterPin = 8;
const byte debugPin = A1;

Bounce triggerbutton = Bounce(triggerPin, 10);  // 10 ms debounce
Bounce togglebutton = Bounce(togglePin, 10);  // 10 ms debounce
Bounce enterbutton = Bounce(enterPin, 10);
Bounce debugbutton = Bounce(debugPin, 10);

//note toggling
boolean note1on = false;
boolean note2on = false;
boolean note3on = false;

//button events
boolean triggerPressed = false;
boolean modePressed = false;
boolean enterPressed = false;
boolean debugPressed = false;
boolean triggerHold = false;

//on time values for accumulator
unsigned volatile long note1_on_time;
unsigned volatile long note2_on_time;
unsigned volatile long note3_on_time;

//32-bit accumulator for note
unsigned volatile long bres1 = 0;
unsigned volatile long bres2 = 0;
unsigned volatile long bres3 = 0;

//32-bit frequency of note
unsigned long note1freq;
unsigned long note2freq;
unsigned long note3freq;
unsigned int freq;

int delta = 1;
unsigned int pressTime = 0;
boolean prevPressed = false;

IntervalTimer bTimer; //initialize interval timer you silly cunt

//calculate note
char* pitch_name(byte pitch) {
  static char* names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  return names[pitch % 12];
}

void setup() {
  read_settings(); //pull saved settings from EEPROM
  pinMode(tone1pin, OUTPUT);
  pinMode(tone2pin, OUTPUT);
  pinMode(tone3pin, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(togglePin, INPUT_PULLUP);
  pinMode(enterPin, INPUT_PULLUP);
  pinMode(debugPin, INPUT);

  lcd.begin(16, 2);
  screen1_line1.attach_function(1, on_time_up);
  screen1_line1.attach_function(2, on_time_down);
  screen1_line2.attach_function(1, freq_up);
  screen1_line2.attach_function(2, freq_down);
  screen1_line2.attach_function(3, change_delta);
  screen1_line3.attach_function(1, note_up);
  screen1_line3.attach_function(2, note_down);
  screen2_line2.attach_function(1, control_up);
  screen2_line2.attach_function(2, control_down);
  screen3_line2.attach_function(1, save_settings);
  LCD_menu.add_screen(burst_screen);
  LCD_menu.add_screen(musical_screen);
  LCD_menu.add_screen(midi_screen);
  LCD_menu.add_screen(mode_screen);
  LCD_menu.add_screen(save_screen);
  LCD_menu.next_screen();
  int32_t temp = 40000;
  controlKnob.write(temp);

  bTimer.begin(noteISR, TIMER_OFF_PERIOD);    //trigger noteISR() function every TIMER_OFF_PERIOD microseconds
  bTimer.priority(1);                     //set the priority of the Timing interrupt to high priority (normally 128)
  interrupts();                           //start interrupts

  usbMIDI.setHandleNoteOn(noteOn);
  usbMIDI.setHandleNoteOff(noteOff);

  Serial.begin(115200);
}
void loop() {
  update_screen(); // update screen contents
  readInputs(); // check state of inputs
  //print_settings();

  //--------------- Set On-Time -----------------------------

  if (note_on_time > MAX_ON_TIME)
    note_on_time = MAX_ON_TIME;
  LCD_on_time = note_on_time; // var to print to LCD
  //  Serial.print("On-Time: ");
  //  Serial.print(note_on_time * TIMER_OFF_PERIOD);
  //  Serial.print("uS,  frequency: ");

  //---------------------------------------------------------

  //------------------- Set Frequency -----------------------

  switch (ctrl_i) {
    case 0: //burst mode
      note1freq = (long) freq * NOTE_MULTIPLIER;
      if (triggerPressed || triggerHold) {
        note1on = true;
        //Serial.print(freq);
      }
      else
        note1on = false;
      break;
    case 1: //musical mode
      note1freq = (long) pitchFrequency[notes_i] * NOTE_MULTIPLIER;
      if (triggerPressed || triggerHold) {
        note1on = true;
         //Serial.print(pitchFrequency[notes_i]);
      }
      else
        note1on = false;
      break;
    case 2: //midi mode
    case 3: //midi mode
      //handled by MIDI_Handler
      usbMIDI.read();
      break;
    case 4: //sd card
      //TODO
      break;
  }

  if (note1freq < 2 * NOTE_MULTIPLIER)
    note1freq = (long) 2 * NOTE_MULTIPLIER;
  else if (note1freq > 20000 * NOTE_MULTIPLIER)
    note1freq = (long) 20000 * NOTE_MULTIPLIER;
  LCD_freq = freq;//note1freq / NOTE_MULTIPLIER; // var to print to LCD
  //Serial.println("Hz");

  //---------------------------------------------------------

  /*if (triggerPressed) {
    note1on = true;
    digitalWrite(LEDPin, HIGH);
    Serial.print("Output HIGH, ON time: ");
    Serial.print(note_on_time);
    Serial.print(", Frequency: ");
    Serial.print(note1freq / NOTE_MULTIPLIER);
    }
    else {
    note1on = false; //stop making noise you fuck
    digitalWrite(LEDPin, LOW);
    Serial.print("Burst Mode OFF");
    }
    Serial.print("\n");
  */
}

void readInputs() {

  focused = LCD_menu.is_focused();

  //--------------------------------
  //button state handling
  //--------------------------------

  if (triggerbutton.update()) {
    if (triggerbutton.fallingEdge()) {
      triggerPressed = true;
    }
    else {
      triggerPressed = false;
    }
  }
  if (togglebutton.update()) {
    if (togglebutton.fallingEdge()) {
      modePressed = true;
    }
    else {
      modePressed = false;
    }
  }
  if (enterbutton.update()) {
    if (enterbutton.fallingEdge()) {
      enterPressed = true;
      prevPressed = true;
      pressTime = millis();
    }
    else {
      enterPressed = false;
    }
  }
  if(enterPressed){
  }
  if (debugbutton.update()) {
    if (debugbutton.fallingEdge()) {
      debugPressed = true;
    }
    else {
      debugPressed = false;
    }
  }

  //--------------------------------
  //button change state handling
  //--------------------------------

  if (triggerbutton.fallingEdge()) {
  }
  if (togglebutton.fallingEdge()) {
    if (triggerHold) {
      triggerHold = false;
    }
    else {
      triggerHold = true;
    }
  }

  if(enterPressed == false && prevPressed == true){ 
    prevPressed = false;
    if((millis() - pressTime) < 1000){ //short press
      LCD_menu.switch_focus();
    }
    else {//long press
      LCD_menu.call_function(3);
    }
  }

  if (debugbutton.fallingEdge()) {
    lcd.noDisplay();
    delay(500);
    lcd.display();
  }

  //--------------------------------
  //rotary encoder handling
  //--------------------------------
  switch (updateEncoder()) {

    case 1: { //CW rotation
        if (focused == 1) { //returns 1 for true, 47 for false (no fucking clue why)
          LCD_menu.call_function(1);
        }
        else {
          LCD_menu.next_screen();
        }
        break;
      }
    case 2: { //CCW rotation
        if (focused == 1) { //returns 1 for true, 47 for false (no fucking clue why)
          LCD_menu.call_function(2);
        }
        else {
          LCD_menu.previous_screen();
        }
        break;
      }
    default: //do nothing
      break;
  }
}

void EEPROMWriteInt(int address, int value){
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);

  EEPROM.update(address, two);
  EEPROM.update(address + 1, one);
}

int EEPROMReadInt(int address){
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);

  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}

