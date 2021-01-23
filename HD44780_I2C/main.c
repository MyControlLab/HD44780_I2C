/*
 * Project			: HD44780_TWI
 * Created			: 10/01/2021 14:01:59
 * Author			: Tonnie te Braake
*/ 

#include <avr/io.h>
#include <avr/delay.h>
#include <stdlib.h>	


int main(void)


{
    int counter = 0;
	char buf[5];


    DDRC &= (1<<DDC4) | (1<<DDC5);												//Make SDA and SCL inputs 
   	PORTC &= ~(1<<PORTC4) | ~(1<<PORTC5);										//Disable internal pullups on SDA and SCL

    HD44780_TWI_init();

	HD44780_TWI_lcd_clear();
	HD44780_TWI_set_cursor(0, 0);
	HD44780_TWI_send_string("A=");
	
	//HD44780_TWI_cursor_blinking_onoff(ON);
	
    while (1) 
    {
	   for (counter=0;counter<20;counter++)
	   {
	      HD44780_TWI_set_cursor(0, 3);
		  HD44780_TWI_send_string("  ");
		  HD44780_TWI_set_cursor(0, 3);
		  itoa(counter,buf,10); 
	      HD44780_TWI_send_string(buf);
		  _delay_ms(500);
	   }
    }
}

