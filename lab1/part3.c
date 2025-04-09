#include "globals.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void ledInit();
void turnNledOn(u08 ledNum);
u08 variableDelay(u16 delayMs);

int main(){
	init();
  init_lcd();
	ledInit();

	u08 curLED = 1;
	u08 curBtn;
	int8_t ledDir = 1;
	u08 gameEnd = 0;

	u16 msDelay = 500;

	char outString [] = "Delay";

	while(gameEnd == 0){
		turnNledOn(curLED);

		curBtn |= get_btn();
   	curBtn |= variableDelay(msDelay);
		

		if(curBtn){
			// button is pressed
			if(curLED == 0 || curLED == 4){
				msDelay -= 25;
			}
			// pressed at the wrong time
			else{
				gameEnd = 1;
				print_string(outString);
				lcd_cursor(0, 1);
				print_num(msDelay);
				break;
			}
		}
		// button is not pressed at end LEDs
		else if (curLED == 0 || curLED == 4){
				gameEnd = 1;
				print_string(outString);
				lcd_cursor(0, 1);
				print_num(msDelay);
				break;
		}


		if(curLED == 0){
			ledDir = 1;
		}
		if(curLED == 4){
			ledDir = -1;
		}

		curLED += ledDir;
		curBtn = 0;

	}

}

void ledInit(){
	/*	 
		pin 8:  LED 0
		pin 9: 	LED 1
		pin 10: LED 2
		pin 11: LED 3
		pin 12: LED 4
	*/
	
	digital_dir(8, 1);
	digital_dir(9, 1);
	digital_dir(10, 1);
	digital_dir(11, 1);
	digital_dir(12, 1);

}

void turnNledOn(u08 ledNum){
	int ledI = 8;

	// turns off all LEDs
	for(ledI = 8; ledI < 13; ledI++){ digital_out(ledI, 0); }

	digital_out((ledNum + 8), 1);

}

u08 variableDelay(u16 delayMs){
	u16 i = 0;
 	u08	btnVal = 0;

	for(i = 0; i < delayMs; i++){
		_delay_ms(1);
		btnVal |= get_btn();
	}
	return btnVal;
}
