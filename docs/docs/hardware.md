# Hardware Connections

# Microcontroller

The project is implemented using the LPC2148 ARM7 microcontroller.

# LCD Interface

A 16x2 LCD is used to display the RTC information, device status,
configuration messages and scheduled ON/OFF times.

| LCD Signal | LPC2148 Pin |
|------------|-------------|
| D0 | P0.8 |
| D1 | P0.9 |
| D2 | P0.10 |
| D3 | P0.11 |
| D4 | P0.12 |
| D5 | P0.13 |
| D6 | P0.14 |
| D7 | P0.15 |
| RS | P0.16 |
| EN | P0.18 |

# 4x4 Keypad Interface

The 4x4 matrix keypad is used for RTC configuration and schedule
configuration.

| Keypad Signal | LPC2148 Pin |
|---------------|-------------|
| Row 1 | P1.16 |
| Row 2 | P1.17 |
| Row 3 | P1.18 |
| Row 4 | P1.19 |
| Column 1 | P1.20 |
| Column 2 | P1.21 |
| Column 3 | P1.22 |
| Column 4 | P1.23 |

# Configuration Switch

An external switch connected to EINT3 is used to request the
configuration menu.

| Function | LPC2148 Pin |
|----------|-------------|
| EINT3 | P0.20 |

The EINT3 interrupt is configured for a falling-edge trigger.

# Device LED

An LED connected to P0.7 is used as the controlled device in this
project.

| Device | LPC2148 Pin |
|--------|-------------|
| Device LED | P0.7 |

The LED is controlled automatically according to the programmed
ON/OFF schedule.

# RTC

The internal RTC of the LPC2148 is used to maintain:

- Hours
- Minutes
- Seconds
- Date
- Month
- Year
- Day

The RTC backup supply allows the RTC to continue operating when the
main board power is removed.

# Hardware Block Diagram

                    +----------------------+
                    |       LPC2148        |
                    |       ARM7 MCU       |
                    |                      |
                    |      Internal RTC    |
                    +----------+-----------+
                               |
             +-----------------+-----------------+
             |                 |                 |
             v                 v                 v
         16x2 LCD          4x4 Keypad        EINT3 Switch
       P0.8-P0.15         P1.16-P1.23          P0.20
        P0.16 RS
        P0.18 EN
             |
             |
             v
        Display Time,
        Date, Day,
        Status & Schedule

                    LPC2148
                       |
                       v
                  Device LED
                    P0.7
                       |
                       v
              Scheduled ON/OFF
Main Hardware Functions
Hardware	                              Function
LPC2148	                            Main controller
RTC	                             Time and date keeping
LCD	                                    Display
4x4                                 Keypad User input
EINT3 Switch	                   Configuration request
LED	                               Controlled device
RTC Backup Supply	          Maintains RTC during main power-off
