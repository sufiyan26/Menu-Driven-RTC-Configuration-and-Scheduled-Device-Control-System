#ifndef __LCD1_DEFINES_H__
#define __LCD1_DEFINES_H__

#define LCD_DATA 8
#define LCD_RS   16
#define LCD_EN   18

#define CLEAR_LCD            0X01
#define RET_CUR_HOME         0X02

#define MODE_8BIT_1LINE      0X30
#define MODE_8BIT_2LINE      0X38

#define MODE_4BIT_1LINE      0X20
#define MODE_4BIT_2LINE      0X28

#define DISP_OFF             0X08
#define DISP_ON_CUR_OFF      0X0C
#define DISP_ON_CUR_ON       0X0E
#define DISP_ON_CUR_BLINK    0X0F

#define SHIFT_CUR_RIGHT      0X06

#define GOTO_LINE1_POS0      0X80
#define GOTO_LINE2_POS0      0XC0

#define GOTO_CGRAM           0X40

#endif
