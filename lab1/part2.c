/*
Names: Joshua Livergood and Danny Moreno
Lab1, part 2
Prints out names scrolling accross the LCD
*/


#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TIME_DELAY 250
#define ROW 0
#define COL_NUM 8

int main(){
	init();
	init_lcd();
	// LCD 

	char partner[][10] = {"Joshua", "Danny"};
	u08 btnVal;
	u08 partnerVal = 0;
	//u08 colVal = 0;
	u08 curLetter = 0;
	u08 i;

	// runs continuously
	while(1){
		// two loops
		// one loop for columns
		for(i = COL_NUM-1; i > 0; i--){
			clear_screen();
			lcd_cursor(i, ROW);
		  print_string(partner[partnerVal]);
			_delay_ms(TIME_DELAY);
		
			// checks for button
			btnVal = get_btn();
			if(btnVal){ 
				partnerVal ^= 1;
			}
		}
		
		lcd_cursor(0, ROW);
		// one loop for letters
		while(partner[partnerVal][curLetter] != '\0'){
			clear_screen();
			print_string(&(partner[partnerVal][curLetter]));
			_delay_ms(TIME_DELAY);
			curLetter++;

			// checks for button
			btnVal = get_btn();
			if(btnVal){ 
				partnerVal ^= 1;
			}
		}

		curLetter = 0;

		btnVal = get_btn();

		if(btnVal){ 
			partnerVal ^= 1; 
			//init_scrolling(partner[partnerVal]);
			//toggle_scrolling();
		}

	}

	return 0;
}
