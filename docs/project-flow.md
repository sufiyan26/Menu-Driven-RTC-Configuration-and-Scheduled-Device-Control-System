# Project Flow

# 1. Overall System Flow

                         START
                           |
                           v
                  Initialize LCD
                           |
                           v
                 Initialize Keypad
                           |
                           v
                   Initialize RTC
                           |
                           v
                  Initialize EINT3
                           |
                           v
                    Startup Display
                           |
                           v
                 Display Current RTC
                           |
                           v
                      Main Loop
                           |
             +-------------+-------------+
             |                           |
             v                           v
      Check Configuration          Control Device
          Request                  Using Schedule
             |                           |
             v                           |
      Request Present?                   |
        /          \                     |
      YES           NO                   |
       |             |                   |
       v             |                   |
Configuration       |                    |
    Menu            |                    |
       |             |                   |
       v             |                   |
  +----+----+----+   |                   |
  |    |    |    |   |                   |
  v    v    v    v   |                   |
Time Date Day Schedule                   |
  |    |    |    |   |                   |
  +----+----+----+   |                   |
             |       |                   |
             +-------+-------------------+
                     |
                     v
                Repeat Loop


2. Configuration Menu Flow

             EINT3 Switch Pressed
                     |
                     v
             Configuration Menu
                     |
        +------------+------------+
        |            |            |
        v            v            v
     1. Time      2. Date      3. Day
        |            |            |
        +------------+------------+
                     |
                     v
                4. Schedule
                     |
                     v
                Return to Main

 3. RTC Time Configuration
              Select Time
                   |
                   v
             Enter HH:MM:SS
                   |
                   v
                Validate
                   |
            +------+------+
            |             |
          Valid         Invalid
            |             |
            v             v
         Set RTC      Show Error
            |          Message
            |             |
            +------+------+
                   |
                   v
              Return to Main

  4. Date Configuration
              Select Date
                   |
                   v
            Enter DD/MM/YYYY
                   |
                   v
         Check Month and Day
                   |
                   v
             Check Leap Year
                   |
            +------+------+
            |             |
          Valid         Invalid
            |             |
            v             v
        Set RTC Date   Show Error
                         Message
            |             |
            +------+------+
                   |
                   v
              Return to Main

5. Day Configuration
               Select Day
                   |
                   v
              Enter Day
                   |
                   v
               Validate
                   |
            +------+------+
            |             |
          Valid         Invalid
            |             |
            v             v
        Set RTC Day    Show Error
                         Message
            |             |
            +------+------+
                   |
                   v
              Return to Main

                     6. Schedule Configuration
             Select Schedule
                    |
                    v
              Enter ON Time
                    |
                    v
             Enter OFF Time
                    |
                    v
               Validate Time
                    |
             +------+------+
             |             |
           Valid         Invalid
             |             |
             v             v
      Compare ON/OFF    Show Error
          Times           Message
             |
       +-----+-----+
       |           |
      Same      Different
       |           |
       v           v
    Reject      Store Schedule
    Schedule        |
       |            |
       +-----+------+
             |
             v
        Return to Main
   
   6. Schedule Configuration
             Select Schedule
                    |
                    v
              Enter ON Time
                    |
                    v
             Enter OFF Time
                    |
                    v
               Validate Time
                    |
             +------+------+
             |             |
           Valid         Invalid
             |             |
             v             v
      Compare ON/OFF    Show Error
          Times           Message
             |
       +-----+-----+
       |           |
      Same      Different
       |           |
       v           v
    Reject      Store Schedule
    Schedule        |
       |            |
       +-----+------+
             |
             v
        Return to Main

      7. Device Control Flow
              Read RTC Time
                    |
                    v
          Calculate Current Time
                    |
                    v
          Compare With Schedule
                    |
             +------+------+
             |             |
          Within         Outside
         Schedule        Schedule
             |             |
             v             v
         Device ON     Device OFF
             |             |
             +------+------+
                    |
                    v
                Repeat
8. LCD Display Flow
              Current RTC
           Time / Date / Day
                    |
                 5 seconds
                    |
                    v
             Device Status
              LED ON/OFF
                    |
                 5 seconds
                    |
                    v
            Scheduled Time
               ON / OFF
                    |
                 5 seconds
                    |
                    v
              Current RTC
                    |
                    v
                  Repeat
