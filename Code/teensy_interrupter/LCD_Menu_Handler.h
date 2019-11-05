#include "pitchFreq.h"
#include <EEPROM.h>
//lcd pins
const int rs = 18, en = 19, d4 = 20, d5 = 21, d6 = 22, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Set LCD refresh rate
unsigned long last_update = millis();
unsigned short period_update = 250; //screen update period in ms

//variables
unsigned long LCD_on_time;
unsigned short LCD_freq;
long ENC_val = 0;
byte focused = 0;

//note selection;
volatile byte notes_i = 70; //start at A4
//String array holding control types
volatile byte ctrl_i = 0;

char* control_type[] = {(char*)"Burst Mode", (char*)"Musical Note", (char*)"USB MIDI", (char*)"MIDI", (char*)"SD Card"};
char* selected_control = control_type[ctrl_i];
char* selected_note = notes[notes_i];

//Save state


// ----- BURST SCREEN -----
LiquidLine screen1_line1(1, 0, "On Time: ", LCD_on_time);
LiquidLine screen1_line2(1, 1, "BPS: ", LCD_freq);
LiquidScreen burst_screen(screen1_line1, screen1_line2);
// --------------------------

// ----- MUSICAL SCREEN -----
LiquidLine screen1_line3(1, 1, "Note: ", selected_note);
LiquidScreen musical_screen(screen1_line1, screen1_line3);
// --------------------------

// ----- MIDI SCREEN -----
LiquidLine screen1_line4(1, 1, "--------------");
LiquidScreen midi_screen(screen1_line1, screen1_line4);
// --------------------------

// ----- MODE SCREEN -----
LiquidLine screen2_line1(1, 0, "Control Mode");
LiquidLine screen2_line2(1, 1, selected_control);
LiquidScreen mode_screen(screen2_line1, screen2_line2);
// --------------------

// ----- SAVE SCREEN -----
LiquidLine screen3_line1(1, 0, "Save Settings ");
LiquidLine screen3_line2(1, 1, "Scroll Fwd: ");
LiquidScreen save_screen(screen3_line1, screen3_line2);
// --------------------------



// Now let's combine the screens into a menu.
LiquidMenu LCD_menu(lcd);


