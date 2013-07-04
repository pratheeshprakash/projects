#include <Servo.h> 
Servo pan,tilt;  // create servo object to control a servo 
void setup()
{
  Serial.begin(9600);
  Serial.println("Starting");
  pan.attach(2);  // attaches the servos 
  tilt.attach(3); 
}


void loop()
{
  char a,p,t;
  if (Serial.available())
  {
   if(Serial.read()=='<')
    {
    while(Serial.available()==0);// wait for second byte to be typed
    while((a=Serial.read())!='>')
      {
       while(Serial.available()==0);// wait for second byte to be typed
       switch(a)
        {
          case 'P': p=Serial.read();
                    break;
          case 'T': t=Serial.read();
                    break;
         }
      }  
    pan.write(p);
    tilt.write(t);
    Serial.println("ACK");
   }
    
  }
}



