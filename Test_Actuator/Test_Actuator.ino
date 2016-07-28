void setup() {
  // initialize digital pin 13 as an output.
  for(int i=2;i<14;i++)
    pinMode(i, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
//  digitalWrite(2, HIGH);
  for(int i=2;i<14;i++)
  {
    digitalWrite(i, HIGH);   
    for(int j=2;j<14;j++)
    {
      if(j!=i)
        digitalWrite(j, LOW); 
    }
    delay(200);
  }
}
