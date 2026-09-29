# Menu-Driven-RTC-Configuration-and-Scheduled-Device-Control-System
Menu-driven RTC configuration and scheduled device control system using LPC2148, 16x2 LCD, 4x4 keypad, EINT3 switch, and RTC-based ON/OFF scheduling.

# Overview

This project is a menu-driven RTC configuration and scheduled device
control system developed using the LPC2148 ARM7 microcontroller.

The system uses an RTC to maintain the current time, date and day.
A 16x2 LCD is used for display, while a 4x4 keypad is used to
configure the RTC and device scheduling.

A configuration switch connected through EINT3 is used to enter the
configuration menu.

The device LED is automatically controlled according to the programmed
ON and OFF schedule.

# Features

- RTC-based timekeeping using LPC2148
- Display of current time, date and day
- Time configuration using 4x4 keypad
- Date configuration using 4x4 keypad
- Day configuration using 4x4 keypad
- Device ON/OFF scheduling
- Automatic device control based on RTC time
- Midnight-crossing schedule support
- Invalid time validation
- Date and month validation
- Leap-year validation
- Rejection of identical ON and OFF times
- EINT3-based configuration request
- LCD display of device status
- LCD display of programmed ON/OFF schedule
- RTC retention during reset and power-off using the RTC backup supply



# Hardware Used

- LPC2148 ARM7 development board
- 16x2 LCD
- 4x4 matrix keypad
- Configuration switch
- LED used as the controlled device
- RTC backup battery



 Software Used

- Keil µVision
- Embedded C
- LPC214x device header files



# Pin Configuration

# LCD

| LCD Signal | LPC2148 Pin |
|------------|-------------|
| D0-D7 | P0.8-P0.15 |
| RS | P0.16 |
| EN | P0.18 |

# 4x4 Keypad

| Keypad Signal | LPC2148 Pin |
|---------------|-------------|
| Rows | P1.16-P1.19 |
| Columns | P1.20-P1.23 |

# Configuration Switch

| Function | LPC2148 Pin |
|----------|-------------|
| EINT3 | P0.20 |

# Device LED

| Device | LPC2148 Pin |
|--------|-------------|
| LED | P0.7 |


# System Operation

After power-up, the LPC2148 initializes the LCD, keypad, RTC,
device output and EINT3.

The system then displays the current RTC information.

The LCD display cycles through:

1. Current time, date and day
2. Device ON/OFF status
3. Programmed ON/OFF schedule

The display cycle repeats continuously.

The RTC seconds continue to update while the current time screen
is displayed.

# Configuration Menu

The configuration switch activates the configuration menu.

The menu provides the following options:

1. Time
2. Date
3. Day
4. Schedule
D. Exit

# RTC Configuration

The user can configure the following RTC parameters using the keypad:

- Hour
- Minute
- Second
- Date
- Month
- Year
- Day

The entered values are validated before updating the RTC.

# Schedule Configuration

The user can configure:

- Device ON hour
- Device ON minute
- Device OFF hour
- Device OFF minute

The controller continuously compares the current RTC time with the
programmed schedule.

If the current time is within the scheduled period, the device is
switched ON. Otherwise, the device is switched OFF.

Schedules crossing midnight are also supported.

# Validation

The system performs validation for:

- Hour: 0-23
- Minute: 0-59
- Second: 0-59
- Month: 1-12
- Valid number of days for each month
- Leap-year February dates
- Day: 1-7
- ON and OFF times must not be identical

# LCD Display Sequence

1. RTC Information

```text
11:16:25
24/09/2026 THUR

The seconds continue updating while the RTC screen is displayed.

2. Device Status
DEVICE STATUS
LED: ON

or

DEVICE STATUS
LED: OFF
3. Schedule
ON= 10:30
OFF= 18:30

The three display screens repeat continuously.



