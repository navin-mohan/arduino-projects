/*
              Automatic Plant Watering System Using Soil Moisture Sensor
          -------------------------------------------------------------
          This is an Arduino based Plant watering system that uses a soil moisture sensor
          to monitor the garden soil and automatically water the plants when the moisture
          level falls below a threshold value. The minimum moisture level can be set using
           a Potentiometer.

           Arduino Pin Config:
            -> A0 : The analog output from the moisture sensor
            -> A3 : The analog input from the Potentiometer
            -> D6 : The digital output to the relay module

          I used an Arduino Uno to test this but an ATtiny85 will work fine especially if its a permanent setup as it
          is smaller and cheaper.

           Author: Navin Mohan
           email: navinmohan@live.com
           GitHub : https://github.com/nvnmo/arduino-projects
           
           Follow us on our blog for more
           http://nvnmo.blogspot.in

*/
#define MOISTURE_SENSOR_INPUT_A  0//analog input of the moisture sensor
#define MOISTURE_SENSOR_INPUT_D  4//digital input of the moisture sensor(useless right now)

#define CALIB_POT_INPUT_A 3//Potentiometer(10k) input(analog)
#define WATER_SUPPLY_OUTPUT 6 // to turn ON the water pump(To TIP122)

#define DEFAULT_MAX_WATER_LEVEL 0.6 //default to fill upto
#define MOISTURE_REFRESH_INTERVAL 1 //moisture refresh interval for waterPlant() in seconds

#define DEBUG 1 //comment out during final compilation

double getPotInputInPercent(); // to return the CALIB_POT input in percent
double getMoistSensorInputInPercent(); //to return the Moisture Sensor input in percent
void waterPlant(double toPercent); // to water the plant to the given moisture level



void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  pinMode(MOISTURE_SENSOR_INPUT_D,INPUT); //set pins as inputs and outputs
  pinMode(WATER_SUPPLY_OUTPUT,OUTPUT);
  digitalWrite(WATER_SUPPLY_OUTPUT,LOW); //set the pump OFF by default
}

void loop() {
  double req_moisture_level = getPotInputInPercent();  //the value set on POT
  double current_moisture = getMoistSensorInputInPercent();//get current moisture value
  #ifdef DEBUG //only for debugging
  Serial.print("pot:");
  Serial.println(req_moisture_level);
  Serial.print("current moisture:");
  Serial.println(current_moisture);
  #endif
  if(current_moisture < req_moisture_level ){ //if the moisture level falls below the minimum required
    waterPlant(req_moisture_level); //water the plants
  }
}

void waterPlant(double toPercent = DEFAULT_MAX_WATER_LEVEL){
  digitalWrite(WATER_SUPPLY_OUTPUT,HIGH); //turn the pump ON
  double d = getMoistSensorInputInPercent();
  while(d < toPercent){ //wait till the the moisture level reach the required minimum
   delay(MOISTURE_REFRESH_INTERVAL*1000);
   d = getMoistSensorInputInPercent();
  }
  digitalWrite(WATER_SUPPLY_OUTPUT,LOW); //turn pump OFF
}
double getPotInputInPercent(){
  return analogRead(CALIB_POT_INPUT_A)/1023.0;
}

double getMoistSensorInputInPercent(){
  return 1 - analogRead(MOISTURE_SENSOR_INPUT_A)/1023.0; //the moisture sensor provides max val at minimum moisture level
}

