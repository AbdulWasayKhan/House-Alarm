#include <stdio.h>
#include <wiringPi.h>
#include <curl/curl.h>
#include "ifttt.h"
#include <time.h>

#define ALARM_OFF 1 /*Constant*/
#define ALARM_ARMING 2
#define ALARM_ARMED 3
#define ALARM_TRIGGERED 4
#define ALARM_SOUNDING 5

int alarm_Off(int currentState); /*We declared the prototypes of methods.*/
int alarm_Arming(int currentState);
int alarm_Armed(int currentState);
int alarm_Triggered(int currentState);
int alarm_Sounding(int currentState);


int main(int argc, char *argv[])
 {
   int currentState;
   wiringPiSetup();
   pullUpDnControl(3, PUD_UP);  /*For button*/
   pinMode(3,INPUT);            /*Button input*/ 
   pinMode(0,INPUT); 		/*motion detecter middle*/
   pinMode(1,OUTPUT); 		/*Led 1: Blue output*/
   pinMode(2,OUTPUT); 		/*Led 2: Red output*/
   pinMode(4,OUTPUT); 		/*Buzzer output*/
   currentState = ALARM_OFF;
   
   currentState = alarm_Off(currentState); /**/
   currentState = alarm_Arming(currentState);/**/
   currentState = alarm_Armed(currentState);/**/
   
   if(currentState == ALARM_OFF)
 {
    currentState = alarm_Off(currentState);
 }
  else if(currentState == ALARM_TRIGGERED)
 {
   /* currentState = alarm_Triggered(currentState); */
   printf("alarm triggered");
 }
 return 0;

 }

int alarm_Off(int currentState)
  {
    if(currentState == ALARM_OFF)
      {
       printf("The system is off(disabled), Case 1:");
      while(digitalRead(3) == 1) /*While button is not pressed*/
    	{
	digitalWrite(1, HIGH);	/*Red Led 1 High*/
	digitalWrite(2, LOW);	/*Blue Led 2 Low*/  
	digitalWrite(4, LOW);	/*Buzzer to low*/
    	}
	if(digitalRead(3) == 0)/*Press the button*/
	{
	   printf("alarm arming");
	   return ALARM_ARMING; /*Returning the constant alarm-arming*/
	}
	
     }
return 0;
}

int alarm_Arming(int currentState)
 {
    int i;
    if(currentState == ALARM_ARMING)
	{
	    printf("Case 2:");
	    for(i = 0; i<=10; i++){
		digitalWrite(1, LOW); delay(500); /*Led 1 to low*/
		digitalWrite(1, HIGH); delay(500); /*Led 1 to high*/
		}
		digitalWrite(1, LOW);	/*Turn led 1 to off*/
		digitalWrite(2, HIGH);	/*Turn led 2 to on*/
	    return ALARM_ARMED;
	}
	return 0;
 }
int alarm_Armed(int currentState)
 {
      if(currentState == ALARM_ARMED)
	{
	   printf("alarm armed");
	   while(1){/*ALways true*/
	   digitalWrite(1, LOW);  /*First led turned off*/
	   digitalWrite(2, HIGH); /*Second led turned on*/
	   digitalWrite(4, LOW); /*Buzzer is set to low*/

	   if(digitalRead(3) == 0) /*Button is pressed*/
		 {
			printf("Alarm turned off");
			return ALARM_OFF;
		 }
		 if(digitalRead(0) == 0) /*Motion detector senses motion*/
		  {
			printf("alarm triggered");
			return ALARM_TRIGGERED; /*returned the constant*/
		 	 }
		}
	}
	return 0;
 }


int alarm_Triggered(int currentState)
 {
	int i;
	   if(currentState == ALARM_TRIGGERED)
		{
	 	   printf("alarm triggered");
		   for(i = 0; i<5 && digitalRead(3) == 1; i++)/*Button is not pressed and waiting for the intruder.*/
		   {
			digitalWrite(4, LOW); /*Buzzer to low*/
			digitalWrite(1, HIGH); delay(500); /*Turn the led on*/
			digitalWrite(1, LOW); delay(500);
			digitalWrite(2, HIGH); delay(500); /*Turn the led on*/
			digitalWrite(2, LOW); delay(500);
		   }
		  if(i == 5)
		   {
			return ALARM_SOUNDING; /*Alarm is going on and waitig for the intruder to press button*/
		   }
		else
			{
				return ALARM_OFF; /*IF  i != 5 then turn the alarm off because intruder pressed the button*/
			}
		}
	return 0;
 }


int alarm_Sounding(int currentState)
 {
	int i;
	printf("alarm sounding");
	ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/cEsyCCLR4jzCs8_UtE3zy3", "Lab05", "House-Alarm", "detected");
	if(currentState == ALARM_SOUNDING)
	{
           while(1)
	   {
           digitalWrite(1, HIGH); delay(2000);  /*First led turned off*/
	   digitalWrite(1, LOW);  delay(2000);
           digitalWrite(2, HIGH); delay(2000); /*Second led turned on*/
	   digitalWrite(2, LOW); delay(2000);
           digitalWrite(4, HIGH); /*Buzzer is set to low*/
	   printf("alarm");
	   }
	}
	   
/*Button is pressed*/
           if(digitalRead(3) == 0)
                 {
                        printf("Alarm turned off");
                        return ALARM_OFF;
                 }
	return 0;
}
