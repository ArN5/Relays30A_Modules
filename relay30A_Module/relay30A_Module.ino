//This code is for the 30 amp relay modules
//With the ACS712 Current sensor
//The voltage divided allows you to read voltages up 15 volts

//These are all the global variables that will hold the information I need
double batteryVoltage = 0;
double batteryAmpsDrawn = 0;

double resistorDividerRatio = 4.09; //330K: 100k Voltage divider
//A1 is the Voltage sense in this code

int ACSOFFSET = 0;
//A0 is current sense pin for this code
//Use any digital pin to control the 
//Signal Pin for the relays

double bitsPerVolts = 0;

void setup() {
    
  Serial.begin(9600);   // start serial at 9600 baud for debugging later
  
   //This for an accurate analog Read
   bitsPerVolts = 1024.0/(readVccMilliVolts()*0.001);
   //if exactly 5 volts then 1024bits/5Volts = (204.8 bits/Volt)
  
   ACSOFFSET = calibrateCurrentSense(14);//This is analog pin A0

}

void loop() {
  // put your main code here, to run repeatedly:
  getAmpsAndVoltage();
}


//For 30A ACS712 Module
//https://www.sparkfun.com/datasheets/BreakoutBoards/0712.pdf
//ACS712ELCTR-30A-T Temp –40C to 85C, AMPS ±30, Sensitivity 66 mV/A

void getAmpsAndVoltage()
{
 double Voltage = 0;
//will be a value holder for the voltage read on the analog reads
 
 int RawValue= 0;

 int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A ACS712 Module
 
 //================================================================== 
  
 RawValue = analogRead(A0);
 Voltage = (RawValue / bitsPerVolts)*1000; // Gets you mV
 batteryAmpsDrawn = ((Voltage - ACSOFFSET) / mVperAmp);
 batteryVoltage = analogRead(A1)/bitsPerVolts*resistorDividerRatio;
 
 Serial.print(batteryAmpsDrawn,3);
 Serial.print(" Amps ");
 Serial.print(batteryVoltage);
 Serial.print(" Volts ");
 
 //==================================================================

 
 Serial.println(".");
 
}


long readVccMilliVolts() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}


double calibrateCurrentSense(int analogPin)
{

 double ACSoffset = 0;
 double voltageReadingSum = 0;

    
     for(int i=0; i<30; i++)
   {
    int RawValue = analogRead(analogPin);
    Serial.println(RawValue);
    int Voltage = (RawValue / bitsPerVolts)*1000; // Gets you mV
    voltageReadingSum =  Voltage+voltageReadingSum;
    delay(10);
   }
   
   ACSoffset=voltageReadingSum/30.0; 
   Serial.println(ACSoffset);
 return ACSoffset;
}

