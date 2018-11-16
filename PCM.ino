//#define acc1 5  //Potentiometer Wiper into pin 5
//#define acc2 6  //Potentiometer Wiper into pin 6
//#define brake 2 //Potentiometer Wiper into pin 2

  int Fault_LED = GREEN_LED; //This LED will be on if there is a fault
  int Good_LED = RED_LED;  // This LED will be on if there is no fault
  int acc1 = A4;
  int acc2 = A5;
  int brake = A0;
  int BSE_flag = 0;     //Flag to signify an existing BSE fault
  int throttle1 = 0;
  int throttle2 = 0;
  int brakeAngle = 0;
  int absThrottle = 0; //Absolute value of difference of throttle values
  
void setup() {
  pinMode(Fault_LED, OUTPUT); //Sets up pin for LED fault
  pinMode(Good_LED, OUTPUT); //Sets up pin for LED fault
  Serial.begin(9600); //Set baud rate to be 9600 bits per second which is standard
                      // for UART communication which we will use for preliminary
                      //testing because it is easy to debug
  analogReadResolution(8);        //Set ADC values to 8 bits intead of default 10 bits
  analogReference(DEFAULT);       //Sets analog input reference value for 0-5V
}

void loop() {                     //Start of main loop for car functions
  throttle1 = analogRead(acc1);   //Reads analog signal from Acc1 potentiometer
  throttle2 = analogRead(acc2);   //Reads analog signal from Acc2 potentiometer
  brakeAngle = analogRead(brake); //Reads analog signal from  potentiometer
  
  Serial.println(throttle1);
  delay(100);
  Serial.println(throttle2);
  if(APPS_Fault(throttle1,throttle2) || BSE_Fault(brakeAngle, throttle1, throttle2)){
      //If code goes within this statement, it means that either APPS or BSE, or both cases
      //faulted and we will set the throttle output value equal to 0 as well as turn 
      //on the fault LED
      digitalWrite(Fault_LED, HIGH);
      digitalWrite(Good_LED, LOW);
      
  }
  else{
      //If code goes within this statement, it means that no faults occurred on the car.
      //This will turn the LED off if it was already on, as well as send the throttle 
      //signal to the Central Control Module(CCM) via UART or CANBus
      //UART and CANBus have yet to be implemented in the Energia IDE/language
      digitalWrite(Good_LED, HIGH);
      digitalWrite(Fault_LED, LOW);
  }
}

int APPS_Fault(int x, int y){
  y = y*1.2945;
  absThrottle = abs(x-y);
  if((absThrottle >25) || (x <10) || (y <10)){
    return 1;
  }
  else{
    return 0;
  }
}
int BSE_Fault(int z,int x, int y){
  int BSE_flag;
    y = y*1.2945;
  if((BSE_flag) && (x > 13 || y > 13)) { //13 is 5% of 255
    return 1;
  }
  if((x > 64 || y > 64) && z > 0x88){ //64 is 25% of 255      0x88 is 53% of 5V where 2.67V is "hard press" of brakes
    BSE_flag = 1;
    return 1;
  }
  else if ((BSE_flag) && (x < 13 && y < 13)){ //13 is 5% of 255  if flag is already set, flag will clear when APPS values are less than 5%
    BSE_flag = 0;
    return 0;
  }
  return 0;
}
