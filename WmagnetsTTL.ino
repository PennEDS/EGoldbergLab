/*
Arduino Hall Effect Sensor Project
Objective: To measure instant velocity
      Based on the poistion of the magnets. 
      There is a starting reference point in the wheel
      The Reset button in Arduino will start the cycle
      of Measurements. 
*/


 volatile byte revolutions;
 volatile byte pulse_flag; 
 
 
 unsigned long timeold;
 unsigned long timestamp;
 unsigned long pulse_duration; 
 
 unsigned int InitialPos;
 unsigned int InstVel;

 unsigned int oddMagnet;
 unsigned int evenMagnet;
 unsigned int Magnet; /* 0 = Odd Magnet (Left sensor,
                          1 =Even Magnet Right sensor */
 unsigned int rpm;

 void setup()
 { 
    pulse_duration = 0; 
    oddMagnet = 0; 
    evenMagnet =0; 

   Serial.begin(115200);
   attachInterrupt(0, magnet_detect2, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2)
   attachInterrupt(1, magnet_detect3, FALLING);//Initialize the intterrupt pin (Arduino digital pin 3)
   
   revolutions = 0;  
   rpm = 0;
   timeold = 0;
   timestamp = 0; 
   InstVel=0; 
   InitialPos =0; /* means the first magnet */
   pulse_flag =0; /*No pulse received yet */
   
   Serial.println("Waiting for Pulse..."); 
   while ( (pulse_duration = pulseIn(5, HIGH))==0) {} // do nothing if no pulse yet
   
   // First pulse received, start the proces! 
   Serial.println("Pulse received, start aquisition!..."); 
 }
 
 void loop()  
 {
  InitialPos =0; 
  Magnet =0; 
  
  pulse_duration = pulseIn(5, HIGH);
  if(pulse_duration!=0){   
     
     /* Pulse received, reset flag for the loop */ 
     pulse_flag =1; 
    
    // poll every 10ms to see how many revoltions happened 
    delay(100); 
 
    calculate_inst_vel(); 
    Serial.print("# of magnets after 100ms: "); 
    Serial.println(revolutions); 
    
  
    /* After 100ms reset revolutions 
    to start polling again */
    revolutions =0; 
 
    /* What direction? */
    /* If both flags are 1 then is clockwise
      if both flags are 0 then is counter clockwise
   
    if (oddMagnet && evenMagnet ) 
   
    /** old code **/
    /*  
   if (revolutions >= 20) { 
     rpm = 30*1000/(millis() - timeold)*half_revolutions;
     timeold = millis();
     revolutions = 0;
    // Serial.println(InstVel,DEC);
   }
   */
    } else if (pulse_flag) {  
    Serial.println("Pulse stopped, acquistion stopped...");
    pulse_flag =0; // reset the pulse flag to print this only once       
    }
}


 void magnet_detect2()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   revolutions++;
   oddMagnet=1;
 }
 
 void magnet_detect3()//This function is called whenever a magnet/interrupt is detected by the arduino
{
    revolutions++;
    evenMagnet=1;  
 }

 /* Calculate Instant velocity with no regard to the direction 
    *  1. At the wheel, set the reference sensor 1 and press reset. 
    *  Free the mouse to start the experiment. 
    *  2. Start polling sensor 1 and 2 to set the direction 
    * - Distance is fixed between sensors.
    * - 6 inch diameter will give us:  47.88 cms of circumference 
    * - at 16 holes, this is 2.99cm per sensor. 
    - Determine s = total distance effectively traversed. 
    * - Divide by 100ms and get inches/sec instantaneous speed. 
    * - Display metric and start the new instantaneous speed iteration  
     */

 void calculate_inst_vel()
 {
   double distance = 0; 
   double inst_vel = 0; 
   
    distance = 2.99 * revolutions;
    /* Debug  
      Serial.println(revolutions, 2); 
      Serial.println(distance, 2); 
    */
   
   /* at 100ms elapsed, inst vel in cm/sec  */ 
   
    inst_vel = distance*10; 
    Serial.print("Inst_vel = "); 
    Serial.print(inst_vel, 2);  
    Serial.println("cm/sec");  
  }

  
 // HW Test routines 
 /*
 void magnet_detect2()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   quarter_revolutions++;
   if(oddMagnet>8){
        oddMagnet=0; 
   }
   //Serial.println("detect2");
   Serial.println("odd");
   Serial.println(oddMagnet,DEC);
   oddMagnet++;
 }
 
 void magnet_detect3()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
    quarter_revolutions++;
    if(evenMagnet>8){
        evenMagnet=0;
    }
  //   Serial.println("detect3");
  Serial.println("even");
  Serial.println(evenMagnet, DEC);
  evenMagnet++;  
 }

 */
