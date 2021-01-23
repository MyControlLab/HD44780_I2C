/*
 * HD44780_TWI.h
 *
 * Created: 10/01/2021 14:03:36
 * Author: Tonnie te Braake
 */ 


#ifndef HD44780-TWI_H_
#define HD44780-TWI_H_

 
#define HD44780_TWI_SLAVE_ADRES  			0x4E
#define HD44780_TWI_BUS_FREQ				100000

#define HD44780_TWI_RS						1
#define HD44780_TWI_RW						2
#define HD44780_TWI_E						4

#define HD44780_TWI_MAX_ROWS				4
#define HD44780_TWI_MAX_COLLUMS				20

#define HD44780_TWI_STARTADR_ROW_0			0x00
#define HD44780_TWI_ENDADR_ROW_0			HD44780_TWI_MAX_COLLUMS
#define HD44780_TWI_STARTADR_ROW_1			0x40
#define HD44780_TWI_ENDADR_ROW_1			HD44780_TWI_MAX_COLLUMS
#define HD44780_TWI_STARTADR_ROW_2			0x14
#define HD44780_TWI_ENDADR_ROW_2			HD44780_TWI_MAX_COLLUMS
#define HD44780_TWI_STARTADR_ROW_3			0x54
#define HD44780_TWI_ENDADR_ROW_3			HD44780_TWI_MAX_COLLUMS


//HD44780 commands
#define HD44780_TWI_START_INIT				0x30

#define HD44780_TWI_CLEAR_LCD				0x01
#define HD44780_TWI_RETURN_HOME				0x02
#define HD44780_TWI_ENTRY_MODE_SET			0x04
#define HD44780_TWI_DISPLAY_CONTROL			0x08
#define HD44780_TWI_CURSOR_SHIFT			0x10
#define HD44780_TWI_FUNCTION_SET			0x20
#define HD44780_TWI_SET_CGRAM_ADDR			0x40
#define HD44780_TWI_SET_DRAM_ADDRESS		0x80

//HD44780 display control flags
#define HD44780_TWI_DISPLAY_ON				0x04
#define HD44780_TWI_DISPLAY_OFF				0x00
#define HD44780_TWI_CURSOR_ON				0x02
#define HD44780_TWI_CURSOR_OFF				0x00
#define HD44780_TWI_BLINK_ON				0x01
#define HD44780_TWI_BLINK_OFF				0x00

//HD44780 display/cursor shift flags
#define HD44780_TWI_DISPLAY_MOVE			0x08
#define HD44780_TWI_CURSOR_MOVE				0x00
#define HD44780_TWI_MOVE_RIGHT				0x04
#define HD44780_TWI_MOVE_LEFT				0x00


//HD44780 function set flags
#define HD44780_TWI_4_BITMODE				0x00
#define HD44780_TWI_2_LINE					0x08
#define HD44780_TWI_1_LINE					0x00
#define HD44780_TWI_5x8DOTS					0x00
#define HD44780_TWI_5x10DOTS				0x04

//HD44780 display entry mode flags
#define HD44780_TWI_DECREMENT				0x00
#define HD44780_TWI_INCREMENT				0x02
#define HD44780_TWI_ENTRY_SHIFT_INCREMENT	0x01
#define HD44780_TWI_ENTRY_SHIFT_DECREMENT	0x00

//HD44780 back light control flags
#define HD44780_TWI_BACKLIGHT				0x08
#define HD44780_TWI_NO_BACKLIGHT			0x00

typedef enum
{
	ON = 1,
	OFF = 0

}State_t;

void HD44780_TWI_send_data(char data);
void HD44780_TWI_send_cmd (char cmd);
void HD44780_TWI_lcd_clear(void);
void HD44780_TWI_set_cursor(int row, int col);
void HD44780_TWI_init();
void HD44780_TWI_send_string(char *str);
void HD44780_TWI_display_onoff(State_t State);
void HD44780_TWI_cursor_onoff(State_t State);
void HD44780_TWI_cursor_blinking_onoff(State_t State);


#endif /* HD44780-TWI_H_ */