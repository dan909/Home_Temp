#include <LowPower.h>
#include <OneWire.h> 
float avlight = 1, alllight = 1, thislight = 1, nightlight = 1;
float avtemp = 5, maxtemp = 0, mintemp = 45, alltemp = 0, thistemp = 0;
float sleep = 1;

#define ArrayDay 145 // 145
int counter = 0;
int tsensorPin = 2;
#define lsensorPin 1
#define vsensorPin 0

#define RED 11
int brightRED = 255;

#define ORINGE 10
int brightORINGE = 255;

#define YELLOW 9
int brightYELLOW = 255;

#define BLUE 6
int brightBLUE = 255;

#define GREEN 5
int brightGREEN = 255;

#define VILO 3
int brightVILO = 255;


OneWire ds(tsensorPin); // on digital pin 2

float DayTemps[ArrayDay]={0};


void setup() {
    
    //Wire.begin();
 
    Serial.begin(9600);
    
    pinMode(RED, OUTPUT);
    pinMode(ORINGE, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(VILO, OUTPUT);

    Serial.print(getVolt());
    Serial.println(" =Volts");
    Serial.print(getLight());
    Serial.println(" =lights");
    Serial.print(getTemp());
    Serial.println(" =temp");

    if (getVolt() < 5.7) {
      for(int Loop = 0; Loop < floor(getVolt()); Loop++) {
        analogWrite(RED, 255);
        delay(400);
        analogWrite(RED, 0);
        delay(400);
      }
    hibenate();
  } else {
    Serial.println("Display test in progress: - ");
    for (int x = 0; x < 42; x++) { // desplay temp function
      desplay(x);
      Serial.print(x);
      analogWrite(RED, brightRED);
      analogWrite(ORINGE, brightORINGE);
      analogWrite(YELLOW, brightYELLOW);
      analogWrite(BLUE, brightBLUE);
      analogWrite(GREEN, brightGREEN);
      analogWrite(VILO, brightVILO);
      delay(500); //100
      Serial.print(" - ");
    }
    brightRED = 0;
    brightORINGE = 0;
    brightYELLOW = 0;
    brightBLUE = 0;
    brightGREEN = 0;
    brightVILO = 0;
    analogWrite(RED, brightRED);
    analogWrite(ORINGE, brightORINGE);
    analogWrite(YELLOW, brightYELLOW);
    analogWrite(BLUE, brightBLUE);
    analogWrite(GREEN, brightGREEN);
    analogWrite(VILO, brightVILO);
  }
}

float getTemp() {
   byte data[12];
   byte addr[8];
  
   if ( !ds.search(addr)) {
     //no more sensors on chain, reset search
     ds.reset_search();
     return -1000;
   }
  
   ds.reset();
   ds.select(addr);
   ds.write(0x44,0); // start conversion, with parasite power on at the end

   Serial.flush();
   LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_ON); 
   
   byte present = ds.reset();
   ds.select(addr);  
   ds.write(0xBE); // Read Scratchpad
  
   for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
   }
   
   ds.reset_search();
   
   byte MSB = data[1];
   byte LSB = data[0];
  
   float tempRead = ((MSB << 8) | LSB); //using two's compliment
   float TemperatureSum = tempRead / 16;
   
   return TemperatureSum;
}

float getLight() {
  float light = analogRead(lsensorPin);
  return light;
}

float getVolt() {
  float reading = analogRead(vsensorPin);
  for (int sample = 1; sample <= 10; sample++) {
    reading = reading + analogRead(vsensorPin);
  }
  reading = reading/11;
  float volt = (reading * (5.0/1023.0))*2.0;//((5/1024)*reading)*2;
  Serial.println("vlot");
  Serial.println(volt);
  return volt;
}

void AvrLT(int N) {
  float light = 0;
  for (int sample = 1; sample <= N; sample++) {
    light = light + getLight();
  }
  thistemp = getTemp();
  thislight = light/N;
  //Serial.println("redings temp, light");
  //Serial.println(thistemp);
  //Serial.println(thislight);  
}

void SetMinMax() {
  if (thistemp < mintemp) {
    mintemp = thistemp;
  }
  if (thistemp > maxtemp) {
    maxtemp = thistemp;
  }
}

void hibenate() {
  while(getVolt() < 6.3) {
    sleep8(10800);
    }
  }

void sleep8(float multy8) {
  if (getVolt() < 6.7) {
    Serial.flush();
    for (int x = 1; x <= multy8; x++) {
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_ON); 
    }
  } else {
    if (getVolt() < 6.9) {
        for (int x = 0; x < 42; x++) { // desplay temp function
          desplay(x);
          analogWrite(RED, brightRED);
          analogWrite(ORINGE, brightORINGE);
          analogWrite(YELLOW, brightYELLOW);
          analogWrite(BLUE, brightBLUE);
          analogWrite(GREEN, brightGREEN);
          analogWrite(VILO, brightVILO);
          delay(5000); //100
        }
    }
    delay(multy8*8*1000);
  }
}

void desplay(float playTemp) {
    
  if (playTemp > 0 && playTemp < 12) {
    brightVILO = map(playTemp, 4, 12, 0, 255);
    brightVILO = 255 - brightVILO;
  } else {
    brightVILO = 0;
  }

  if (playTemp > 9 && playTemp < 17) {
    brightGREEN = map(playTemp, 9, 17, 0, 255);
    brightGREEN = 255 - brightGREEN;
  } else {
    brightGREEN = 0;
  }

  if (playTemp > 14 && playTemp < 22) {
    brightBLUE = map(playTemp, 14, 22, 0, 255);
    brightBLUE = 255 - brightBLUE;
  } else {
    brightBLUE = 0;
  }

  if (playTemp > 17 && playTemp < 29) {
    brightYELLOW = map(playTemp, 17, 29, 0, 255);
  } else {
    brightYELLOW = 0;
  }

  if (playTemp > 22 && playTemp < 30) {
    brightORINGE = map(playTemp, 22, 30, 0, 255);
  } else {
    brightORINGE = 0;
  }

  if (playTemp > 28 && playTemp < 55) {
    brightRED = map(playTemp, 28, 35, 0, 255);
  } else {
    brightRED = 0;
  }

}

void Day() {
  counter = 0;
  if (getVolt() < 5.7) {
    for(int Loop = 0; Loop < floor(getVolt()); Loop++) {
      analogWrite(RED, 255);
      delay(400);
      analogWrite(RED, 0);
      delay(400);
    }
    hibenate();
  }
  for (int x = 1; x < ArrayDay-1; x++) {
    Serial.print("Day - ");
    Serial.println(x);
    Serial.print("Counter - ");
    Serial.println(counter);
    AvrLT(25); // Avriges the Light and Temp N times sets them globely to #thislight #thistemp
    SetMinMax(); // re rites #maxtemp and #mintemp based on #thistemp
    
    alltemp = alltemp + thistemp;
    alllight = alllight + thislight;

    Serial.println("This ligt vs avlight");
    Serial.println(thislight);
    Serial.println(avlight);
    
    if (thislight < avlight - 100 || thislight < 100) {
      counter = counter + 1;
    } else {
      if(counter > 0) {
        counter = counter - 1;
      }
    }

    DayTemps[x] = thistemp;
    avtemp = alltemp / float(x);
    avlight = alllight / float(x);
    sleep8(75); // 75

    if (counter > 4) {
      break;
    }
  }
  nightlight = (thislight) / 3;
}

void Night() {
  
  counter = 0;
  
  while (counter < 10) {
    Serial.println("Night Falls");
    AvrLT(25); // Avriges the Light and Temp N times sets them globely to #thislight #thistemp

    Serial.print("This light: "); Serial.println(thislight);
    Serial.print("Average light: "); Serial.println(alllight);
    
    if (thislight > alllight + 20) {
      counter = counter + 1;
    } else if (thislight < alllight) {
      if(counter > 0) {
        counter = counter - 1;
      }
    }
    
    Serial.print("Counter = ");
    Serial.println(counter);

    float volt = getVolt(); 
    
    if (volt > 6.6) {
      sleep = 1;
      Serial.println("VOLTAGE TOO HIGH !!!!!!!!!!!!!!!!!!!");
    } else {

      if (volt < 5.7) {
        for(int Loop = 0; Loop < floor(volt); Loop++) {
          analogWrite(RED, 255);
          delay(400);
          analogWrite(RED, 0);
          delay(400);
        }
        hibenate();
      } else {
        Serial.println("Display in progress");
        for (int x = 0; x < ArrayDay-1; x++) { // desplay temp function
          desplay(DayTemps[x]);
          analogWrite(RED, brightRED);
          analogWrite(ORINGE, brightORINGE);
          analogWrite(YELLOW, brightYELLOW);
          analogWrite(BLUE, brightBLUE);
          analogWrite(GREEN, brightGREEN);
          analogWrite(VILO, brightVILO);
          delay(200); //100
        }
  
        brightRED = 0;
        brightORINGE = 0;
        brightYELLOW = 0;
        brightBLUE = 0;
        brightGREEN = 0;
        brightVILO = 0;
        analogWrite(RED, brightRED);
        analogWrite(ORINGE, brightORINGE);
        analogWrite(YELLOW, brightYELLOW);
        analogWrite(BLUE, brightBLUE);
        analogWrite(GREEN, brightGREEN);
        analogWrite(VILO, brightVILO);
      }
    }
    
    if (thislight > alllight + 40) {
      sleep = 1;
    } else {
      if(random(1,8) > 3) {
        sleep = ceil(((sleep*3.0)/2.0)-((sleep*8.0)/60)); // lots of 8 seconds to sleep
      }
    }
    Serial.print("Sleep is: ");
    Serial.println(sleep);
    sleep8(sleep);// Call Sleep function
    alllight = thislight;
  }
}


void loop() {
  

  Day();

  Night();

  Serial.println("End Loop");


  DayTemps[140]={0};
  counter = 0;
  maxtemp = 0, mintemp = 45, alltemp = 0, alllight = 1, thistemp = 0, thislight = 1;
  //sleep for 10 mins
}



