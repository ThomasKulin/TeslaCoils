#include <TimerOne.h>
#include <MIDI.h>
#include "pitchFreq.h"

#if defined(USBCON)
#include <midi_UsbTransport.h>

static const unsigned sUsbTransportBufferSize = 16;
typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;

UsbTransport sUsbTransport;

MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, MIDI);

#else // No USB available, fallback to Serial
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

 
#define TIMER_OVF_PERIOD 32  //period in uS before Timer1 triggers the interrupt
#define MAX_ON_TIME 10.0 

//on time for each note
unsigned long note_on_time = MAX_ON_TIME;

//define pins for tone output
int tone1pin = 3;            
int tone2pin = 5;
int tone3pin = 6;
int LEDPin = 13; 
int togglePin = 12;
int modePin = 11;
bool lastState = false;

//note toggling
boolean note1on = false;     
boolean note2on = false;
boolean note3on = false;

//on time values for accumulator
unsigned long note1_on_time;
unsigned long note2_on_time;
unsigned long note3_on_time;

//32-bit accumulator for note
unsigned long bres1 = 0;     
unsigned long bres2 = 0;
unsigned long bres3 = 0;

//32-bit frequency of note
unsigned long note1freq;     
unsigned long note2freq;
unsigned long note3freq;
unsigned long freq;

//calculate note
const char* pitch_name(byte pitch) {
  static const char* names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  return names[pitch % 12];
}

void setup() {
  
  pinMode(tone1pin, OUTPUT);
  pinMode(tone2pin, OUTPUT);
  pinMode(tone3pin, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(togglePin, INPUT);
  pinMode(modePin, INPUT);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  Timer1.initialize(TIMER_OVF_PERIOD);    //trigger the interrupt every 32 microseconds
  Timer1.attachInterrupt(noteISR);        //attach interrupt service routine to Timer1
  interrupts();                           //start interrupts

  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);

  MIDI.setHandleNoteOn(noteOn);
  MIDI.setHandleNoteOff(noteOff);
}
 
void loop() {
//  note1freq = (long) analogRead(A0)*640;
//  note2freq = (long) analogRead(A1)*640;
//  note3freq = (long) analogRead(A2)*640;
 
//  note1freq = (long) 2000 * 64;             //assign A4 note to note1
//  note2freq = (long) 440 * 64;
//  note3freq = (long) 329 * 64;
  
  //if(digitalRead(modePin)){ //bps moode or MIDI mode
    MIDI.read(); //MIDI mode
    note_on_time = (long) analogRead(A0)/10;
    note_on_time = (long) analogRead(A0);
    note_on_time = (long) (note_on_time*0.00782013685*6); //map output between 1.25ms and 2.75ms per pulse (800hz to 364 hz)
    
      if(lastState == false) { //if we are switching from BPS mode, we need to stop the note first
        note1on = false;
        lastState = true;
        Serial.println("WE TURNED IT OFF");
      }
  /*}
  else{ //bps mode
    Serial.print("BPS MODE: ");
    lastState = false;

    //ON TIME
    note_on_time = (long) analogRead(A0);
    note_on_time = (long) (note_on_time*0.00782013685*6); //map output between 1.25ms and 2.75ms per pulse (800hz to 364 hz)
    if(note_on_time < 1)
      note_on_time = 1;
    else if(note_on_time > 48)
      note_on_time = 48;

    //FREQUENCY
    freq = (long) analogRead(A1);
    note1freq = (long) 2 * 64;
    //note1freq = (long) (((freq/1023.0)*10000.0)+1)*64; //map to frequency between 1Hz and 10kHz
    if(note1freq < 2*64)
      note1freq = (long) 2*64;
    else if(note1freq > 10000*64)
      note1freq = (long) 10000*64;
    
    
    if(digitalRead(togglePin)){ //if the trigger is pulled
        note1on = true;
        Serial.print("Output HIGH, ON time: ");
        Serial.print(note_on_time);
        Serial.print(", Frequency: ");
        Serial.print(note1freq/64);
    }
    else{
      note1on =false; //stop making noise you fuck
      Serial.print("Burst Mode OFF");
    }
    Serial.print("\n");
  }*/
}

  /**
   * Note Handling Stage
   */

  char note;   // stores the incoming notes

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  note = pitch_name(pitch);
  freq = (long) 64 * pitchFrequency[pitch];
 
  int ch = (int) channel;
  Serial.print("NOTE: ");
  Serial.print(note);
  Serial.print(", FREQ: ");
  Serial.print(freq);
  
  //switch(ch){
    //case(1):{
    if (ch == 1){
      if(!note1on){
        note1on = true;
        if(velocity > 0){
          note1freq = freq;
        }
      }
    }
    //case(2):{
    if (ch == 2){
      if(!note2on){
        note2on = true;
        if(velocity > 0){
          note2freq = freq;
        }
      }
    }
    //case(3):{
    if (ch == 3){
      if(!note3on){
        note3on = true;
        if(velocity > 0){
          note3freq = freq;
        }
      }
    }
 // }
}

void noteOff(byte channel, byte pitch, byte velocity) {
  
 // switch(channel){
    //case(1):{
    if (channel == 1){
      note1on = false;
    }
   //case(2):{
   if (channel == 2){
      note2on = false;
    }
    //case(3):{
    if (channel == 3){
      note3on = false;
    }
  //}
}
 
/*****************************
 * Interrupt Service Routine *
 ***************************/
 
void noteISR() {
  /**
   * Note 1
   */
 if(note1on){
  bres1 += note1freq;  
  if(bres1 >= 2000000)     // using 31.25kHz * 64
  {
    bres1 -= 2000000;
    note1_on_time = note_on_time;
  }
 }

  if(note1_on_time > 0) //on time is # times the period of 31.25kHz (unfortunately this means only integer multiples of 3.2x10^-5s are possible on times
  {
    note1_on_time--;
    //PORTD = B00001000;      // note HI
    digitalWrite(tone1pin, HIGH);
  }
  else {
  //PORTD = B00000000;   // note LO again
  digitalWrite(tone1pin, LOW);
  }
  /**
   * Note 2
   */
 if(note2on){
  bres2 += note2freq;  
  if(bres2 >= 2000000)     // using 31.25kHz * 64
  {
    bres2 -= 2000000;
    note2_on_time = note_on_time;
  }
 }

  if(note2_on_time > 0)
  {
    note2_on_time--;
    //PORTD = B00100000;      // note HI
    digitalWrite(tone2pin, HIGH);
  }
  else {
  //PORTD = B00000000;   // note LO again
  digitalWrite(tone2pin, LOW);
  }

  /**
   * Note 3
   */
 if(note3on){
  bres3 += note3freq;  
  if(bres3 >= 2000000)     // using 31.25kHz * 64
  {
    bres3 -= 2000000;
    note3_on_time = note_on_time;
  }
 }

  if(note3_on_time > 0)
  {
    note3_on_time--;
    //PORTD = B01000000;      // note HI
    digitalWrite(tone3pin, HIGH);
  }
  else {
  //PORTD = B00000000;   // note LO again
  digitalWrite(tone3pin, LOW);
  }
}
