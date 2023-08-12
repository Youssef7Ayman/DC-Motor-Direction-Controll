/*
 * main.c
 *
 *  Created on: Sep 9, 2022
 *      Author: hp
 */
# include "STD_TYPES.h"
# include "DIO_interface.h"
# include "LCD_interface.h"
# include "KPD_interface.h"
# include <util/delay.h>




int main ()
{
	u8 Direction,DIR,flag=1,i;
	u32 Time,TIME=0;

/*----------------------------------------------------------LCD--------------------------------------------------------------------------------*/
	DIO_VidSetPortDirection(Port_D,0b11100000); // LCD CMD Pins  : Port_D {7,6,5--->OutPut} RS,RW,E respectively only
	DIO_VidSetPortDirection(Port_C,0b11111111); // LCD DATA Pins : Port_C {7,6,5,4,3,2,1,0--->OutPut}
//---------------------------------------------------------------------------------------------------------------------------------------------
/*----------------------------------------------------------KPD--------------------------------------------------------------------------------*/
	DIO_VidSetPortDirection(Port_A,0b00001111); // KPD Pins      : Port_A {7,6,5,4-->Input} R4,R3,R2,R1 | {3,2,1,0-->Output} C1,C2,C3,C4 (Gnd)
	DIO_VidSetPortValue(Port_A,0b11110000);     // Pullup Reseistors activation for KPD Input pins : R4,R3,R2,R1
//---------------------------------------------------------------------------------------------------------------------------------------------
/*---------------------------------------------------DC Motor,LEDs,Buzzer----------------------------------------------------------------------*/
												// DC Motor Pins : Port_B {3,2,1,0-->Output} AntiClockwise<-Pin1,Pin2   ,  Clockwise<-Pin1,Pin0
	DIO_VidSetPortDirection(Port_B,0b11111111); // LEDs Pins     : Port_B {6(Green),5(Yellow),4(Red)-->Output}
	                                            // Buzzer Pins   : Port_B {7-->Output}
//---------------------------------------------------------------------------------------------------------------------------------------------

	LCD_VidDisplayClear();
	LCD_VidInit();

	LCD_VidSendString("     WELCOME");
	LCD_VidGotoXY(1,1);
	LCD_VidSendString("   *-------*");
	_delay_ms(1000);
	LCD_VidDisplayClear();

	LCD_VidSendString("Clockwise----->C");
	LCD_VidGotoXY(1,0);
	LCD_VidSendString("AntiClockwise->A");
	_delay_ms(2000);

	while (1)
	{
		LCD_VidDisplayClear();
		LCD_VidSendString("Please enter the ");
		LCD_VidGotoXY(1,0);
		LCD_VidSendString("DIRECTION : [ ]");
		while (1)
		{
			Direction=KPD_u8GetPressedKey();
			if (Direction!=101)
			{
				LCD_VidDisplayClear();
				LCD_VidSendString("Please enter the ");
				LCD_VidGotoXY(1,0);
				LCD_VidSendString("DIRECTION : [ ]");

				LCD_VidGotoXY(1,13);

				if (Direction=='E')
				{
					LCD_VidDisplayClear();
					break;
				}
				DIR=Direction;
				LCD_VidSendData(Direction);

			}
		}

		LCD_VidDisplayClear();
		LCD_VidSendString("Please enter the ");
		LCD_VidGotoXY(1,0);
		LCD_VidSendString("TIME(Sec) : ");

		while (1)
		{
			Time=KPD_u8GetPressedKey();
			if (Time!=101)
			{
				LCD_VidDisplayClear();
				LCD_VidSendString("Please enter the ");
				LCD_VidGotoXY(1,0);
				LCD_VidSendString("TIME(Sec) : ");

				if (Time=='E')
				{
					LCD_VidDisplayClear();
					break;
				}

				else if (Time=='B')
				{
					TIME=TIME/10;
					flag=0;
				}
				else if (flag==1)
				{
					TIME=(TIME*10)+Time-48;
				}

				LCD_VidSendNumber(TIME);
				flag=1;


			}

		}

		switch (DIR)
		{
			case 'C':
			{
				DIO_VidSetPinValue(Port_B,Pin6,PinHigh);

				LCD_VidSendString("DCMotor operates ");
				LCD_VidGotoXY(1,0);
				LCD_VidSendString("in C.W for ");
				LCD_VidSendNumber(TIME);
				LCD_VidSendString("sec");


				DIO_VidSetPinValue(Port_B,Pin0,PinHigh);
				DIO_VidSetPinValue(Port_B,Pin1,PinHigh);
				_delay_ms(TIME*1000);

				DIO_VidSetPinValue(Port_B,Pin6,PinLow);

				DIO_VidSetPinValue(Port_B,Pin0,PinLow);
				DIO_VidSetPinValue(Port_B,Pin1,PinLow);

				for (i=0;i<5;i++)
				{
					DIO_VidSetPinValue(Port_B,Pin4,PinHigh);
					DIO_VidSetPinValue(Port_B,Pin7,PinHigh);
					_delay_ms(100);
					DIO_VidSetPinValue(Port_B,Pin4,PinLow);
					DIO_VidSetPinValue(Port_B,Pin7,PinLow);
					_delay_ms(50);

				}

				break;
			}

			case 'A':
			{
				DIO_VidSetPinValue(Port_B,Pin5,PinHigh);

				LCD_VidSendString("DCMotor operates ");
				LCD_VidGotoXY(1,0);
				LCD_VidSendString("in AC.W for ");
				LCD_VidSendNumber(TIME);
				LCD_VidSendString("sec");


				DIO_VidSetPinValue(Port_B,Pin2,PinHigh);
				DIO_VidSetPinValue(Port_B,Pin3,PinHigh);
				_delay_ms(TIME*1000);

				DIO_VidSetPinValue(Port_B,Pin5,PinLow);

				DIO_VidSetPinValue(Port_B,Pin2,PinLow);
				DIO_VidSetPinValue(Port_B,Pin3,PinLow);

				for (i=0;i<5;i++)
				{
					DIO_VidSetPinValue(Port_B,Pin4,PinHigh);
					DIO_VidSetPinValue(Port_B,Pin7,PinHigh);
					_delay_ms(100);
					DIO_VidSetPinValue(Port_B,Pin4,PinLow);
					DIO_VidSetPinValue(Port_B,Pin7,PinLow);
					_delay_ms(50);

				}
				break;

			}

			default :
			{
				DIO_VidSetPinValue(Port_B,Pin7,PinHigh);

				LCD_VidDisplayClear();
				LCD_VidSendString("     InValid");
				LCD_VidGotoXY(1,4);
				LCD_VidSendString("Direction!");
				_delay_ms(1000);

				DIO_VidSetPinValue(Port_B,Pin7,PinLow);

				break;
			}

		}

		DIR=0;
		TIME=0;

	}









}
