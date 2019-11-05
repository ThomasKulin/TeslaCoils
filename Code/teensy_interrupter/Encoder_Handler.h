const byte ENC_a = 9;
const byte ENC_b = 10;
unsigned long ENC_pos = 10000; 
unsigned long ENC_new;
double knob;
Encoder controlKnob(ENC_a, ENC_b);

byte updateEncoder() { //returns 0 for no rotation, 1 for CW rotation, 2 for CCW rotation
  knob = ((double)controlKnob.read() / 4) + 0.5;
  ENC_new = (long) knob;
  if (ENC_new > ENC_pos) {
    ENC_pos = ENC_new;
    ENC_val = ENC_pos-10000;
    return 1;
  }
  else if (ENC_new < ENC_pos) {
    ENC_pos = ENC_new;
    ENC_val = ENC_pos-10000;
    return 2;
  }
  else
    return 0;
}

