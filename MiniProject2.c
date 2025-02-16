/*
 * MiniProject2.c
 *
 *  Created on: Feb 10, 2025
 *      Author: Hatem
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DISPLAY_DELAY 2


unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;
unsigned char pause_flag = 0;
unsigned char count_mode = 0;// 0 for count-up and 1 for count-down
unsigned char count_change_flag = 1; // 0 for pressed and 1 for not pressed
unsigned char button_flag = 1;//flag used for all the increment and decrement buttons

void displaySegment(unsigned char segment, unsigned char value) {
    PORTA = segment;
    PORTC = value;
    _delay_ms(DISPLAY_DELAY);
}

void Timer_1_CTC_Init(void){
	TCCR1A = (1<<FOC1A);//enable this bit when not using pwm modes
	TCCR1B = (1<<WGM12)| (1<<CS12) | (1<<CS10);//set CTC mode and prescalar to 1024
	TIMSK = (1<<OCIE1A); // Enable Timer1 Compare A Interrupt
	TCNT1 = 0;//set initial value
	OCR1A = 15625;//number of timer counts in one second with the defined prescalar
	SREG |= (1<<7);//enable I-bit
}

void INT0_Init(void){
	DDRD  &= (~(1<<PD2));//configure PD2 as input pin
	PORTD |= (1<<PD2);//internal pull-up for the reset button
	MCUCR |= (1<<ISC01);//falling edge of INT0 triggers an interrupt response
	GICR |= (1<<INT0);//INT0 enable
}

ISR(INT0_vect){
	seconds = 0;
	minutes = 0;
	hours = 0;//resetting timer
	PORTD &= ~(1<<PD0); // Turn off the buzzer
}

void INT1_Init(void){
	DDRD  &= (~(1<<PD3));//configure PD3 as input pin
	MCUCR |= (1<<ISC11)|(1<<ISC10);//rising edge of INT1 triggers an interrupt response
	GICR |= (1<<INT1);//INT1 enable
}

ISR(INT1_vect){
	TCCR1B = 0;// Stop timer by clearing clock bits
	pause_flag = 1;
}

void INT2_Init(void){
	GICR |= (1<<INT2);//INT2 enable
	MCUCSR &= ~(1<<ISC2); // Ensure INT2 triggers on a rising edge
}

ISR(INT2_vect){
	 TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10); // Resume timer by setting clock bits
	 pause_flag = 0;
}

ISR(TIMER1_COMPA_vect){
	if(count_mode){//count-down mode
		if(hours == 0 && minutes == 0 && seconds == 0) {
			PORTD |= (1<<PD0);//sound buzzer
		}else{
			if(seconds == 0){
				if(minutes > 0){
					minutes--;
					seconds = 59;
				}else if(hours > 0){
					hours--;
					minutes = 59;
					seconds = 59;
				}
			}else{
				seconds--;
			}
		}
	}else{//count-up mode
		seconds++;
		if(seconds == 60){
			seconds = 0;
			minutes++;
		}
		if(minutes == 60){
			minutes = 0;
			hours++;
		}
		if(hours == 99){
			hours = 0;
		}
	}
}

int main(void){
	Timer_1_CTC_Init();
	INT0_Init();
	INT1_Init();
	INT2_Init();

	DDRC |= 0x0F;
	PORTC &= 0xF0;//sending over time as binary to decoder

	DDRA |= 0x3F;
	PORTA &= 0xC0;// rolling through the 7-segments

	DDRD |= (1<<PD4) | (1<<PD5) | (1<<PD0);//count mode LEDs and buzzer
	PORTD |= (1<<PD5);//turn on count-down mode led initially

	DDRB = 0;//increment and decrement buttons, resume button, count-change button
	PORTB = 0xFF;//enable all their internal pull-ups


	while(1){
		if(!(PINB & (1<<PB7))){//count change operation
		    if(count_change_flag && pause_flag){//check if timer is paused
		        count_mode ^= 1; // Toggle count mode
		        if(count_mode){
		            PORTD |= (1<<PD4);// turn on count-down led
		            PORTD &= ~(1<<PD5);//turn off count-up led
		        }else{
		            PORTD |= (1<<PD5);// turn on count-up led
		            PORTD &= ~(1<<PD4);//turn off count-down led
		        }
		    }
		    count_change_flag = 0;
		}else{
			count_change_flag = 1;
		}

		//Hours Adjustment
		if(!(PINB & (1<<PB1)) && button_flag){//Increment Hours
		    if(hours < 99){
		    	hours++;
				button_flag = 0;
		    }
		}
		if(!(PINB & (1<<PB0)) && button_flag){//Decrement Hours
		    if (hours > 0){
		    	hours--;
				button_flag = 0;
		    }
		}
		//Minutes Adjustment
		if(!(PINB & (1<<PB4)) && button_flag){//Increment Minutes
		    if (minutes < 59){
		    	minutes++;
		    	button_flag = 0;
		    }
		}
		if(!(PINB & (1<<PB3)) && button_flag){//Decrement Minutes
		    if (minutes > 0){
		    	minutes--;
		    	button_flag = 0;
		    }
		}
		//Seconds Adjustment
		if(!(PINB & (1<<PB6)) && button_flag){//Increment Seconds
		    if (seconds < 59){
		    	seconds++;
		    	button_flag = 0;
		    }
		}

		if(!(PINB & (1<<PB5)) && button_flag){//Decrement Seconds
		    if (seconds > 0){
		    	seconds--;
		    	button_flag = 0;
		    }
		}
		// Reset button flag to allow the next press
		if(PINB == 0xFF){ // Check if no button is pressed
		    button_flag = 1;
		}

		//displaying time on the 7-segments
		displaySegment(0xE0, seconds % 10);
		displaySegment(0xD0, seconds / 10);
		displaySegment(0xC8, minutes % 10);
		displaySegment(0xC4, minutes / 10);
		displaySegment(0xC2, hours % 10);
		displaySegment(0xC1, hours / 10);
	}
}
