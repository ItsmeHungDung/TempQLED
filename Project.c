/*
Program:Project
Author: Hung Dung,Le
Date: March 31th,2022
Purpose:The program will run for 25 seconds to make one LED to flash and another LED to turn on when a temperature sensor 
reads a value above a threshold of 26 oC. When the input is below the threshold, the blue LED output will 
flash on and off at a rate of 4Hz and the red LED will be off. When the threshold is exceeded, the blue LED 
output will be off and the red LED will be on. The temperature must be read and displayed to the user every 
0.25 s, regardless of its value.
*/

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "C:\Program Files (x86)\LabJack\Drivers\LabJackUD.h"

int main(void)
{	
	//Set up the labjack and some proper value.
	LJ_ERROR lj_cue;
	LJ_HANDLE lj_handle = 0;
	double dblValueAIN1 = 0;
	double dblValueDAC1 = 0;
	double dblValueFIO4 = 0;
	int counter = 0;
	int dutycycle = 0;

	//Open the lab jack and reset all the pin.
	lj_cue = OpenLabJack(LJ_dtU3, LJ_ctUSB, "1", 1, &lj_handle);
	lj_cue = ePut(lj_handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);
	//Set-up PWM signal for the blueLED to blink.
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_COUNTER_PIN_OFFSET, 4, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_BASE, LJ_tc12MHZ_DIV, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_DIVISOR, 46, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chNUMBER_TIMERS_ENABLED, 1, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_TIMER_MODE, 0 , LJ_tmPWM16, 0, 0);
	lj_cue = Go();
	//Create a loop that will run for 25sec
	while (counter < 100)
	{	
		//Get the voltage value from the temperature sensor.
		lj_cue = AddRequest(lj_handle, LJ_ioGET_AIN, 1 , 0, 0, 0);
		lj_cue = Go();
		lj_cue = GetResult(lj_handle, LJ_ioGET_AIN, 1, &dblValueAIN1);
		printf("\n\t\tThe value of the temperature sensor: %.2lfV", dblValueAIN1);
		//if temperature go below 26 degrees, the blueLED blink and the red will turn off.
		//if temperature go above 26 degrees, the blueLEd won't blink and the red will turn on.
		if (dblValueAIN1 < 0.52)
		{	
	
			lj_cue = AddRequest(lj_handle, LJ_ioPUT_DAC, 1, 0, 0, 0);
			lj_cue = AddRequest(lj_handle, LJ_ioPUT_TIMER_VALUE, 0, 32768, 0, 0);
			lj_cue = Go();
		}
		else 
		{

			lj_cue = AddRequest(lj_handle, LJ_ioPUT_DAC, 1, 5, 0, 0);
			lj_cue = AddRequest(lj_handle, LJ_ioPUT_TIMER_VALUE, 0, 65535, 0, 0);
			lj_cue = Go();
		}
		counter++;
		Sleep(250);
		
	}
	
	//Reset all the pin
	lj_cue = ePut(lj_handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);
	//Turn off all the LED when the program finished.
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_DAC, 1, 0, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_DIGITAL_BIT, 4, 0, 0, 0);
	lj_cue = Go();
	
	system("pause");

	
	Close();
	return 0;

}