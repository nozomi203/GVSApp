//wave func
float bipole(float phase){
  if(phase < 0.5){
    return 1;
  }else{
    return -1;
  }
}
float posimono(float phase){
  if(phase < 0.5){
    return 1;
  }else{
    return 0;
  }
}
float negamono(float phase){
  if(phase < 0.5){
    return -1;
  }else{
    return 0;
  }
}
float posicon(float phase){
  return 1;
}
float negacon(float phase){
  return -1;
}

float (*waves[])(float) = {bipole, posimono, negamono, posicon, negacon, bipole, bipole, bipole};

//const int analogOutPin = A0; // Analog output pin that the LED is attached to
int g_Ch = 0;
int g_Val = 0;//0-255
int g_Freq = 0;//0-1023
float (*g_Wave)(float)= bipole;
int data[3] = {0,0,0};
char key = 0;
int g_polarity = 0;

float t = 0;
float ts = 0;
float te = 0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  
  Serial1.begin(9600);
  Serial.begin(9600);
  while (!Serial1);
  while(!Serial);

 
}

int setPolarity(int value){
  if(value > 0){
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
    return value;
  }else{
    digitalWrite(2, HIGH);
    digitalWrite(1, LOW);
    return -value;
  }
}

void loop()
{
  char Dat0, Dat1, Dat2, Dat3, Dat4;
   while(1){
    if(Serial1.available()){
      Dat0 = Serial1.read();
      delay(1);
      Serial.write("Dat0 Receive:");
      Serial.print(Dat0);
      Serial.write("\r\n");
      if(Dat0 == 71){
        digitalWrite(LED_BUILTIN, LOW);
        while(Serial1.available()<=0);
        Dat1 = Serial1.read();
        Serial.write("Dat1 Receive:");
        Serial.print(Dat1);
        Serial.write("\r\n");
        while(Serial1.available()<=0);
        Dat2 = Serial1.read();
        Serial.write("Dat2 Receive:");
        Serial.print(Dat2);
        Serial.write("\r\n");
        while(Serial1.available()<=0);
        Dat3 = Serial1.read();
        Serial.write("Dat3 Receive:");
        Serial.print(Dat3);
        Serial.write("\r\n");
        while(Serial1.available()<=0);
        Dat4 = Serial1.read();
        Serial.write("Dat4 Receive:");
        Serial.print(Dat4);
        Serial.write("\r\n");
        digitalWrite(LED_BUILTIN, HIGH);
  
        ConvertChtoBin(Dat1,Dat2, Dat3, Dat4);
      }
      if(Dat0 == 67){
        Serial.write("Alive.\r\n");
        Serial1.print(g_Ch);
      }
    }
    //update
    te = micros()/(float)1000000;
    float diff = te - ts;
    ts = te;
    t = t + diff * g_Freq;
    t = t - (int)t;
    int value = g_Val * g_Wave(t);
    value = setPolarity(value);
    analogWrite(A0, (char)(value));
    Dat0 = 0;
  }
}

void ConvertChtoBin(char dat1, char dat2, char dat3, char dat4)
{
    int ch =0;
    
    if((dat1 & 0x80) != 0) ch = ch + 2;
    if((dat1 & 0x40) != 0) ch = ch + 1;

    if(g_Ch == ch){

        t = 0;
        te = micros()/ (float)1000000;
        ts = te;
        
        int current = 0;
        int frequency = 0;
        int wave = 0;
        
        if((dat1 & 0x20) != 0) current = current + 2048;
        if((dat1 & 0x10) != 0) current = current + 1024;
        if((dat1 & 0x08) != 0) current = current + 512;
        if((dat1 & 0x04) != 0) current = current + 256;
        if((dat1 & 0x02) != 0) current = current + 128;
        if((dat1 & 0x01) != 0) current = current + 64;
        if((dat2 & 0x80) != 0) current = current + 32;
        if((dat2 & 0x40) != 0) current = current + 16;
        if((dat2 & 0x20) != 0) current = current + 8;
        if((dat2 & 0x10) != 0) current = current + 4;
        if((dat2 & 0x08) != 0) current = current + 2;
        if((dat2 & 0x04) != 0) current = current + 1;

        g_Val = (int) (current / 16);
        Serial.write("Current:");
        Serial.print(g_Val);
        Serial.write("\r\n");

        if((dat2 & 0x02) != 0) frequency = frequency + 512;
        if((dat2 & 0x01) != 0) frequency = frequency + 256;
        if((dat3 & 0x80) != 0) frequency = frequency + 128;
        if((dat3 & 0x40) != 0) frequency = frequency + 64;
        if((dat3 & 0x20) != 0) frequency = frequency + 32;
        if((dat3 & 0x10) != 0) frequency = frequency + 16;
        if((dat3 & 0x08) != 0) frequency = frequency + 8;
        if((dat3 & 0x04) != 0) frequency = frequency + 4;
        if((dat3 & 0x02) != 0) frequency = frequency + 2;
        if((dat3 & 0x01) != 0) frequency = frequency + 1;

        g_Freq = frequency;
        Serial.write("Frequency:");
        Serial.print(g_Freq);
        Serial.write("\r\n");
        
        if((dat4 & 0x80) != 0) wave = wave + 4;
        if((dat4 & 0x40) != 0) wave = wave + 2;
        if((dat4 & 0x20) != 0) wave = wave + 1;

        g_Wave = waves[wave];
    }

}
