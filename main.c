#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define LED_RED 7
#define LED_GREEN 21
#define KEY_UP 3
#define KEY_DOWN 4

#define NOTE_C_LOW 956
#define NOTE_C 1912
#define PIR_D 2 // 27
#define BUZZER 15
#define BUFF_SIZE 1024

char pir_flag = 0;

void PIR_interrupt() 
{
	pir_flag = 1 ;
}

int main(void){
	char bufff[256];
	char buff[256];
	int i,ret_u=0, ret_d = 0;
  	int count = 0;
	
	if(wiringPiSetup()==-1)  
    	return -1;
	pinMode(PIR_D,INPUT);
	pinMode(BUZZER,OUTPUT);
	pinMode(LED_GREEN,OUTPUT);
	pinMode(LED_RED,OUTPUT);
	pinMode(BUZZER,OUTPUT);
	pinMode(KEY_UP,INPUT);
	pinMode(KEY_DOWN, INPUT);
	digitalWrite(LED_RED,0);
	digitalWrite(LED_GREEN,0);
	
	wiringPiISR(PIR_D, INT_EDGE_RISING, &PIR_interrupt);

	
    	while(1){	
		ret_u = digitalRead(KEY_UP);
		if(ret_u==0)
		{
			while(1)
			{	
					
				ret_d = digitalRead(KEY_DOWN);
				if(ret_d == 0){break;}
				printf("Alert mode activated!!\n");
				if(pir_flag == 1) 
				{
					printf("Invader Detected(BUZZER,RED LED ON) !! \n");
					//tone(BUZZER, NOTE_C, 400);
					count++;
					digitalWrite(BUZZER,1);
					digitalWrite(LED_RED,1);
					digitalWrite(LED_GREEN,0);
					sprintf(buff, "sudo fswebcam -r 1280x720 invader%d.jpg", count);
					system(buff);
					pir_flag = 0;
					sleep(0.5);
					sprintf(bufff, "sudo /home/pi/Downloads/pir/ex %d", count);
					system(bufff);
   	    			}
				else 
				{
					printf("Invader Not detected(BUZZER OFF, GREEN LED ON) !! \n");
					digitalWrite(BUZZER,0);
					digitalWrite(LED_RED,0);
					digitalWrite(LED_GREEN,1);
					sleep(0.5);
	    			}
				
			}
		}
		else {
			printf("Alert mode not yet activated!!\n");
			//tone(BUZZER, NOTE_C_LOW, 100);
			digitalWrite(BUZZER,0);
			digitalWrite(LED_RED,0);
			digitalWrite(LED_GREEN,1);
			pir_flag = 0;
		}
		usleep(500000);
	}
	return 0;
}
