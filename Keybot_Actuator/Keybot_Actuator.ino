int inputarray[12];
unsigned long scale;
int results[12];
int note[3] = {0, 0, 0};
bool flag;

int tempo_line = 5;
int note_line = 6;
int scale_bit0_line = 0;
int scale_bit1_line = 1;
int scale_bit2_line = 2;
int scale_bit3_line = 3;
int scale_bit4_line = 4;

void case2(int *inputarray, int *results ,int scales)
{
  for(int i = 12; i--;)
    results[i] = inputarray[i];
}

void case3(int *inputarray, int *results ,int scale)
{
  int major;
  int rootnote,lastnote, midnote;

  for(int i = 12; i--;)
    results[i] = 0;

  major = scale % 2;

  rootnote = (scale + major) / 2;

  lastnote = (rootnote + 7) % 12;
  if(!lastnote)
    lastnote = 12;

  midnote=(rootnote + 3 + major) % 12;
  if(!midnote)
    midnote=12;

  results[lastnote - 1] = 1;
  results[rootnote - 1] = 1;
  results[midnote - 1] = 1;
}

void case1(int *inputarray, int *results ,int scales)
{
  //initialize array
  int highnote;
  int major = scale % 2;
  int rootnote = ((scale + 1) / 2) - 1;
  int lastnote;
  int midnote = 0;
  int x=0, y=0;

  for(int i = 12; i--;)
    results[i] = 0;

  for(int i = 12; i--;)
    if(inputarray[i])
    {
      highnote = i;     
      break;
    } Serial.println(highnote);

  if(major == 1)
  {
    if(highnote == (rootnote + 1) % 12)
      highnote--;
    else if(highnote == (rootnote + 3) % 12)
      highnote++;
    else if(highnote == (rootnote + 6) % 12)
      highnote--;
    else if(highnote == (rootnote + 8) % 12)
      highnote++;
    else if(highnote == (rootnote + 10) % 12)
      highnote--;
    else if(highnote == (rootnote + 11) % 12)
      highnote = rootnote;
  }
  else
  {
    if(highnote == (rootnote + 1) % 12)
      highnote--;
    else if(highnote == (rootnote + 4) % 12)
      highnote--;
    else if(highnote == (rootnote + 6) % 12)
      highnote++;
    else if(highnote == (rootnote + 8) % 12)
      highnote++;
    else if(highnote == (rootnote + 11) % 12)
      highnote = rootnote;
  }
  switch(major)
  {
    case 1:
      if (highnote == rootnote || highnote == (5+rootnote)%12 ||highnote == (7+rootnote)%12 || highnote == 0)
        x=1;
      else if(highnote == (2+rootnote)%12 || highnote == (4+rootnote)%12 ||highnote == (9+rootnote)%12)
        y=1;
      break;
    case 0:
      if(highnote == (3+rootnote)%12 || highnote == (9+rootnote)%12 ||highnote == (10+rootnote)%12)
        x=1;
      else if(highnote == (0+rootnote)%12 || highnote == (2+rootnote)%12 ||highnote == (5+rootnote)%12 ||highnote == (7+rootnote)%12 || highnote == 0)
        y=1;
  }

  lastnote = (highnote + 7) % 12;

  if(x)
  {
    midnote = (4 + highnote) % 12;
    results[midnote] = 1;
  }
  else if(y)
  {
    midnote = (3 + highnote) % 12;
    results[midnote] = 1;
  }
  Serial.println(rootnote); Serial.println(highnote); Serial.println(midnote); Serial.println(lastnote);

  results[highnote] = 1;
  results[midnote] = 1;
  results[lastnote] = 1;
}

int scale_bit[5];

void setup() {
  Serial.begin(9600);
  for(int i = 2; i <= 13; i++)
    pinMode(i, OUTPUT);
  flag = true;
}

unsigned int count = 0, switchsum = 0, count2;
void loop() {
  if (analogRead(tempo_line) >= 700)
  {
    if (count < 12 && flag)
    {
      flag = false;
      digitalWrite(note[0], LOW);
      digitalWrite(note[1], LOW);
      digitalWrite(note[2], LOW);
      if (analogRead(note_line) >= 500)
      {
        inputarray[count++] = 1;
        switchsum++;
        Serial.print(".");
      }
      else
        inputarray[count++] = 0;
    }
  }
  else
    flag = true;

  if(count == 12)
  {
    count = 0;
    scale_bit[0] = (analogRead(scale_bit0_line) >= 900) ? 1 : 0;
    scale_bit[1] = (analogRead(scale_bit1_line) >= 900) ? 1 : 0;
    scale_bit[2] = (analogRead(scale_bit2_line) >= 900) ? 1 : 0;
    scale_bit[3] = (analogRead(scale_bit3_line) >= 900) ? 1 : 0;
    scale_bit[4] = (analogRead(scale_bit4_line) >= 900) ? 1 : 0;
    scale = scale_bit[0] + (scale_bit[1] << 1) + (scale_bit[2] << 2) + (scale_bit[3] << 3) + (scale_bit[4] << 4);
    Serial.print("Scale: "); Serial.println(scale);
//    Serial.println(switchsum);
    switch(switchsum)
    {
      case 0:
        results[0] = 0; results[1] = 0; results[2] = 0; results[3] = 0; results[4] = 0; results[5] = 0;
        results[6] = 0; results[7] = 0; results[8] = 0; results[9] = 0; results[10] = 0; results[11] = 0;
        break;
      
      case 1:
        case1(inputarray,results, scale);
        break;

      case 2: case 3:
        case2(inputarray,results, scale);     
        break;

      case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12:
        case3(inputarray,results, scale);
    }
    
    switchsum = 0;
    count2 = 0;
    for (int i = 0; i < 12; i++)
      if (results[i])
      {
//        Serial.println(i);
        note[count2++] = i + 2;
      }

//    Serial.print("Number of high notes: "); Serial.println(count);
    if (count2 == 1)
    {
      digitalWrite(note[0], HIGH);
    }
    else if (count2 == 2)
    {
      digitalWrite(note[0], HIGH);
      digitalWrite(note[1], HIGH);
    }
    else if (count2 == 3)
    {
      digitalWrite(note[0], HIGH);
      digitalWrite(note[1], HIGH);
      digitalWrite(note[2], HIGH);
    }
    Serial.print(note[0]-1); Serial.print(";"); Serial.print(note[1]-1); Serial.print(";");  Serial.println(note[2]-1);
  }
}
