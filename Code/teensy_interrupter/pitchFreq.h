// Conversion from MIDI pitch codes to frequencies in Hz
static const float pitchFrequency[] = {
  8.176,     8.662,     9.177,     9.723,    10.301,    10.913,    11.562,    12.250,    12.978,    13.750,    14.568,    15.434,
  16.352,    17.324,    18.354,    19.445,    20.602,    21.827,    23.125,    24.500,    25.957,    27.500,    29.135,    30.868,
  32.703,    34.648,    36.708,    38.891,    41.203,    43.654,    46.249,    48.999,    51.913,    55.000,    58.270,    61.735,
  65.406,    69.296,    73.416,    77.782,    82.407,    87.307,    92.499,    97.999,   103.826,   110.000,   116.541,   123.471,
  130.813,   138.591,   146.832,   155.563,   164.814,   174.614,   184.997,   195.998,   207.652,   220.000,   233.082,   246.942,
  261.626,   277.183,   293.665,   311.127,   329.628,   349.228,   369.994,   391.995,   415.305,   440.000,   466.164,   493.883,
  523.251,   554.365,   587.330,   622.254,   659.255,   698.456,   739.989,   783.991,   830.609,   880.000,   932.328,   987.767,
  1046.502,  1108.731,  1174.659,  1244.508,  1318.510,  1396.913,  1479.978,  1567.982,  1661.219,  1760.000,  1864.655,  1975.533,
  2093.005,  2217.461,  2349.318,  2489.016,  2637.020,  2793.826,  2959.955,  3135.963,  3322.438,  3520.000,  3729.310,  3951.066,
  4186.009,  4434.922,  4698.636,  4978.032,  5274.041,  5587.652,  5919.911,  6271.927,  6644.875,  7040.000,  7458.620,  7902.133,
  8372.018,  8869.844,  9397.273,  9956.063, 10548.082, 11175.303, 11839.822, 12543.854,
};

char* notes[] = {
  "C -1",     "C# -1",     "D -1",     "D# -1",    "E -1",    "F -1",    "F# -1",    "G -1",    "G# -1",    "A -1",    "A# -1",    "B -1",
  "C 0",     "C# 0",     "D 0",     "D# 0",    "E 0",    "F 0",    "F# 0",    "G 0",    "G# 0",    "A 0",    "A# 0",    "B 0",
  "C 1",     "C# 1",     "D 1",     "D# 1",    "E 1",    "F 1",    "F# 1",    "G 1",    "G# 1",    "A 1",    "A# 1",    "B 1",
  "C 2",     "C# 2",     "D 2",     "D# 2",    "E 2",    "F 2",    "F# 2",    "G 2",    "G# 2",    "A 2",    "A# 2",    "B 2",
  "C 3",     "C# 3",     "D 3",     "D# 3",    "E 3",    "F 3",    "F# 3",    "G 3",    "G# 3",    "A 3",    "A# 3",    "B 3",
  "C 4",     "C# 4",     "D 4",     "D# 4",    "E 4",    "F 4",    "F# 4",    "G 4",    "G# 4",    "A 4",    "A# 4",    "B 4",
  "C 5",     "C# 5",     "D 5",     "D# 5",    "E 5",    "F 5",    "F# 5",    "G 5",    "G# 5",    "A 5",    "A# 5",    "B 5",
  "C 6",     "C# 6",     "D 6",     "D# 6",    "E 6",    "F 6",    "F# 6",    "G 6",    "G# 6",    "A 6",    "A# 6",    "B 6",
  "C 7",     "C# 7",     "D 7",     "D# 7",    "E 7",    "F 7",    "F# 7",    "G 7",    "G# 7",    "A 7",    "A# 7",    "B 7",
  "C 8",     "C# 8",     "D 8",     "D# 8",    "E 8",    "F 8",    "F# 8",    "G 8",    "G# 8",    "A 8",    "A# 8",    "B 8",
  "C 9",     "C# 9",     "D 9",     "D# 9",    "E 9",    "F 9",    "F# 9",    "G 9",
  };

