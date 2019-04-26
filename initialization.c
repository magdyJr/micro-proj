#include "tm4c123gh.h"
#include "LCDHeader.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
//This Function is used to send commands to the LCD
void LCD_command(unsigned char command)
{
    GPIO_PORTA_DATA_R = 0;     /* 0x20 = 0, R/W = 0 */
    GPIO_PORTB_DATA_R = command;
    GPIO_PORTA_DATA_R = 0x80;    /* pulse E */
    Delay_ms(1);			 /* Enable pulse Width */
    GPIO_PORTA_DATA_R = 0;
    if (command < 4)
        Delay_ms(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        Delay_Us(40);        /* all others 40 us */
}
void LCD_data(unsigned char data)
{
    GPIO_PORTA_DATA_R = 0x20;    /* RS = 1, R/W = 0 */
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTA_DATA_R = 0x80 | 0x20;   /* pulse E */
    Delay_ms(1);
    GPIO_PORTA_DATA_R = 0;
    Delay_Us(40);
}

void LCD_init(void){
	  SYSCTL_RCGCGPIO_R |=0x17;               //Enable Port A&B Clock
	  //while((SYSCTL_PRGPIO_R & 0x17) == 0){}; //Wait for clock stabilization
		 GPIO_PORTB_DIR_R |= 0xFF;
		 GPIO_PORTA_DIR_R |= 0xFF;
			
     GPIO_PORTB_DEN_R |= 0xFF;
		 GPIO_PORTA_DEN_R |= 0xFF;
			
		 GPIO_PORTB_AMSEL_R &= ~0xFF;
		 GPIO_PORTA_AMSEL_R &= ~0xFF;
			
		 GPIO_PORTB_AFSEL_R &= 0x00;
		 GPIO_PORTA_AFSEL_R &= 0x00;
			
			
			
		//keypad
			GPIO_PORTE_DIR_R |= 0x0F; /* set row pins 3-0 as output */
GPIO_PORTE_DEN_R |= 0x0F; /* set row pins 3-0 as digital pins */
GPIO_PORTE_ODR_R |= 0x0F; /* set row pins 3-0 as open drain */

GPIO_PORTC_DIR_R &= ~0xF0; /* set column pin 7-4 as input */
GPIO_PORTC_DEN_R |= 0xF0; /* set column pin 7-4 as digital pins */
GPIO_PORTC_PUR_R |= 0xF0; /* enable pull-ups for pin 7-4*/
		
			// Wake Up LCD Sequence
    Delay_ms(20);            /* initialization sequence */// Wait >15 ms after power is applied
    LCD_command(0x30);		// command 0x30 = Wake up
    Delay_ms(10);				// must wait 5ms, busy flag not available
    //LCD_command(0x30);		// command 0x30 = Wake up #2
    //Delay_ms(10);			// must wait 100us, busy flag not available
    //LCD_command(0x30);		// command 0x30 = Wake up #3
    Delay_ms(10);			// must wait 100us, busy flag not available
  // Prepare LCD Operation and Function  
    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}
unsigned char ispressed(void)
{
int col;

/* check to see any key pressed */
GPIO_PORTE_DATA_R = 0; /* enable all rows */
col = GPIO_PORTC_DATA_R & 0xF0; /* read all columns */
if (col == 0xF0)
return 0; /* no key pressed */
else
return 1; /* a key is pressed */
}
unsigned char key_pressed(void){
const unsigned char keymap[4][4] = {
{ '1', '2', '3', '+'},
{ '4', '5', '6', '-'},
{ '7', '8', '9', '.'},
{ 'x', '0', '/', '='},
};
int row, col;
/* check to see any key pressed first */
GPIO_PORTE_DATA_R = 0; /* enable all
rows */
col = GPIO_PORTC_DATA_R & 0xF0;
while (1)
{
row = 0;
GPIO_PORTE_DATA_R= 0x0E; /* enable row 0 */
Delay_Us(2); /* wait for signal to settle */
col = GPIO_PORTC_DATA_R& 0xF0;
if (col != 0xF0) {
	switch(col){
		case 0x70:
			col=0;
			break;
		case 0xB0:
			col=1;
			break;
		case 0xD0:
			col=2;
			break;
		case 0xE0:
			col=3;
			break;
	}
	return keymap[row][col];
}

row = 1;
GPIO_PORTE_DATA_R= 0x0D; /* enable row 0 */
Delay_Us(2); /* wait for signal to settle */
col = GPIO_PORTC_DATA_R& 0xF0;
if (col != 0xF0) {
	switch(col){
		case 0x70:
			col=0;
			break;
		case 0xB0:
			col=1;
			break;
		case 0xD0:
			col=2;
			break;
		case 0xE0:
			col=3;
			break;
	}
	return keymap[row][col];
}

	row = 2;
GPIO_PORTE_DATA_R= 0x0B; /* enable row 0 */
Delay_Us(2); /* wait for signal to settle */
col = GPIO_PORTC_DATA_R& 0xF0;
if (col != 0xF0) {
	switch(col){
		case 0x70:
			col=0;
			break;
		case 0xB0:
			col=1;
			break;
		case 0xD0:
			col=2;
			break;
		case 0xE0:
			col=3;
			break;
	}
	return keymap[row][col];
}

	row = 3;
GPIO_PORTE_DATA_R= 0x07; /* enable row 0 */
Delay_Us(2); /* wait for signal to settle */
col = GPIO_PORTC_DATA_R& 0xF0;
if (col != 0xF0) {
	switch(col){
		case 0x70:
			col=0;
			break;
		case 0xB0:
			col=1;
			break;
		case 0xD0:
			col=2;
			break;
		case 0xE0:
			col=3;
			break;
	}
	return keymap[row][col];
} 
}

}
void calc_fun(void){
char x[3],tem1;
	//unsigned char op;
	unsigned char tem;
	int a=0,b,count=0,ans=0;
	float c,d,ans2;
	unsigned char float_flag=0,end_flag=0;
	  LCD_init();
		    LCD_command(1);       /* clear display */
    LCD_command(0x80);/* lcd cursor location */
	Delay_ms(300000);
    while(1)
    {
			
			
      Delay_ms(300000);Delay_ms(300000);
			if(ispressed())
			{
				if(end_flag){
					LCD_command(1);       /* clear display */
    LCD_command(0x80);
					count=0;
				}
				tem=key_pressed();
				
				if(tem=='+'||tem=='-'||tem=='x'||tem=='/'){
					LCD_data(tem);
					count++;
					switch(tem){
						case '+':
							x[count]+=tem;
							count++;
							break;
						case '-':
							x[count]+=tem;
						count++;
							break;
						case 'x':
							x[count]+=tem;
						count++;
							break;
						case '/':
							x[count]+=tem;
						count++;
							break;
					}
				}else if(tem=='.'){
					float_flag=1;		
					LCD_data(tem);
							x[count]+=tem;
					Delay_Us(400);
				}
					else if(tem=='='){
						
						if(float_flag){
							c=(float)(x[0]-'0');
							d=(float)(x[2]-'0');
							switch(x[1]){
						case '+':
							ans2=c+d;
							break;
						case '-':
							ans2=c-d;
							break;
						case 'x':
							ans2=c*d;
							break;
						case '/':
							ans2=c/d;
							break;
					}
							LCD_command(1);     
							LCD_command(0x80);	
					Delay_ms(300000);
					sprintf(&tem1,"%2f",c);
							LCD_data(tem1);
					LCD_data(x[1]);
					sprintf(&tem1,"%2f",d);
					LCD_data(tem1);
					LCD_command(0xC0);
 					//LCD_data(' ');
//					LCD_data(a);
//					LCD_data(' ');
//					LCD_data(b);
					
					Delay_Us(50);
					sprintf(&tem1,"%2f",ans2);
					LCD_data(tem1);
					end_flag=1;
						float_flag=0;
						}
						else{
							a = atoi(&x[0]);
					b=atoi(&x[2]);
						
							
							switch(x[1]){
						case '+':
							ans=a+b;
							break;
						case '-':
							ans=a-b;
							break;
						case 'x':
							ans=a*b;
							break;
						case '/':
							ans=a/b;
							break;
					}
							LCD_command(1);     
							LCD_command(0x80);	
					Delay_ms(300000);
					sprintf(&tem1,"%d",a);
							LCD_data(tem1);
					LCD_data(x[1]);
					sprintf(&tem1,"%d",b);
					LCD_data(tem1);
					LCD_command(0xC0);
 					//LCD_data(' ');
//					LCD_data(a);
//					LCD_data(' ');
//					LCD_data(b);
					
					Delay_Us(50);
					sprintf(&tem1,"%d",ans);
					LCD_data(tem1);
					end_flag=1;
				}
//						
						
					}
				else{
					LCD_data(tem);
							x[count]+=tem;
					Delay_Us(400);
				}
				//LCD_data(key_pressed());
				
			}
      Delay_ms(300000);
        
  
		}	

}
