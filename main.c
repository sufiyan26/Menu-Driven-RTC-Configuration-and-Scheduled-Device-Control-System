#include <lpc214x.h>
#include "types.h"
#include "defines.h"
#include "LCD1_defines.h"
#include "delay.h"
#include "RTC_defines.h"
#include "KPM_defines.h"

#define CONFIG_SW 20
#define EINT3_CHNO 17
#define DEVICE_LED 7



volatile u32 config_request=0;
u32 on_hour=0;
u32 on_min=0;

u32 off_hour=0;
u32 off_min=0;

u32 schedule_active=0;


void WRITE_LCD_CMD(u8 CMD)
{
    SCLRBIT(IOCLR0,LCD_RS);

    WRITEBYTE(IOPIN0,LCD_DATA,CMD);

    SSETBIT(IOSET0,LCD_EN);
    delay_us(1);
    SCLRBIT(IOCLR0,LCD_EN);

    delay_ms(2);
}


void WRITE_LCD_DATA(u8 ascii)
{
    SSETBIT(IOSET0,LCD_RS);

    WRITEBYTE(IOPIN0,LCD_DATA,ascii);

    SSETBIT(IOSET0,LCD_EN);
    delay_us(1);
  SCLRBIT(IOCLR0,LCD_EN);

    delay_ms(2);
}


void INIT_LCD(void)
{
    WRITEBYTE(IODIR0,LCD_DATA,255);

    SETBIT(IODIR0,LCD_RS);
    SETBIT(IODIR0,LCD_EN);

    SCLRBIT(IOCLR0,LCD_RS);
    SCLRBIT(IOCLR0,LCD_EN);

    delay_ms(20);

    WRITE_LCD_CMD(0x30);
    delay_ms(5);

    WRITE_LCD_CMD(0x30);
    delay_us(150);

    WRITE_LCD_CMD(0x30);

    WRITE_LCD_CMD(0x38);
	WRITE_LCD_CMD(0x08);
    WRITE_LCD_CMD(0x01);
    WRITE_LCD_CMD(0x06);
    WRITE_LCD_CMD(0x0C);
}


void strLCD(s8 *str)
{
    while(*str)
    {
        WRITE_LCD_DATA(*str++);
    }
}


void U32LCD(u32 n)
{
    char a[10];
    int i=0;

    if(n==0)
    {
        WRITE_LCD_DATA('0');
    }
    else
    {
        while(n)
	{
            a[i++]=n%10+'0';
            n=n/10;
        }

        for(--i;i>=0;i--)
        {
            WRITE_LCD_DATA(a[i]);
        }
    }
}


/*------------------------------------------------
              NEW: READ NUMBER
------------------------------------------------*/

u32 ReadNumber(u32 digits)
{
    u8 key;
    u32 num=0;
    u32 count=0;
	//u32 start_pos;

	//start_pos=3;


    while(1)
    {
        key=KeyScan();

        if(key>='0' && key<='9')
	    {
           if(count<digits)
		   {
		    num=(num*10)+(key-'0');

            WRITE_LCD_DATA(key);

            count++;
        }
		}
		 else if(key=='C')
        {
           if(count>0)
		   {
			   count--;
			   num=num/10;

            //WRITE_LCD_CMD(GOTO_LINE2_POS0+start_pos+count);
			   WRITE_LCD_CMD(GOTO_LINE2_POS0+3+count);
            WRITE_LCD_DATA(' ');

            //WRITE_LCD_CMD(GOTO_LINE2_POS0+start_pos+count);
				   WRITE_LCD_CMD(GOTO_LINE2_POS0+3+count);
        }
    }
	else if(key== '#')
	{
	if(count==digits)
	return num;

    }
	}
	}
	
	

   



/*------------------------------------------------
              NEW: LEAP YEAR
------------------------------------------------*/

u32 IsLeapYear(u32 year)
{
    if(year%400==0)
        return 1;

    if(year%100==0)
        return 0;

    if(year%4==0)
        return 1;
   return 0;
}


/*------------------------------------------------
              NEW: DAYS IN MONTH
------------------------------------------------*/

u32 DaysInMonth(u32 month,u32 year)
{
    if(month==2)
    {
        if(IsLeapYear(year))
            return 29;
        else
            return 28;
    }

    if(month==4 || month==6 || month==9 || month==11)
        return 30;

    return 31;
}


/*------------------------------------------------
              NEW: SET TIME
------------------------------------------------*/
 void SetTimeUsingKeypad(void)
{
     u32 h,m,s;

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(GOTO_LINE1_POS0);
    strLCD("SET TIME");

    WRITE_LCD_CMD(GOTO_LINE2_POS0);
    strLCD("HH=");

    h=ReadNumber(2);

    WRITE_LCD_CMD(GOTO_LINE2_POS0);
    strLCD("                ");

    WRITE_LCD_CMD(GOTO_LINE2_POS0);
    strLCD("MM=");

    m=ReadNumber(2);

    WRITE_LCD_CMD(GOTO_LINE2_POS0);
    strLCD("                ");

    WRITE_LCD_CMD(GOTO_LINE2_POS0);
    strLCD("SS=");

    s=ReadNumber(2);

    if(h<24 && m<60 && s<60)
	 {
        SetRTCTimeInfo(h,m,s);

        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("TIME SET");

        delay_s(1);
    }
    else
    {
        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("INVALID TIME");

        delay_s(1);
    }

}


/*------------------------------------------------
              NEW: SET DATE
------------------------------------------------*/

void SetDateUsingKeypad(void)
{
    u32 d,m,y;

    while(1)
    {
        WRITE_LCD_CMD(CLEAR_LCD);
 WRITE_LCD_CMD(GOTO_LINE1_POS0);
        strLCD("SET DATE");

        WRITE_LCD_CMD(GOTO_LINE2_POS0);
        strLCD("DD=");

        d=ReadNumber(2);

        WRITE_LCD_CMD(GOTO_LINE2_POS0);
        strLCD("MM=");

        m=ReadNumber(2);

        WRITE_LCD_CMD(GOTO_LINE2_POS0);
        strLCD("YYYY=");

        y=ReadNumber(4);

        if(m>=1 && m<=12)
        {
            if(d>=1 && d<=DaysInMonth(m,y))
            {
                SetRTCDateInfo(d,m,y);
 WRITE_LCD_CMD(CLEAR_LCD);
                strLCD("DATE SET");

                delay_s(1);

                return;
            }
        }

        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("INVALID DATE");

        delay_s(1);
    }
}


/*------------------------------------------------
              NEW: SET DAY
------------------------------------------------*/

void SetDayUsingKeypad(void)
{
    u32 d;

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(GOTO_LINE1_POS0);
	strLCD("SET DAY");

   WRITE_LCD_CMD(GOTO_LINE2_POS0);
	strLCD("DAY");
 d=ReadNumber(1);

    if(d>=1 && d<=7)
    {
        SetRTCDay(d-1);

        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("DAY SET");

        delay_s(1);
		return;
    }
	
    else
    {
        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("INVALID DAY");
        delay_s(1);

    
    }
}


  /*------------------------------------------------
              EINT3 ISR
------------------------------------------------*/

void eint3_isr(void) __irq
{
    config_request=1;

    EXTINT=(1<<3);

    VICVectAddr=0;
}

/*------------------------------------------------
              EINT3 INITIALIZATION
------------------------------------------------*/

void EINT3_Init(void)
{
    /* P0.20 -> EINT3 */

    PINSEL1 &= ~(3<<8);
    PINSEL1 |=  (3<<8);

    /* Falling edge */

    EXTMODE |= (1<<3);

    /* Falling edge polarity */

    EXTPOLAR &= ~(1<<3);

    /* Clear pending EINT3 flag */

    EXTINT=(1<<3);


    /* Configure VIC */

    VICIntSelect &= ~(1<<EINT3_CHNO);

    VICVectCntl0=(1<<5)|EINT3_CHNO;

    VICVectAddr0=(u32)eint3_isr;
	VICIntEnable=(1<<EINT3_CHNO);
}

void setscheduleusingkeypad(void)
{

   u32 oh,om,fh,fm;

   while(1)
   {    /*set on time*/
     WRITE_LCD_CMD(CLEAR_LCD);
     WRITE_LCD_CMD(GOTO_LINE1_POS0);
	 strLCD("ON TIME");
	   WRITE_LCD_CMD(GOTO_LINE2_POS0);
	 strLCD("HH= ");
   				oh=ReadNumber(2);
		 WRITE_LCD_CMD(GOTO_LINE2_POS0);
	            strLCD("             ");
	WRITE_LCD_CMD(GOTO_LINE2_POS0);
	 strLCD("MM= ");
   	  	om=ReadNumber(2);

		 /*set off time*/

   	   WRITE_LCD_CMD(CLEAR_LCD);
     WRITE_LCD_CMD(GOTO_LINE1_POS0);
	 strLCD("OFF TIME");
	   WRITE_LCD_CMD(GOTO_LINE2_POS0);
	 strLCD("HH= ");
   				fh=ReadNumber(2);
		 WRITE_LCD_CMD(GOTO_LINE2_POS0);
	            strLCD("             ");
	WRITE_LCD_CMD(GOTO_LINE2_POS0);
	 strLCD("MM= ");
   	  	fm=ReadNumber(2);
     /*valid time*/

	 if(oh<24 && om<60 && fh<24 && fm<60)
	 {
	     /*on_hour=oh;
		 on_min=om;

		 off_hour=fh;
		 off_min=fm;*/

		 if(oh==fh && om==fm)
		 {  
		 WRITE_LCD_CMD(CLEAR_LCD);
		 WRITE_LCD_CMD(GOTO_LINE1_POS0);
		 strLCD("ON/OFF SAME");
		 
		   delay_s(2);
		   continue;
		 
		 }
		 on_hour=oh;
		 on_min=om;

		 off_hour=fh;
		 off_min=fm;

		 /*temp addintion*/

		 WRITE_LCD_CMD(CLEAR_LCD);
		 WRITE_LCD_CMD(GOTO_LINE1_POS0);
		 strLCD("ON=");
		 WRITE_LCD_DATA(on_hour/10+'0');
		 WRITE_LCD_DATA(on_hour%10+'0');
		 WRITE_LCD_DATA(':');
		 WRITE_LCD_DATA(on_min/10+'0');
		 WRITE_LCD_DATA(on_min%10+'0');
		 WRITE_LCD_CMD(GOTO_LINE2_POS0);
		 strLCD("OFF=");
		 WRITE_LCD_DATA(off_hour/10+'0');
		 WRITE_LCD_DATA(off_hour%10+'0');
		 WRITE_LCD_DATA(':');

		 WRITE_LCD_DATA(off_min/10+'0');
		 WRITE_LCD_DATA(off_min%10+'0');

		 delay_s(2);

		// schedule_active=1;
		  
		  WRITE_LCD_CMD(CLEAR_LCD);
     WRITE_LCD_CMD(GOTO_LINE1_POS0);
	 strLCD("schedule set");
						  delay_s(1);
						  	 return;
						  }
						  else
						  {		 WRITE_LCD_CMD(CLEAR_LCD);
     WRITE_LCD_CMD(GOTO_LINE1_POS0);
	 strLCD("INVALID TIME");
						  delay_s(2);
	 					   return;
	 
	 	  
	 }
	    
   }

}


void devicecontrol(void)
{
     u32 current_time;
	 u32 on_time;
	 u32 off_time;

	 /*if(schedule_active==0)
	 {
	     SETBIT(IOCLR0,DEVICE_LED);
		 return;
	 } */

	 current_time=(HOUR*60)+MIN;

	 on_time=(on_hour*60)+on_min;

	 off_time=(off_hour*60)+off_min;

	  if(current_time < on_time)
	  {
	  	  SETBIT(IOCLR0,DEVICE_LED);	
	  
	  }

	   else if(current_time < off_time)
	  {
	  	  SETBIT(IOSET0,DEVICE_LED);	
	  
	  }
	  else
	  {
	  	SETBIT(IOCLR0,DEVICE_LED);	
	  
	  }
	 if(on_time<off_time)
	 {
	 
	    if(current_time>=on_time && current_time<off_time)
		SETBIT(IOSET0,DEVICE_LED);
  else
	 	 SETBIT(IOCLR0,DEVICE_LED);
	}
	else if(on_time>off_time)
	{
	    if(current_time>=on_time||current_time<off_time)
				 	SETBIT(IOSET0,DEVICE_LED);
  else
	 	 SETBIT(IOCLR0,DEVICE_LED);	
	  
	  }
	  else
	  {
	  
        SETBIT(IOCLR0,DEVICE_LED);
	  
	  }	
	    

   }


void configmenu(void)
{
   u8 key;
   while(1)
   {
   WRITE_LCD_CMD(CLEAR_LCD);
     WRITE_LCD_CMD(GOTO_LINE1_POS0);
	 strLCD("1.Time 2.Date ");
	   WRITE_LCD_CMD(GOTO_LINE2_POS0);
	 strLCD("3.Day 4.Sched ");

	  while(ColScan()!=0);
	  key=KeyScan();
	  if(key== '1')
	  {
		 SetTimeUsingKeypad();
		 return;
		 }
		 else if(key=='2')
		 {
		 	   SetDateUsingKeypad();
			   return;
		 
		 }
            else if(key=='3')
		 {
		 	   SetDayUsingKeypad();
			   return;
		 }
					 else if(key=='4')
					 {
					 	setscheduleusingkeypad();
					    return;
					 }
					 else if(key=='D')
				     return;
					 
					 }
}


void startupdisplay(void)
{
   s8 project[]="Menu-Driven RTC Configuration "; //and Scheduled Device Control System
   u32 len;
   s32 pos;
   u32 i;
   s32 index;

   len=0;
   while(project[len]!='\0')
   len++;

   WRITE_LCD_CMD(CLEAR_LCD);
   delay_ms(2);

   WRITE_LCD_CMD(GOTO_LINE1_POS0);
   strLCD("MOHAMMED SUFIYAN");

   for(pos=0; pos<len+16;pos++)
   {
     WRITE_LCD_CMD(GOTO_LINE2_POS0);   
   	 for(i=0; i<16;i++)
	 { 
	   index=(s32)i+(s32)pos- (s32)len;
	   if(index>=0 && index<(s32)len)
	 WRITE_LCD_DATA(project[index]);
					else
					WRITE_LCD_DATA(' ');
	}

	 delay_ms(200);
	 
	 
   }
    WRITE_LCD_CMD(CLEAR_LCD);

}

 void DisplayDay(u32 day)
{
    WRITE_LCD_DATA(' ');

	if(day==0)
	strLCD("SUN");
	else if(day==1)
	strLCD("MON");
		else if(day==2)
	strLCD("TUE");
		else if(day==3)
	strLCD("WED");
		else if(day==4)
	strLCD("THUR");
		else if(day==5)
	strLCD("FRI");
		else if(day==6)
	strLCD("SAT");

}

void normaldisplay(void)
{
WRITE_LCD_CMD(GOTO_LINE1_POS0);  
DisplayRTCTime(HOUR,MIN,SEC);
WRITE_LCD_CMD(GOTO_LINE2_POS0);
DisplayRTCDate(DOM,MONTH,YEAR);
DisplayDay(DOW);


}

void displaydevicestatus(void)
{
  WRITE_LCD_CMD(CLEAR_LCD);
  WRITE_LCD_CMD(GOTO_LINE1_POS0);
  strLCD("DEVICE STATUS");
   WRITE_LCD_CMD(GOTO_LINE2_POS0);
  strLCD("LED: ");
  if((IOPIN0 & (1<<DEVICE_LED)) !=0)
  strLCD("ON");
  else
  strLCD("OFF");
}
 /*newly added start*/
void displayschedule(void)
{
	 WRITE_LCD_CMD(CLEAR_LCD);
  WRITE_LCD_CMD(GOTO_LINE1_POS0);
  strLCD("ON= ");
  WRITE_LCD_DATA(on_hour/10+'0');
  WRITE_LCD_DATA(on_hour%10+'0');
  WRITE_LCD_DATA(':');
  WRITE_LCD_DATA(on_min/10+'0');
  WRITE_LCD_DATA(on_min%10+'0');
  WRITE_LCD_CMD(GOTO_LINE2_POS0);
  strLCD("OFF= ");
  WRITE_LCD_DATA(off_hour/10+'0');
  WRITE_LCD_DATA(off_hour%10+'0');
  WRITE_LCD_DATA(':');
  WRITE_LCD_DATA(off_min/10+'0');
  WRITE_LCD_DATA(off_min%10+'0');
 } 
 /*newly added end*/


/*------------------------------------------------
                    MAIN
------------------------------------------------*/


int main()
{

    u8 key;
 /*newly add start*/
 u32 display_mode=0;
 u32 last_display_sec=0;
 u32 current_sec;
 /*newly add end*/

    INIT_LCD();
    Init_KPM();
    RTC_Init();

      SETBIT(IODIR0,DEVICE_LED);
	  SETBIT(IOCLR0,DEVICE_LED);

   /* SetRTCTimeInfo(15,58,00);
    SetRTCDateInfo(5,9,2026);
    SetRTCDay(MON);
	 */
	  EINT3_Init();
		startupdisplay();

				 /*newly added start*/
				 last_display_sec=(HOUR*3600)+(MIN*60)+SEC;
				 /*newly added end*/

  while(1)
    {
        /*----------------------------------------
              CONFIGURATION REQUEST
        ----------------------------------------*/

		//normaldisplay();   TEMPORARY COMMANT
		/*new added start*/
		current_sec=(HOUR*3600)+(MIN*60)+SEC;
		/*new added end*/
		devicecontrol();
		/*newly added code start*/
		if(current_sec != last_display_sec)
		{
		   last_display_sec=current_sec;

		   if(display_mode==0)
		   {
		   normaldisplay(); // new added
		   //if(SEC % 5 ==0)
		   if(SEC % 5 ==4)
		   {
		  // display_mode++;
			display_mode=1;// new added

		   //if(display_mode>2)
		   //display_mode=0;

		   WRITE_LCD_CMD(CLEAR_LCD);
		   displaydevicestatus();
		   }
		   }
		   // if(display_mode==0)
		   else if(display_mode==1)
		   {
		     if(SEC%5==4)
			 {
			    display_mode=2;
				WRITE_LCD_CMD(CLEAR_LCD);
		   displayschedule();
		      //normaldisplay();
			  }
		}
			  /*else if(display_mode==1)
			  {
			  displaydevicestatus();
			  }
			  else
			  {
			  displayschedule();
			  }
			  }
			  }	*/
			  else
			  {
			      if(SEC%5==4)
				  {
				  display_mode=0;
				  WRITE_LCD_CMD(CLEAR_LCD);
		          normaldisplay();
			  }
			  }
			  }
			  
		/*new added code end*/

        if(config_request)
        {
            config_request=0;

            configmenu();
			 WRITE_LCD_CMD(CLEAR_LCD);		 //comment it

        }
		}


        /*----------------------------------------
              DISPLAY CURRENT RTC
        ----------------------------------------*/

        WRITE_LCD_CMD(GOTO_LINE1_POS0);

        DisplayRTCTime(HOUR,MIN,SEC);

        WRITE_LCD_CMD(GOTO_LINE2_POS0);
        DisplayRTCDate(DOM,MONTH,YEAR);
		   DisplayRTCDay(DOW);

			 devicecontrol();

        /*----------------------------------------
              CHECK KEYPAD
        ----------------------------------------*/

        if(ColScan()==0)
        {
            key=KeyScan();

            if(key=='D')
            {
                SetTimeUsingKeypad();

                SetDateUsingKeypad();

                SetDayUsingKeypad();
            }
        }
		 
    }

			