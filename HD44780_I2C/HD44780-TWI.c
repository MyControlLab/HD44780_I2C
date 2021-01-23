/*
 * HD44780_TWI.c
 *
 * Created: 10/01/2021 14:03:21
 * Author: Tonnie te Braake
 *
 *=============================================================================================================
 * - PCF8574 Remote 8-bit I/O expander for I2C-bus with interrupt
 * - 100 kHz I2C-bus interface (Standard-mode I2C-bus)
  *===========================================================================================================================
 *=		LCD PIN			ABBR		HD47480 DESC					PCF8574		ATMEGA328	UTILLS
 *===========================================================================================================================
 *=			-			-			-								SDA			PC4			I2C Data
 *=			-			-			-								SCL			PC5			I2C Clock
 *=			1			VSS			GND								-						GND +5V power supply
 *=			2			VDD			+5V								-						+5V power supply
 *=			3			VO			Contrast Adjustment				-						Via potentiometer to +5V
 *=			4			RS			H/L Register select				4 (P0)
 *=			5			R/W			H/L Read/Write					5 (P1)
 *=			6			E			H/L Enable signal				6 (P2)
 *=			-			-			Back light						7 (P3)
 *=			7			DB0			H/L Data bus line				-
 *=			8			DB1			H/L Data bus line				-
 *=			9			DB2			H/L Data bus line				-
 *=			10			DB3			H/L Data bus line				-
 *=			11			DB4			H/L Data bus line				9  (P4)
 *=			12			DB5			H/L Data bus line				10 (P5)
 *=			13			DB6			H/L Data bus line				11 (P6)
 *=			14			DB7			H/L Data bus line				12 (P7)
 *=			15			A			+4.2V for LED					-							
 *=			16			K			Power supply for BKL(0V)		-							
 *===========================================================================================================================
  */ 
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "HD44780-TWI.h"
#include "TWI_Master.h"

unsigned char messageBuf[TWI_BUFFER_SIZE];	
unsigned char _displayCMD=0;


/// HD44780_TWI_init
///
/// This function initializes the HD44780 lcd driver
void HD44780_TWI_init()
{
	TWI_Master_Initialise(HD44780_TWI_BUS_FREQ);
	sei();

	//Power up delay
	_delay_ms(50);

	//Initial startup sequence
	HD44780_TWI_send_cmd(HD44780_TWI_START_INIT);
	_delay_ms(6);
	HD44780_TWI_send_cmd(HD44780_TWI_START_INIT);
	_delay_ms(2);
	HD44780_TWI_send_cmd(HD44780_TWI_START_INIT);
	_delay_ms(11);

	//4bit mode
	HD44780_TWI_send_cmd(HD44780_TWI_FUNCTION_SET | HD44780_TWI_4_BITMODE);

	//Function set
	HD44780_TWI_send_cmd(HD44780_TWI_FUNCTION_SET | HD44780_TWI_4_BITMODE | HD44780_TWI_2_LINE | HD44780_TWI_5x8DOTS);
	_delay_ms(2);  ;

	//Display ON/OFF control
	_displayCMD = HD44780_TWI_DISPLAY_CONTROL | HD44780_TWI_DISPLAY_OFF | HD44780_TWI_CURSOR_OFF | HD44780_TWI_BLINK_OFF;
	HD44780_TWI_send_cmd(_displayCMD);
	_delay_ms(2);

	//Clear display
	HD44780_TWI_send_cmd(HD44780_TWI_CLEAR_LCD);
	_delay_ms(4);

	//Entry mode set
	HD44780_TWI_send_cmd(HD44780_TWI_ENTRY_MODE_SET | HD44780_TWI_INCREMENT | HD44780_TWI_ENTRY_SHIFT_DECREMENT);
	_delay_ms(2);

	//Display ON/OFF control
	_displayCMD = HD44780_TWI_DISPLAY_CONTROL | HD44780_TWI_DISPLAY_ON | HD44780_TWI_CURSOR_OFF | HD44780_TWI_BLINK_OFF;
	HD44780_TWI_send_cmd(_displayCMD);
}

/// HD44780_TWI_lcd_clear 
///
/// This functions clears the display
void HD44780_TWI_lcd_clear(void)
{
   HD44780_TWI_send_cmd(HD44780_TWI_CLEAR_LCD);
   _delay_ms(2);
}

/// HD44780_TWI_set_cursor
///
/// This function sets the DDRAM address based on the row (y) and column (x)
/// @param x The is this column selected
/// @param y This is the sow selected
/// @returns Error state (ERROR or SUCCES)
/// @note The max rows and columns are set in the header file via HD44780_TWI_MAX_COLLUMS and  HD44780_TWI_MAX_ROWS
void HD44780_TWI_set_cursor(int row, int col)
{
	switch(row)
	{
		case 0:
			HD44780_TWI_send_cmd(HD44780_TWI_SET_DRAM_ADDRESS | (HD44780_TWI_STARTADR_ROW_0 + col));
			break;
		case 1:
			HD44780_TWI_send_cmd(HD44780_TWI_SET_DRAM_ADDRESS | (HD44780_TWI_STARTADR_ROW_1 + col));
			break;
		case 2:
			HD44780_TWI_send_cmd(HD44780_TWI_SET_DRAM_ADDRESS | (HD44780_TWI_STARTADR_ROW_2 + col));
			break;
		case 3: 
		    HD44780_TWI_send_cmd(HD44780_TWI_SET_DRAM_ADDRESS | (HD44780_TWI_STARTADR_ROW_3 + col));
			break;
		default:
			HD44780_TWI_send_cmd(HD44780_TWI_SET_DRAM_ADDRESS | (HD44780_TWI_STARTADR_ROW_0 + col));
			break;
	}
	
}

/// HD44780_TWI_send_data 
///
/// This functions send the data to the LCD driver
/// @param cmd Command to send
void HD44780_TWI_send_data(char data)
{
	unsigned char data_HigNib, data_LowNib;
	
	data_HigNib = (data & 0xF0);
	data_LowNib = ((data<<4) & 0xF0);
	messageBuf[1] = data_HigNib | HD44780_TWI_BACKLIGHT | HD44780_TWI_RS | HD44780_TWI_E;		
	messageBuf[2] = (data_HigNib | HD44780_TWI_BACKLIGHT | HD44780_TWI_RS) & (~HD44780_TWI_E);
	messageBuf[3] = data_LowNib | HD44780_TWI_BACKLIGHT | HD44780_TWI_RS | HD44780_TWI_E;
	messageBuf[4] = (data_LowNib | HD44780_TWI_BACKLIGHT | HD44780_TWI_RS) & (~HD44780_TWI_E);
	
	messageBuf[0] = (HD44780_TWI_SLAVE_ADRES) | (TWI_WRITE<<TWI_RW_BIT);		
	TWI_Start_Transceiver_With_Data(messageBuf, 5);
}

/// HD44780_TWI_send_cmd 
///
/// This functions send the cmd to the LCD driver
/// @param cmd Command to send
void HD44780_TWI_send_cmd (char cmd)
{
	unsigned char data_HigNib, data_LowNib;
		
	data_HigNib = (cmd & 0xF0);
	data_LowNib = ((cmd<<4) & 0xF0);
	messageBuf[1] = data_HigNib | HD44780_TWI_BACKLIGHT | HD44780_TWI_E;
	messageBuf[2] = (data_HigNib | HD44780_TWI_BACKLIGHT) & (~HD44780_TWI_E);
	messageBuf[3] = data_LowNib | HD44780_TWI_BACKLIGHT | HD44780_TWI_E;
	messageBuf[4] = (data_LowNib | HD44780_TWI_BACKLIGHT) & (~HD44780_TWI_E);
	
	messageBuf[0] = (HD44780_TWI_SLAVE_ADRES) | (TWI_WRITE<<TWI_RW_BIT);		
	TWI_Start_Transceiver_With_Data(messageBuf, 5);
}



/// Writes a string to the LCD display
///
/// @param *str String to write to the display
void HD44780_TWI_send_string(char *str)
{
	while(*str)
	{
		HD44780_TWI_send_data(*str++);
	}

}
/// HD44780_TWI_display_onoff 
///
/// This functions switches the display ON/OFF
/// @param cmd state to send
void HD44780_TWI_display_onoff(State_t State)
{
   switch (State)
   {
    case ON :
		_displayCMD |= HD44780_TWI_DISPLAY_ON;
	break;
	case OFF:
		_displayCMD &= ~HD44780_TWI_DISPLAY_ON;
	break;
   }

   HD44780_TWI_send_cmd(HD44780_TWI_DISPLAY_CONTROL |_displayCMD );
   _delay_ms(4);
}
/// HD44780_TWI_cursor_onoff 
///
/// This functions switches the cursor ON/OFF
/// @param cmd state to send
void HD44780_TWI_cursor_onoff(State_t State)
{
   switch (State)
   {
    case ON :
		_displayCMD |= HD44780_TWI_CURSOR_ON;
	break;
	case OFF:
		_displayCMD &= ~HD44780_TWI_CURSOR_ON;
	break;
   }
   HD44780_TWI_send_cmd(HD44780_TWI_DISPLAY_CONTROL | _displayCMD);
   _delay_ms(4);
}
/// HD44780_TWI_cursor_blinking_onoff 
///
/// This functions switches the blinking of the cursor
/// @param cmd state to send
void HD44780_TWI_cursor_blinking_onoff(State_t State)
{
   switch (State)
   {
    case ON :
		_displayCMD |= HD44780_TWI_BLINK_ON;
	break;
	case OFF:
		_displayCMD &= ~HD44780_TWI_BLINK_ON;
	break;
   }
   HD44780_TWI_send_cmd(HD44780_TWI_DISPLAY_CONTROL | _displayCMD);
   _delay_ms(4);
}


