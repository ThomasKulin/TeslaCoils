/*****************************
     Interrupt Service Routine
   ***************************/

void noteISR() {
  /**
     Note 1
  */
  if (note1on) {
    bres1 += note1freq;
    if (bres1 >= 200000 * NOTE_MULTIPLIER)  // using 200kHz * NOTE_MULTIPLIER
    {
      bres1 -= 200000 * NOTE_MULTIPLIER;
      note1_on_time = note_on_time;
    }
  }

  if (note1_on_time > 0) //on time is # times the period of 200KHz (unfortunately this means only integer multiples of 5x10^-6s are possible on times)
  {
    note1_on_time--;
    digitalWrite(tone1pin, HIGH);
  }
  else {
    digitalWrite(tone1pin, LOW);
  }
  /**
     Note 2
  */
  if (note2on) {
    bres2 += note2freq;
    if (bres2 >= 200000 * NOTE_MULTIPLIER)  // using 31.25kHz * NOTE_MULTIPLIER
    {
      bres2 -= 200000 * NOTE_MULTIPLIER;
      note2_on_time = note_on_time;
    }
  }

  if (note2_on_time > 0)
  {
    note2_on_time--;
    digitalWrite(tone2pin, HIGH);
  }
  else {
    digitalWrite(tone2pin, LOW);
  }

  /**
     Note 3
  */
  if (note3on) {
    bres3 += note3freq;
    if (bres3 >= 200000 * NOTE_MULTIPLIER)  // using 31.25kHz * NOTE_MULTIPLIER
    {
      bres3 -= 200000 * NOTE_MULTIPLIER;
      note3_on_time = note_on_time;
    }
  }

  if (note3_on_time > 0)
  {
    note3_on_time--;
    digitalWrite(tone3pin, HIGH);
  }
  else {
    digitalWrite(tone3pin, LOW);
  }
}

