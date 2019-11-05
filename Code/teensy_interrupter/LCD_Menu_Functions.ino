void update_screen() {
  if (millis() - last_update > period_update) {
    last_update = millis();
    LCD_menu.update();
  }
}

void on_time_up() {
  if(note_on_time < MAX_ON_TIME)
    note_on_time++;
}
void on_time_down() {
  if(note_on_time > 0)
    note_on_time--;
}

void freq_up() {
  freq+=delta;
}
void freq_down() {
    freq-=delta;
    if(freq > 60000)
      freq=0;
}
void change_delta(){
  if(delta == 1)
    delta = 25;
  else if(delta == 25)
    delta = 100;
  else if(delta == 100)
    delta = 1;
}

void note_up() {
  if (notes_i < (sizeof(notes)/sizeof(char*))-1)
    notes_i++;
  else 
    notes_i = 0;
    
  selected_note = notes[notes_i];
}
void note_down() {
    if (notes_i > 0)
    notes_i--;
  else 
    notes_i = (sizeof(notes)/sizeof(char*))-1;
    
  selected_note = notes[notes_i];
}

void control_up() {
  if (ctrl_i < (sizeof(control_type)/sizeof(char*))-1)
    ctrl_i++;
  else 
    ctrl_i = 0;
    
  selected_control = control_type[ctrl_i];
  hide_unused_screens();
}

void control_down() {
  if (ctrl_i > 0) 
    ctrl_i--;
  else 
    ctrl_i = (sizeof(control_type)/sizeof(char*))-1;
     
  selected_control = control_type[ctrl_i];
  hide_unused_screens();
}

void save_settings(){
  EEPROM.update(0, ctrl_i); //selected control mode
  EEPROM.update(1, notes_i); //selected note
  EEPROMWriteInt(2, note_on_time); //set on time
  EEPROMWriteInt(4, freq); //set BPS frequency
  Serial.print(ctrl_i);
  Serial.println(notes_i);
}

void read_settings(){
  ctrl_i = EEPROM.read(0); //selected control mode
  selected_control = control_type[ctrl_i];
  notes_i = EEPROM.read(1); //selected note
  selected_note = notes[notes_i];
  hide_unused_screens();
  note_on_time = EEPROMReadInt(2); //set on time
  freq = EEPROMReadInt(4); //set BPS frequency
}
void print_settings(){
  Serial.print(EEPROM.read(0));
  Serial.print(", ");
  Serial.print(EEPROM.read(1));
  Serial.print(", ");
  Serial.print(control_type[ctrl_i]);
  Serial.print(", ");
  Serial.print(notes[notes_i]);
  Serial.print(", ");
  Serial.print(ENC_val);
  Serial.print(", ");
  Serial.println(knob);
  
}

void hide_unused_screens(){
  switch (ctrl_i){
    case 0: //burst mode
      burst_screen.hide(false);
      musical_screen.hide(true); 
      midi_screen.hide(true);
      break;
    case 1: //musical mode
      burst_screen.hide(true);
      musical_screen.hide(false); 
      midi_screen.hide(true);
      break;
    case 2: //midi mode
    case 3: //midi mode
      burst_screen.hide(true);
      musical_screen.hide(true); 
      midi_screen.hide(false);
      break;
    case 4: //sd card
      burst_screen.hide(true);
      musical_screen.hide(true); 
      midi_screen.hide(true);
      break;
  }
}


