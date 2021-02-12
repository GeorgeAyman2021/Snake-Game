/*
 * main2.c
 *
 *  Created on: Dec 4, 2020
 *      Author: George Ayman
 */



#include <util/delay.h>
#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"
#include "DIO_LIB/DIO_Interface.h"
#include "LCD_LIB/LCD_Init.h"
#include "LCD_LIB/LCD_Config.h"
#include<stdlib.h>


#define BUTTON_LEFT 0
#define BUTTON_RIGHT 1

#define SWITCH_ON	0
#define SWITCH_OFF	1


typedef struct Pos {
	u8 x, y;
	struct Pos* Next;
}Snake;

typedef struct app{
	u8 x, y;
}Apple;


enum {SNAKE_UP,SNAKE_DOWN}snakeDirection1;
enum {SNAKE_LEFT,SNAKE_RIGHT}snakeDirection2;

u8 SNAKE_TALL=1;

Apple apple;


void EndInsert(Snake** head)
{
	Snake* P=*head;
	while(P->Next!=0)
		P=P->Next;
	Snake* temp=0;
	temp=(Snake*)malloc(sizeof(Snake));
	temp->Next=0;
	P->Next=temp;
	temp->y=P->y;
	if(snakeDirection2==SNAKE_RIGHT)
	{
		temp->x=((P->x)-1);
	}
	else
	{
		temp->x=((P->x)+1);
	}
	SNAKE_TALL++;
}
void SPrint(Snake ** head)
{

	Snake* P=*head;
	LCD_VoidSendCmd(LCD_CLEAR);
	_delay_ms(10);
	LCD_VoidGoToXY(apple.x,apple.y);
	LCD_VoidSendChar(2);
	while(P->Next!=0){
		if(P==*head)
		{
			LCD_VoidGoToXY(P->x , P->y);
			LCD_VoidSendChar(0);
		}
		else{
			LCD_VoidGoToXY(P->x , P->y);
			LCD_VoidSendChar(1);

		}
		P=P->Next;
	}
	LCD_VoidGoToXY(P->x , P->y);
	LCD_VoidSendChar(1);


}
void Calculate(Snake ** head,u8 nx,u8 ny)
{
	Snake* P=*head;
	u8 Px =P->x;
	u8 Py =P->y;
	u8 tx,ty;
	while(P->Next!=0){
		if(P==*head)
		{
			P->x=nx;
			P->y=ny;
		}
		else{
			tx=P->x;
			ty=P->y;
			P->x=Px;
			P->y=Py;
			Px=tx;
			Py=ty;
		}
		P=P->Next;
	}
	tx=P->x;
	ty=P->y;
	P->x=Px;
	P->y=Py;
	Px=tx;
	Py=ty;
}
void ApplePos()
{

	apple.x=rand()%15;
	apple.y=rand()%2;
}
void Calcuate_Apple_pos(Snake** head)
{

	Snake* P=*head;
	ApplePos();
	while(P->Next!=0)
	{
		if(P->x==apple.x && P->y==apple.y)
		{
			ApplePos();
			P=*head;
		}
		P=P->Next;
	}

}

void Check(Snake** head)
{
	Snake* P=*head;
	Snake* N=P->Next;
	if(SNAKE_TALL==30)
	{
		LCD_VoidSendCmd(LCD_CLEAR);
		_delay_ms(10);
		LCD_VoidGoToXY(0,0);
		LCD_VoidSendStr("You Are Win ");
		LCD_VoidGoToXY(13,0);
		LCD_VoidSendChar(4);
		LCD_VoidGoToXY(0,1);
		LCD_VoidSendStr("Score : ");
		LCD_VoidGoToXY(8,1);
		LCD_VoidSendNum(SNAKE_TALL);
		while(1);
	}
	else
	{
		while(N->Next!=0)
		{
			if(P->x==N->x && P->y==N->y)
			{
				LCD_VoidSendCmd(LCD_CLEAR);
				_delay_ms(10);
				LCD_VoidGoToXY(0,0);
				LCD_VoidSendStr("You Are Lose ");
				LCD_VoidGoToXY(13,0);
				LCD_VoidSendChar(3);
				LCD_VoidGoToXY(0,1);
				LCD_VoidSendStr("Score : ");
				LCD_VoidGoToXY(8,1);
				LCD_VoidSendNum(SNAKE_TALL);

				while(1);
			}
			N=N->Next;
		}
	}
}
void Snake_Init ()
{

	u8 Snake[]= {0B10001, 0B01010, 0B11111, 0B10101, 0B11111, 0B10101, 0B10001, 0B01110,	//Snake Head    0
			0B00100, 0B01110, 0B01110, 0B11011, 0B11011, 0B01110, 0B01110, 0B00100,			//Snake Tail	1
			0B00000, 0B00000, 0B00010, 0B00100, 0B01110, 0B11011, 0B11110, 0B01100,			//Apple			2
			0x00, 0x0A, 0x00, 0x04, 0x0E, 0x11, 0x00, 0x00,									//lose			3
			0x00, 0x0A, 0x00, 0x11, 0x0E, 0x04, 0x00, 0x00									//Win			4
	};


	u8 i;
	LCD_VoidSendCmd(LCD_CGRAM_ADD0);
	for(i=0 ; i<40 ; i++)
	{
		LCD_VoidSendChar(Snake[i]);
	}

	LCD_VoidSendCmd(LCD_DDRAM_ADD0);


}
void main()
{
	snakeDirection2=SNAKE_RIGHT;
	Calcuate_Apple_pos(&Head);

	Snake* Head =0;
	Head=(Snake*)malloc(sizeof(Snake));
	Head->Next=0;
	Head->x=2;
	Head->y=0;
	EndInsert(&Head);
	EndInsert(&Head);

	s8 x,y;
	s8 inc_dec=1;
	LCD_VoidInit();
	Snake_Init();

	LCD_VoidSendStr("Wellcome Iti");
	LCD_VoidGoToXY(3, 1);
	LCD_VoidSendStr("George Snake");
	DIO_VoidSetPinDirection(PORTA,PIN0,INPUT);
	DIO_VoidSetPinDirection(PORTA,PIN1,INPUT);
	DIO_VoidSetPinValue(PORTA,PIN0,HIGH);
	DIO_VoidSetPinValue(PORTA,PIN1,HIGH);
	_delay_ms(2000);
	LCD_VoidSendCmd(LCD_CLEAR);
	_delay_ms(10);
	SPrint(&Head);


	x=2;
	y=0;

	snakeDirection1=SNAKE_UP;
	snakeDirection2=SNAKE_RIGHT;
	u8 counter=0;

	while(1)
	{
		

		if(counter==25)
		{
			x+=inc_dec;
			Calculate(&Head,x,y);
			SPrint(&Head);
			Check(&Head);
			counter=0;

		}
		_delay_ms(10);
		counter++;
		if(x==16 && snakeDirection2==SNAKE_RIGHT)
		{
			x=-1;
			SPrint(&Head);
		}
		else if (x<0 && snakeDirection2==SNAKE_LEFT)
		{
			x=16;
			SPrint(&Head);
		}

		if(DIO_U8GetPinValue(PORTA,BUTTON_LEFT)==SWITCH_ON){
			while(DIO_U8GetPinValue(PORTA,BUTTON_LEFT)==SWITCH_ON)
				_delay_ms(100);
			snakeDirection2=SNAKE_LEFT;
			switch(snakeDirection1){

			case SNAKE_UP:       snakeDirection1=SNAKE_DOWN; y=1;inc_dec=-1;break;

			case SNAKE_DOWN:   snakeDirection1=SNAKE_UP;y=0;inc_dec=-1;break;
			}

			Calculate(&Head,x,y);
			SPrint(&Head);
			counter=0;

		}


		if(DIO_U8GetPinValue(PORTA,BUTTON_RIGHT)==SWITCH_ON){
			while(DIO_U8GetPinValue(PORTA,BUTTON_RIGHT)==SWITCH_ON)
				_delay_ms(100);
			snakeDirection2=SNAKE_RIGHT;
			switch(snakeDirection1){
			case SNAKE_UP:       snakeDirection1=SNAKE_DOWN; y=1;inc_dec=1;break;

			case SNAKE_DOWN:   snakeDirection1=SNAKE_UP;y=0;inc_dec=1;break;
			}
			Calculate(&Head,x,y);
			SPrint(&Head);
			counter=0;


		}

		if(Head->x==apple.x && Head->y==apple.y)
		{
			EndInsert(&Head);
			Calcuate_Apple_pos(&Head);
			
		}
	}
}



