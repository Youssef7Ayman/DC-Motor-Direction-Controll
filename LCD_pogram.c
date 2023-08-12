/***************************************************************************************************************************************************************************************/
/************************************************* SWE     : LCD DRIVER              *************************************************************************************************************/
/************************************************* BY      : Youssef Ayman           *************************************************************************************************************/
/************************************************* Date    : 27Aug20                 *************************************************************************************************************/
/************************************************* Version : v1.0                    *************************************************************************************************************/
/***************************************************************************************************************************************************************************************/
# include "STD_TYPES.h"
# include "BIT_MATH.h"
# include "DIO_interface.h"
# include "LCD_interface.h"
# include "LCD_private.h"
# include "LCD_config.h"
# include <util/delay.h>



void LCD_VidSendCmd (u8 Copy_u8Cmd)
{
	DIO_VidSetPinValue(CONTROL_PORT,RS_PIN,PinLow);
    DIO_VidSetPinValue(CONTROL_PORT,RW_PIN,PinLow);
    DIO_VidSetPortValue(DATA_PORT,Copy_u8Cmd);
    DIO_VidSetPinValue(CONTROL_PORT,E_PIN,PinHigh);
    _delay_ms(3);
    DIO_VidSetPinValue(CONTROL_PORT,E_PIN,PinLow);
	
	
}


void LCD_VidSendData (u8 Copy_u8Data)
{
	DIO_VidSetPinValue(CONTROL_PORT,RS_PIN,PinHigh);
	DIO_VidSetPinValue(CONTROL_PORT,RW_PIN,PinLow);
	DIO_VidSetPortValue(DATA_PORT,Copy_u8Data);
	DIO_VidSetPinValue(CONTROL_PORT,E_PIN,PinHigh);
	_delay_ms(3);
	DIO_VidSetPinValue(CONTROL_PORT,E_PIN,PinLow);
	
}



void LCD_VidInit (void)
{
	_delay_ms(40);
	LCD_VidSendCmd(0b00111100); // Define:-     No. of lines [B3] : (1) "2 Lines"   ,   Pixels No. [B2] : (1) "5x8"
	_delay_ms(1);
	LCD_VidSendCmd(0b00001100); // Define:-     Display [B2] : (1) "Display On" , Cursor [B1] : (0) "Cursor Off" , Cursor Blink [B0] : (0) "No Blinking"
	LCD_VidSendCmd(1);

}


void LCD_VidDisplayClear (void)
{
	LCD_VidSendCmd(1);

}



void LCD_VidGotoXY (u8 Copy_u8X , u8 Copy_u8Y)             //u8 Address;
{                                                          //if (Copy_u8X==0)
	if (Copy_u8X==1)                                       //{
	{                                                      //   Address=Copy_u8Y;
		Copy_u8Y +=0x40;                                   //}
	}                                                      //else
                                                           //{
	LCD_VidSendCmd(Copy_u8Y + 0b10000000);                 //   Address=Copy_u8Y+64;
                                                           //}
}                                                          //LCD_VidSendCmd(Address + 0b10000000);*/



void LCD_VidSendString (u8 *pu8String)
{
	for (u8 i=0;pu8String[i]!='\0';i++)
	{
		LCD_VidSendData(pu8String[i]);

	}

}


void LCD_VidSendSpecialChar (u8 *Patern,u8 Patrn_num,u8 Copy_u8Xpos,u8 Copy_u8Ypos)
{
	u8 Local_address=8*Patrn_num;
	LCD_VidSendCmd(0b01000000+Local_address);

	for (u8 i=0;i<8;i++)
	{
		LCD_VidSendData(Patern[i]);
	}

	LCD_VidGotoXY(Copy_u8Xpos,Copy_u8Ypos);

	LCD_VidSendData(Patrn_num);


}



void LCD_VidSendValue (u8 Copy_u8Value)
{
	if (Copy_u8Value!=0)
	{
		LCD_VidSendValue(Copy_u8Value/10);
		LCD_VidSendData((Copy_u8Value%10)+48);
	}

}


void LCD_VidCompleteInSecondLineInString (u8 *pu8String)
{
	for (u8 i=0;pu8String[i]!='\0';i++)
		{
			LCD_VidSendData(pu8String[i]);

			if (i==15)
			{
				LCD_VidGotoXY(1,0);
			}
		}

	}


void LCD_VidSendNumber (u32 Copy_u32Number)
{
	u32 Number=Copy_u32Number;
	u8 count=0,flag=0;
	s8 i;
	while (Copy_u32Number>0)
	{
		Copy_u32Number/=10;
		count++;
		flag=1;
	}
	if (flag==0)
	{
		count=1;
		flag=1;
	}

	u8 Arr[count];

	for (i=count-1;i>=0;i--)
	{
		Arr[i]=Number%10+48;
		Number/=10;
	}
	for (i=0;i<count;i++)
	{
		LCD_VidSendData(Arr[i]);
	}


}









