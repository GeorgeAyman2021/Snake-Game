/*
 * LCD_Prog.c
 *
 *  Created on: Nov 30, 2020
 *      Author: 20122
 */


#include <util/delay.h>
#include "../LIB/STD_TYPES.h"
#include "../DIO_LIB/DIO_Interface.h"
#include "LCD_Config.h"

void LCD_VoidInit(void)
{
	DIO_VoidSetPinDirection(CONTROL_PORT,RS,OUTPUT);
	DIO_VoidSetPinDirection(CONTROL_PORT,RW,OUTPUT);
	DIO_VoidSetPinDirection(CONTROL_PORT,En,OUTPUT);
	DIO_VoidSetPortDirection(DATA_PORT,0xff);
	_delay_ms(35);
#if MODE==EIGHT_BIT
	LCD_VoidSendCmd(LCD_FUNCTIONSET_8BIT);
	_delay_ms(1);
	LCD_VoidSendCmd(LCD_DISPLAYON);
	_delay_ms(1);
	LCD_VoidSendCmd(LCD_CLEAR);
	_delay_ms(2);

#elif MODE==FOUR_BIT

	LCD_VoidSendCmd(LCD_HOME);
	_delay_ms(1);
	LCD_VoidSendCmd(LCD_FUNCTIONSET_4BIT);
	_delay_ms(1);
	LCD_VoidSendCmd(LCD_DISPLAYON);
	_delay_ms(1);
	LCD_VoidSendCmd(LCD_CLEAR);
	_delay_ms(15);
	LCD_VoidSendCmd(LCD_ENTERYMODE);
	_delay_ms(2);

#endif
}

void LCD_VoidSendCmd(u8 Copy_U8Cmd)
{
#if MODE==EIGHT_BIT
	//Reset (RS&RW)pins.
	DIO_VoidSetPinValue(CONTROL_PORT,RS,LOW);
	DIO_VoidSetPinValue(CONTROL_PORT,RW,LOW);
	//Send command through data port
	DIO_VoidSetPortValue(DATA_PORT,Copy_U8Cmd);
	/*Set the (E) pin for a period
		defined in datasheet ( 1ms is
		working) then reset it.*/
	DIO_VoidSetPinValue(CONTROL_PORT,En,HIGH);
	_delay_ms(1);
	DIO_VoidSetPinValue(CONTROL_PORT,En,LOW);
#elif MODE==FOUR_BIT
	//Reset (RS&RW)pins.
	DIO_VoidSetPinValue(CONTROL_PORT,RS,LOW);
	DIO_VoidSetPinValue(CONTROL_PORT,RW,LOW);
	//Send most 4 bits command through data port
	DIO_VoidSetPortValue(DATA_PORT,Copy_U8Cmd);
	/*Set the (E) pin for a period
		defined in datasheet ( 1ms is
		working) then reset it.*/
	DIO_VoidSetPinValue(CONTROL_PORT,En,HIGH);
	_delay_ms(1);
	DIO_VoidSetPinValue(CONTROL_PORT,En,LOW);
	//Send least command through data port
	_delay_ms(5);
	DIO_VoidSetPortValue(DATA_PORT,(Copy_U8Cmd<<4));
	/*Set the (E) pin for a period
			defined in datasheet ( 1ms is
			working) then reset it.*/
	DIO_VoidSetPinValue(CONTROL_PORT,En,HIGH);
	_delay_ms(1);
	DIO_VoidSetPinValue(CONTROL_PORT,En,LOW);
#endif
}


void LCD_VoidSendChar(u8 Copy_U8Char)
{
#if MODE==EIGHT_BIT
	//Set RS and Reset RW pins
	DIO_VoidSetPinValue(CONTROL_PORT,RS,HIGH);
	DIO_VoidSetPinValue(CONTROL_PORT,RW,LOW);
	//Send DATA through data port.
	DIO_VoidSetPortValue(DATA_PORT,Copy_U8Char);
	/*Set the (E) pin for a period
		defined in datasheet ( 1ms is
		working) then reset it.*/
	DIO_VoidSetPinValue(CONTROL_PORT,En,HIGH);
	_delay_ms(1);
	DIO_VoidSetPinValue(CONTROL_PORT,En,LOW);
#elif MODE==FOUR_BIT
	//Set RS and Reset RW pins
	DIO_VoidSetPinValue(CONTROL_PORT,RS,HIGH);
	DIO_VoidSetPinValue(CONTROL_PORT,RW,LOW);
	//Send Most 4 bits DATA through data port.
	DIO_VoidSetPortValue(DATA_PORT,Copy_U8Char);
	/*Set the (E) pin for a period
			defined in datasheet ( 1ms is
			working) then reset it.*/
	DIO_VoidSetPinValue(CONTROL_PORT,En,HIGH);
	_delay_ms(1);
	DIO_VoidSetPinValue(CONTROL_PORT,En,LOW);
	//Send Least 4 bits DATA through data port.
	_delay_ms(5);
	DIO_VoidSetPortValue(DATA_PORT,(Copy_U8Char<<4));
	/*Set the (E) pin for a period
			defined in datasheet ( 1ms is
			working) then reset it.*/
	DIO_VoidSetPinValue(CONTROL_PORT,En,HIGH);
	_delay_ms(1);
	DIO_VoidSetPinValue(CONTROL_PORT,En,LOW);
#endif
}

void LCD_VoidSendStr (u8* Ptr_Str)
{
	u8 i=0;
	while(Ptr_Str[i]!='\0')
	{
		LCD_VoidSendChar(Ptr_Str[i]);
		i++;
	}
}
void LCD_VoidSendNum(u16 Copy_U16Num)
{
	if (Copy_U16Num==0)
	{
		LCD_VoidSendChar('0');
	}
	s8 i=0;
	u8 str_num[5];
	while(Copy_U16Num>0)
	{
		str_num[i]=Copy_U16Num%10;
		Copy_U16Num/=10;
		i++;
	}
	i--;
	while(i>=0)
	{
		switch(str_num[i])
		{
		case 0 : LCD_VoidSendChar('0');break;
		case 1 : LCD_VoidSendChar('1');break;
		case 2 : LCD_VoidSendChar('2');break;
		case 3 : LCD_VoidSendChar('3');break;
		case 4 : LCD_VoidSendChar('4');break;
		case 5 : LCD_VoidSendChar('5');break;
		case 6 : LCD_VoidSendChar('6');break;
		case 7 : LCD_VoidSendChar('7');break;
		case 8 : LCD_VoidSendChar('8');break;
		case 9 : LCD_VoidSendChar('9');break;
		}

		i--;
	}
}

void LCD_VoidGoToXY(u8 Copy_U8Col ,u8 Copy_U8Row  )
{
	if (Copy_U8Row==0)
	{
		LCD_VoidSendCmd(LCD_SETCURSER0+Copy_U8Col);//127
	}
	else if (Copy_U8Row==1)
	{
		LCD_VoidSendCmd(LCD_SETCURSER1+Copy_U8Col);//191
	}
}




void LCD_VoidSendSpcial (u8 Copy_U8Row,u8 Copy_U8Col)
{

	u8 Game[]= {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,// Right
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,//Left
			0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x00, 0x00, 0x00 //Ball

	};


	u8 i,j;
	LCD_VoidSendCmd(LCD_CGRAM_ADD0);
	for(i=0 ; i<24 ; i++)
	{
		LCD_VoidSendChar(Game[i]);
	}

	LCD_VoidSendCmd(LCD_DDRAM_ADD0);

	LCD_VoidGoToXY(Copy_U8Row,Copy_U8Col);
	for(j=0;j<3;j++){
		LCD_VoidSendChar(j);
	}

}
