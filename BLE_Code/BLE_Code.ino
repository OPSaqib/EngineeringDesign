//Libraries:
#include <ArduinoBLE.h>
#include <Arduino.h>
#include <TimeLib.h>

//Service:
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic hourCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic minCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);  

//FOR Hydration 1
BLEByteCharacteristic remindhour1Characteristic("19B10007-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic remindminute1Characteristic("19B10008-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//FOR Medications 1
BLEByteCharacteristic remindhour2Characteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic remindminute2Characteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//FOR Repeating Each Reminder:
BLEByteCharacteristic repeat1Characteristic("19B10009-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic repeat2Characteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic repeat3Characteristic("19B10019-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic repeat4Characteristic("19B10020-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//FOR Hydration 2
BLEByteCharacteristic remindhour3Characteristic("19B10015-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic remindminute3Characteristic("19B10016-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//FOR Medications 2
BLEByteCharacteristic remindhour4Characteristic("19B10017-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic remindminute4Characteristic("19B10018-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//FOR Resetting Reminders:
BLEByteCharacteristic resetCharacteristic("19B10010-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//Pins Init:
#define motor_pin D2 
#define redled_pin A7
#define blueled_pin A6

//Start time measurement:
time_t t ;

//Reminder time hydration (to be updated ahead) 1:
int remhour1 = -1;
int remminute1 = -1;
int repeat1 = -1;

//Reminder time medication 1:
int remhour2 = -1;
int remminute2 = -1;
int repeat2 = -1;

//Reminder time hydration 2:
int remhour3 = -1;
int remminute3 = -1;
int repeat3 = -1;

//Reminder time medication 2:
int remhour4 = -1;
int remminute4 = -1;
int repeat4 = -1;

//To store current time when user uses app (hours and minutes):
int hour_1 = -1;
int minute_1 = -1;

//To update current time:
int current_hour = 0;
int current_minute = 0;

//Temporary variables for if statements:
bool y = true; //Checking hour and minute data (current time)

bool temp = false; //Checking and activating reminder if, while loop (IF STILL CONNECTED)

int k = 0; //For runnning the while loop mentioned above

bool j = true; //Checking and activating reminder if, while loop (IF NOT CONNECTED)

int m = 0; //For runnning the while loop mentioned above

int reset = 1; //For resetting of the reminder

void setup() {

  //Begin serial:
  Serial.begin(9600);

  //Set motor pin to OUTPUT and initially OFF:
  pinMode(motor_pin, OUTPUT);
  digitalWrite(motor_pin, LOW);

  //Debugging code:
  if (!BLE.begin()) { //Begin initialization
    while (1);  
    
    Serial.println("Waiting for BLE"); //Wait until initialization complete
  }
 
  BLE.setLocalName("Watch"); //Set advertised local name
  BLE.setAdvertisedService(ledService); //Set advertised service UUID
  ledService.addCharacteristic(hourCharacteristic); //Add the hour characteristic to the service
  ledService.addCharacteristic(minCharacteristic); //Add the min characteristic to the service

  ledService.addCharacteristic(remindhour1Characteristic); //Add the remindhour1 characteristic to the service
  ledService.addCharacteristic(remindminute1Characteristic); //Add the remindminute1 characteristic to the service
  ledService.addCharacteristic(remindhour2Characteristic); //Add the remindhour2 characteristic to the service
  ledService.addCharacteristic(remindminute2Characteristic); //Add the remindminute2 characteristic to the service
  ledService.addCharacteristic(repeat1Characteristic); //Add the repeat1 characteristic to the service
  ledService.addCharacteristic(repeat2Characteristic); //Add the repeat2 characteristic to the service
  ledService.addCharacteristic(resetCharacteristic); //Add the reset characteristic to the service

  ledService.addCharacteristic(remindhour3Characteristic); //Add the remindhour3 characteristic to the service
  ledService.addCharacteristic(remindminute3Characteristic); //Add the remindminute3 characteristic to the service
  ledService.addCharacteristic(remindhour4Characteristic); //Add the remindhour4 characteristic to the service
  ledService.addCharacteristic(remindminute4Characteristic); //Add the remindminute4 characteristic to the service
  ledService.addCharacteristic(repeat3Characteristic); //Add the repeat3 characteristic to the service
  ledService.addCharacteristic(repeat4Characteristic); //Add the repeat4 characteristic to the serice

  BLE.addService(ledService); //Add service

  //Set the initial value for the characeristics:
  hourCharacteristic.writeValue(0);
  minCharacteristic.writeValue(0);

  remindhour1Characteristic.writeValue(0);
  remindminute1Characteristic.writeValue(0); 
  remindhour2Characteristic.writeValue(0);
  remindminute2Characteristic.writeValue(0);
  repeat1Characteristic.writeValue(0);
  repeat2Characteristic.writeValue(0);

  remindhour3Characteristic.writeValue(0);
  remindminute3Characteristic.writeValue(0); 
  remindhour4Characteristic.writeValue(0);
  remindminute4Characteristic.writeValue(0);
  repeat3Characteristic.writeValue(0);
  repeat4Characteristic.writeValue(0);

  resetCharacteristic.writeValue(0);

  BLE.advertise(); //Start advertising

  Serial.println("BLE LED Peripheral");
}
 
void loop() {
  BLEDevice central = BLE.central(); //Listen for BLE devices to connect:
 
  if (central) { // if a central is connected to peripheral:

    Serial.println("Connected to device");
                                                                
    while (central.connected()) { //While the central is still connected to peripheral

      //If current hour written then store it in hour_1:
      if (hourCharacteristic.written()) {
        hour_1 = hourCharacteristic.value();
        Serial.println("Hour:");
        Serial.println(hour_1);
      }

      //If current minute written then store it in minute_1:
      if (minCharacteristic.written()) {
        minute_1 = minCharacteristic.value();
        Serial.println("Minute:");
        Serial.println(minute_1);
      }

      //Setting current time/date, ONLY when hour and minute data received, only run once:
      if (y && (hour_1 != -1) && (minute_1 != -1)) {
        setTime(hour_1, minute_1, 0, 12, 5, 2012);

        t = now();

        y = false; //To ensure this doesn't run again

        Serial.println("Time set");
      }

      //If reminder time (hour1) written then store it in remhour1:
      if (remindhour1Characteristic.written()) {
        remhour1 = remindhour1Characteristic.value();
        Serial.println("REMINDER HOUR 1 (Hydration):");
        Serial.println(remhour1);
      }

      //If reminder time (minute1) written then store it in remminute1:
      if (remindminute1Characteristic.written()) {
        remminute1 = remindminute1Characteristic.value();
        Serial.println("REMINDER MINUTE 1 (Hydration):");
        Serial.println(remminute1);
      }

      //If reminder time (hour2) written then store it in remhour2:
      if (remindhour2Characteristic.written()) {
        remhour2 = remindhour2Characteristic.value();
        Serial.println("REMINDER HOUR 2 (Medication):");
        Serial.println(remhour2);
      }

      //If reminder time (minute2) written then store it in remminute2:
      if (remindminute2Characteristic.written()) {
        remminute2 = remindminute2Characteristic.value();
        Serial.println("REMINDER MINUTE 2 (Medication):");
        Serial.println(remminute2);
      }

      //If reminder time (hour3) written then store it in remhour3:
      if (remindhour3Characteristic.written()) {
        remhour3 = remindhour3Characteristic.value();
        Serial.println("REMINDER HOUR 3 (Hydration):");
        Serial.println(remhour3);
      }

      //If reminder time (minute3) written then store it in remminute3:
      if (remindminute3Characteristic.written()) {
        remminute3 = remindminute3Characteristic.value();
        Serial.println("REMINDER MINUTE 3 (Hydration):");
        Serial.println(remminute3);
      }

      //If reminder time (hour4) written then store it in remhour4:
      if (remindhour4Characteristic.written()) {
        remhour4 = remindhour4Characteristic.value();
        Serial.println("REMINDER HOUR 4 (Medication):");
        Serial.println(remhour4);
      }

      //If reminder time (minute4) written then store it in remminute4:
      if (remindminute4Characteristic.written()) {
        remminute4 = remindminute4Characteristic.value();
        Serial.println("REMINDER MINUTE 4 (Medication):");
        Serial.println(remminute4);
      }

      //If repeat for reminder 1 written store it in repeat1:
      if (repeat1Characteristic.written()) {
        repeat1 = repeat1Characteristic.value();
        Serial.println("REPEAT 1: HYD 1");
        Serial.println(repeat1);
      }

      //If repeat for reminder 2 written store it in repeat2:
      if (repeat2Characteristic.written()) {
        repeat2 = repeat2Characteristic.value();
        Serial.println("REPEAT 2: MED 1");
        Serial.println(repeat2);
      }

      //If repeat for reminder 3 written store it in repeat3:
      if (repeat3Characteristic.written()) {
        repeat3 = repeat3Characteristic.value();
        Serial.println("REPEAT 3: HYD 3");
        Serial.println(repeat3);
      }

      //If repeat for reminder 4 written store it in repeat4:
      if (repeat4Characteristic.written()) {
        repeat4 = repeat4Characteristic.value();
        Serial.println("REPEAT 4: MED 4");
        Serial.println(repeat4);
      }

      //If all data collected then only go into the loop ahead in the code:
      if ((repeat1 != -1) && (repeat2 != -1) && (repeat3 != -1) && (repeat4 != -1) && (remhour1 != -1) && (remhour2 != -1) && (remhour3 != -1) && (remhour4 != -1) && (remminute1 != -1) && (remminute2 != -1) && (remminute3 != -1) && (remminute4 != -1)) {
        temp = true;
        k = 0;
        j = true;
        m = 0;
      }

      //Activate reminder here ONLY IF hour, minute data received and set AND reminder data received:
      if (temp && (!y)) {
        
        Serial.println("Entered loop");


        //Keep looping until reminder is sounded, keep updating the current_hour and current_minute variables
        //checking if these equal the reminder time:
        while (k == 0) {

          t = now();

          current_hour = hour(t);
          current_minute = minute(t);

          //If it is time for the reminder, activate vibration and LED's for HYDRATION 1:
          if ((current_hour == remhour1) && (current_minute == remminute1)) {
            
            if ((remminute1 + repeat1) >= 60) {

              //This was for testing ignore it:
              //for (int i = 0; i < 10; i++) {
                //Serial.println("TIME MET HYDRATION (reminder1)");
              //}

              remhour1 = remhour1 + 1; //Go to next hour
              remminute1 = (remminute1 + repeat1) - 60; //Go to upcoming minute

              //Turn on motor + LED's:
              digitalWrite(motor_pin, HIGH);
              analogWrite(blueled_pin, 100);

              //Do this for 5 seconds:
              delay(5000);

              //Then turn them off again:
              digitalWrite(motor_pin, LOW);
              analogWrite(blueled_pin, 0);

            } 
            else {
          
              //This was for testing ignore it:
              //for (int i = 0; i < 10; i++) {
                //Serial.println("TIME MET HYDRATION (reminder1)");
              //}

              remhour1 = remhour1 + 0; //Stay
              remminute1 = remminute1 + repeat1; //Go to upcoming minute

              //Turn on motor + LED's:
              digitalWrite(motor_pin, HIGH);
              analogWrite(blueled_pin, 100);

              //Do this for 5 seconds:
              delay(5000);

              //Then turn them off again:
              digitalWrite(motor_pin, LOW);
              analogWrite(blueled_pin, 0);
            }
          }

          //If it is time for the reminder, activate vibration and LED's for MEDICATION 1:
          if ((current_hour == remhour2) && (current_minute == remminute2)) {
            
            if ((remminute2 + repeat2) >= 60) {

              //This was for testing ignore it:
              //for (int i = 0; i < 10; i++) {
              //Serial.println("TIME MET MEDICATON (reminder2)");
              //}

              remhour2 = remhour2 + 1; //Go to next hour
              remminute2 = (remminute2 + repeat2) - 60; //Go to upcoming minute

              //Turn on motor + LED's:
              digitalWrite(motor_pin, HIGH);
              analogWrite(redled_pin, 100);

              //Do this for 5 seconds:
              delay(5000);

              //Then turn them off again:
              digitalWrite(motor_pin, LOW);
              analogWrite(redled_pin, 0);

            } 
            else {
          
              //This was for testing ignore it:
              //for (int i = 0; i < 10; i++) {
                //Serial.println("TIME MET MEDICATON (reminder2)");
              //}

              remhour2 = remhour2 + 0; //Stay
              remminute2 = remminute2 + repeat2; //Go to upcoming minute

              //Turn on motor + LED's:
              digitalWrite(motor_pin, HIGH);
              analogWrite(redled_pin, 100);

              //Do this for 5 seconds:
              delay(5000);

              //Then turn them off again:
              digitalWrite(motor_pin, LOW);
              analogWrite(redled_pin, 0);
            }
          }

          //If it is time for the reminder, activate vibration and LED's for HYDRATION 2:
          if ((current_hour == remhour3) && (current_minute == remminute3)) {
            
            if ((remminute3 + repeat3) >= 60) {

              //This was for testing ignore it:
              //for (int i = 0; i < 10; i++) {
                //Serial.println("TIME MET HYDRATION (reminder1)");
              //}

              remhour3 = remhour3 + 1; //Go to next hour
              remminute3 = (remminute3 + repeat3) - 60; //Go to upcoming minute

              //Turn on motor + LED's:
              digitalWrite(motor_pin, HIGH);
              analogWrite(blueled_pin, 100);

              //Do this for 5 seconds:
              delay(5000);

              //Then turn them off again:
              digitalWrite(motor_pin, LOW);
              analogWrite(blueled_pin, 0);

            } 
            else {
          
              //This was for testing ignore it:
              //for (int i = 0; i < 10; i++) {
                //Serial.println("TIME MET HYDRATION (reminder1)");
              //}

              remhour3 = remhour3 + 0; //Stay
              remminute3 = remminute3 + repeat3; //Go to upcoming minute

              //Turn on motor + LED's:
              digitalWrite(motor_pin, HIGH);
              analogWrite(blueled_pin, 100);

              //Do this for 5 seconds:
              delay(5000);

              //Then turn them off again:
              digitalWrite(motor_pin, LOW);
              analogWrite(blueled_pin, 0);
            }
          }

          //If it is time for the reminder, activate vibration and LED's for MEDICATION 2:
          if ((current_hour == remhour4) && (current_minute == remminute4)) {
            
            if ((remminute4 + repeat4) >= 60) {

              //This was for testing ignore it:
              //for (int i = 0; i < 10; i++) {
              //Serial.println("TIME MET MEDICATON (reminder2)");
              //}

              remhour4 = remhour4 + 1; //Go to next hour
              remminute4 = (remminute4 + repeat4) - 60; //Go to upcoming minute

              //Turn on motor + LED's:
              digitalWrite(motor_pin, HIGH);
              analogWrite(redled_pin, 100);

              //Do this for 5 seconds:
              delay(5000);

              //Then turn them off again:
              digitalWrite(motor_pin, LOW);
              analogWrite(redled_pin, 0);

            } 
            else {
          
              //This was for testing ignore it:
              //for (int i = 0; i < 10; i++) {
                //Serial.println("TIME MET MEDICATON (reminder2)");
              //}

              remhour4 = remhour4 + 0; //Stay
              remminute4 = remminute4 + repeat4; //Go to upcoming minute

              //Turn on motor + LED's:
              digitalWrite(motor_pin, HIGH);
              analogWrite(redled_pin, 100);

              //Do this for 5 seconds:
              delay(5000);

              //Then turn them off again:
              digitalWrite(motor_pin, LOW);
              analogWrite(redled_pin, 0);
            }
          }

          BLEDevice central = BLE.central(); //Listen for BLE devices to connect:

          if (central) {
            
            //If resetted --> terminate the loop store new variables and then continue the loop
            if (resetCharacteristic.written()) {
              if (reset == resetCharacteristic.value()) {
                
                //Reset all variables to original so the code restarts:
                temp = false;
                k = 1;

                remhour1 = -1;
                remminute1 = -1;
                repeat1 = -1;

                remhour2 = -1;
                remminute2 = -1;
                repeat2 = -1;

                remhour3 = -1;
                remminute3 = -1;
                repeat3 = -1;

                remhour4 = -1;
                remminute4 = -1;
                repeat4 = -1;

                hour_1 = -1;
                minute_1 = -1;

                current_hour = 0;
                current_minute = 0;

                y = true;

                Serial.println("resetted");
              }
            }
          }
        }
      }
    } 
  }
}                                                                                                                    