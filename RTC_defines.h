#ifndef __RTC_H
#define __RTC_H

#include <lpc214x.h>
#include "types.h"
#include "LCD1_defines.h"

#define CPU_LPC2148

void WRITE_LCD_CMD(u8 CMD);
void WRITE_LCD_DATA(u8 ascii);
void INIT_LCD(void);
void strLCD(s8 *str);
void U32LCD(u32 n);


/*------------------------------------------------
                 RTC CLOCK VALUES
------------------------------------------------*/

#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)

#define PREINT_VAL  ((int)(PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))


/*------------------------------------------------
                  RTC CONTROL BITS
------------------------------------------------*/

#define RTC_ENABLE  (1<<0)
#define RTC_RESET   (1<<1)
#define RTC_CLKSRC  (1<<4)


/*------------------------------------------------
                   DAYS OF WEEK
------------------------------------------------*/

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6


/*------------------------------------------------
                RTC VARIABLES
------------------------------------------------*/

s32 hour,min,sec;
s32 date,month,year;
s32 day;

s8 week[][4] =
{
    "SUN",
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"
};


/*------------------------------------------------
                   RTC INITIALIZE
------------------------------------------------*/

void RTC_Init(void)
{
    /*CCR=RTC_RESET;
   #ifdef CPU_LPC2148
    CCR=RTC_ENABLE|RTC_CLKSRC;
	  */
	  #ifdef CPU_LPC2148 
	  CCR=RTC_ENABLE|RTC_CLKSRC;
#else

    PREINT=PREINT_VAL;
    PREFRAC=PREFRAC_VAL;

    CCR=RTC_ENABLE;

#endif
}


/*------------------------------------------------
                  SET RTC TIME
------------------------------------------------*/

void SetRTCTimeInfo(u32 hour,u32 minute,u32 second)
{
    HOUR=hour;
    MIN=minute;
    SEC=second;
}


/*------------------------------------------------
                  GET RTC TIME
------------------------------------------------*/

void GetRTCTimeInfo(s32 *hour,s32 *minute,s32 *second)
{
    *hour=HOUR;
    *minute=MIN;
    *second=SEC;
}


/*------------------------------------------------
                DISPLAY RTC TIME
------------------------------------------------*/

void DisplayRTCTime(u32 hour,u32 minute,u32 second)
{
    WRITE_LCD_CMD(GOTO_LINE1_POS0);

    WRITE_LCD_DATA(hour/10+48);
    WRITE_LCD_DATA(hour%10+48);

    WRITE_LCD_DATA(':');

    WRITE_LCD_DATA(minute/10+48);
    WRITE_LCD_DATA(minute%10+48);

    WRITE_LCD_DATA(':');

    WRITE_LCD_DATA(second/10+48);
    WRITE_LCD_DATA(second%10+48);
}


/*------------------------------------------------
                  SET RTC DATE
------------------------------------------------*/

void SetRTCDateInfo(u32 date,u32 month,u32 year)
{
    DOM=date;
    MONTH=month;
    YEAR=year;
}


/*------------------------------------------------
                  GET RTC DATE
------------------------------------------------*/

void GetRTCDateInfo(s32 *date,s32 *month,s32 *year)
{
    *date=DOM;
    *month=MONTH;
    *year=YEAR;
}


/*------------------------------------------------
                DISPLAY RTC DATE
------------------------------------------------*/

void DisplayRTCDate(u32 date,u32 month,u32 year)
{
    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    WRITE_LCD_DATA(date/10+48);
    WRITE_LCD_DATA(date%10+48);

    WRITE_LCD_DATA('/');

    WRITE_LCD_DATA(month/10+48);
    WRITE_LCD_DATA(month%10+48);

    WRITE_LCD_DATA('/');

    U32LCD(year);
}


/*------------------------------------------------
                  SET RTC DAY
------------------------------------------------*/

void SetRTCDay(u32 dow)
{
    DOW=dow;
}


/*------------------------------------------------
                  GET RTC DAY
------------------------------------------------*/

void GetRTCDay(s32 *dow)
{
    *dow=DOW;
}


/*------------------------------------------------
                DISPLAY RTC DAY
------------------------------------------------*/

void DisplayRTCDay(u32 day)
{
    WRITE_LCD_CMD(GOTO_LINE1_POS0+10);

    strLCD(week[day]);
}

#endif
