
/**
   Note Handling Stage
*/

char note;   // stores the incoming notes

void noteOn(byte channel, byte pitch, byte velocity) {
  digitalWrite(LEDPin, HIGH);
  //  note = pitch_name(pitch);  broken
  freq = (int) NOTE_MULTIPLIER * pitchFrequency[pitch+12];

  int ch = (int) channel;

  if (ch == 1) {
    if (!note1on) {
      note1on = true;
      if (velocity > 0) {
        note1freq = freq;
      }
    }
  }
  if (ch == 2) {
    if (!note2on) {
      note2on = true;
      if (velocity > 0) {
        note2freq = freq;
      }
    }
  }
  if (ch == 3) {
    if (!note3on) {
      note3on = true;
      if (velocity > 0) {
        note3freq = freq;
      }
    }
  }
}

void noteOff(byte channel, byte pitch, byte velocity) {
  digitalWrite(LEDPin, LOW);
  if (channel == 1) {
    note1on = false;
  }
  if (channel == 2) {
    note2on = false;
  }
  if (channel == 3) {
    note3on = false;
  }
}

