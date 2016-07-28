/*Tweak according to requirement*/
#define THRESHOLD 4000 //input volume threshold; decrease for running at lower volumes, but more prone to noise
#define NOTE_PULSE_INTERVAL 3 //half of interval of note enable cycle
#define TEMPO_CHANGE_INTERVAL 1 //by how much tempo increase/decrease should be made
#define DEFAULT_TEMPO 30 //default tempo

/*frequency table*/
#define C4 261.6256
#define Cs4 277.1826
#define D4 293.6648
#define Ds4 311.1270
#define E4 329.6276
#define F4 349.2282
#define Fs4 369.9944
#define G4 391.9954
#define Gs4 415.3047
#define A4 440.0000
#define As4 466.1638
#define B4 493.8833

#define C5 523.2511
#define Cs5 554.3653
#define D5 587.3295
#define Ds5 622.2540
#define E5 659.2551
#define F5 698.4565
#define Fs5 739.9888
#define G5 783.9909
#define Gs5 830.6094
#define A5 880.0000
#define As5 932.3275
#define B5 987.7666

/*AudioCodecShield preprocessor directives*/
#define SAMPLE_RATE 44
#define ADCS 1
#define BYPASS 1
#include <Wire.h>
#include <SPI.h>
#include <AudioCodec.h>

/*Cos/Sin preprocessor directive*/
#include <SpeedTrig.h>

#define SAMPLE_SIZE 270
#define BIN_SIZE 24
#define NOTE_PIN 3 //note output pin
#define TEMPO_CLK_PIN 2 //note output enable pin
#define BUSY_PIN 6 //busy indicator pin; HIGH-busy, LOW-reading tempo change
#define TEMPO_INC_PIN 7 //tap to increase tempo
#define TEMPO_DEC_PIN 4 //tap to decrease tempo

//default tempo
unsigned int tempo = DEFAULT_TEMPO;

/*Global Variables*/
double frequencies[BIN_SIZE];
int samples[(SAMPLE_SIZE)];
double spectrum[(BIN_SIZE)];
int notes[12];
double factor_fft;

void init_frequencies()
{
  int factor = 1;
  frequencies[0]  = Fs4 * factor;
  frequencies[1]  = G4 * factor;
  frequencies[2]  = Gs4 * factor;
  frequencies[3]  = A4 * factor;
  frequencies[4] = As4 * factor;
  frequencies[5] = B4 * factor;

  frequencies[6] = C5 * factor;
  frequencies[7] = Cs5 * factor;
  frequencies[8] = D5 * factor;
  frequencies[9] = Ds5 * factor;
  frequencies[10] = E5 * factor;
  frequencies[11] = F5 * factor;
  frequencies[12] = Fs5 * factor;
  frequencies[13] = G5 * factor;
  frequencies[14] = Gs5 * factor;
  frequencies[15] = A5 * factor;
  frequencies[16] = As5 * factor;
  frequencies[17] = B5 * factor++;

  frequencies[18]  = C5 * factor;
  frequencies[19]  = Cs5 * factor;
  frequencies[20]  = D5 * factor;
  frequencies[21]  = Ds5 * factor;
  frequencies[22]  = E5 * factor;
  frequencies[23]  = F5 * factor;
}

void setup() {
  Serial.begin(9600);
  pinMode(TEMPO_INC_PIN, INPUT);
  pinMode(TEMPO_DEC_PIN, INPUT);
  pinMode(NOTE_PIN, OUTPUT);
  pinMode(TEMPO_CLK_PIN, OUTPUT);
  pinMode(BUSY_PIN, OUTPUT);
  digitalWrite(NOTE_PIN, LOW);
  digitalWrite(TEMPO_CLK_PIN, LOW);
  digitalWrite(BUSY_PIN, LOW);
  factor_fft = (double) 360.0 / (double) SAMPLE_SIZE;
  init_frequencies();
  AudioCodec_init();
}

void read_inputs()
{
  unsigned int clk = 0, lix = 0;
  int left_in, right_in;
  while (clk < SAMPLE_SIZE)
  {
    lix = left_in;
    AudioCodec_data(&left_in, &right_in, 0, 0);
    if (lix != left_in)
      samples[clk++] = left_in;
  }
}

void spectrum_analysis()
{
  int i, j;
  double angle_factor, angle;
  double cosx;
  double sumReal;
  double sumImag;
  angle_factor = factor_fft * BIN_SIZE;
  for (i = BIN_SIZE; i--;)
  {
    sumReal = 0;
    sumImag = 0;
    angle_factor -= factor_fft;
    angle = angle_factor * SAMPLE_SIZE;
    for (j = SAMPLE_SIZE; j--;)
    {
      angle -= angle_factor;
      cosx = SpeedTrig.cos(angle);
      sumReal += cosx * (double) samples[j];
//      sumImag += sqrt(1 - (cosx * cosx)) * (double) samples[j];
    }
//    spectrum[i] = sqrt((sumReal * sumReal) + (sumImag * sumImag));
    spectrum[i] = abs(sumReal);
  }
}

void write_outputs()
{
  unsigned int clk = 0;
  unsigned long clock_timer = 0;
  bool note_clock = false;
  while (clk < 24)
  {
    clock_timer = millis();
    note_clock = !note_clock;
    digitalWrite(TEMPO_CLK_PIN, note_clock);
    if (!(clk & 1))
      digitalWrite(NOTE_PIN, notes[clk >> 1]);
    else
      digitalWrite(NOTE_PIN, LOW);
    while (millis() - clock_timer < NOTE_PULSE_INTERVAL);
    clk++;
  }
}

int max_spectrum(unsigned long *maxx)
{
  int i;
  *maxx = 0;
  int max_index;
  for (i = 11; i <= 22; i++)
    if (*maxx < spectrum[i])
    {
      *maxx = spectrum[i];
      max_index = i;
    }
  max_index -= 5;
  if (max_index >= 12)
    max_index -= 12;
  return max_index;
}

void debug_spectrum()
{
  int i;
  for (i = 0; i < BIN_SIZE >> 1; i++)
  {
    Serial.print(spectrum[i]); Serial.print(",");
  }
  Serial.println("");
  for (i = BIN_SIZE >> 1; i < BIN_SIZE; i++)
  {
    Serial.print(spectrum[i]); Serial.print(",");
  }
  Serial.println("");
}

void debug_notes()
{
  int i;
  Serial.print(notes[0]); Serial.print(notes[1]); Serial.print(notes[2]); Serial.print(notes[3]); Serial.print(notes[4]); Serial.print(notes[5]); Serial.print(notes[6]); Serial.print(notes[7]); Serial.print(notes[8]); Serial.print(notes[9]); Serial.print(notes[10]); Serial.print(notes[11]); 
  Serial.println("");
}

void loop() {
  if (digitalRead(TEMPO_DEC_PIN))
    return;
  Serial.println(tempo);
  unsigned long startTime = millis();
  digitalWrite(BUSY_PIN, HIGH);
  unsigned long maxx;
//  noInterrupts();
  read_inputs();
  spectrum_analysis();
  int max_index = max_spectrum(&maxx);

  /*Set the Note vector*/
  for (int i = 0; i < 12; i++)
    notes[i] = 0;
  if (maxx > THRESHOLD)
    notes[max_index] = 1;

  /*Debug - Print time delay*/
//  Serial.println(millis() - startTime);

  /* Test Output*/
//  notes[0] = 0; notes[1] = 0; notes[2] = 0; notes[3] = 0; notes[4] = 0; notes[5] = 1; notes[6] = 0; notes[7] = 0; notes[8] = 0; notes[9] = 0; notes[10] = 0; notes[11] = 0;

//  debug_spectrum();
//  debug_notes();

  write_outputs();
  digitalWrite(BUSY_PIN, LOW);

  /*wait till next tempo interval starts*/
  unsigned int interval = 60000 / tempo;
  int inc = 0, dec = 0;
  bool inc_flag = false, dec_flag = false;
  while (1)
  {
    if (!inc_flag && digitalRead(TEMPO_INC_PIN) == HIGH)
    {
      inc_flag = true;
      delay(10);
//      Serial.println("I+");
    }
    else if (inc_flag && digitalRead(TEMPO_INC_PIN) == LOW)
    {
      inc_flag = false;
      delay(10);
//      Serial.println("I-");
      inc++;
    }
    if (!dec_flag && digitalRead(TEMPO_DEC_PIN) == HIGH)
    {
      dec_flag = true;
      delay(10);
//      Serial.println("D+");
    }
    else if (dec_flag && digitalRead(TEMPO_DEC_PIN) == LOW)
    {
      dec_flag = false;
      delay(10);
//      Serial.println("D-");
      dec++;
    }
    if (startTime + interval - millis() <= 5)
      break;
  }
  
  tempo += TEMPO_CHANGE_INTERVAL * (inc - dec);
  delay(interval - millis() + startTime);
}
