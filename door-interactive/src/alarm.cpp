#include <RTClib.h>
#include <main.h>
RTC_DS3231 rtc;

bool initWakeAlarm()
{
    pinMode(ALARM_PIN,INPUT_PULLUP);
    bool wokeFromAlarm = digitalRead( ALARM_PIN ) == LOW;

    // initializing the rtc
    if(!rtc.begin(&Wire)) {
        Serial.println("Couldn't find RTC!");
        Serial.flush(); 
        while (1) delay(10);
    }

    if(rtc.lostPower()) {
        Serial.println("Settings date time");
        // this will adjust to the date and time at compilation
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    rtc.disable32K();

    rtc.clearAlarm(1);
    rtc.clearAlarm(2);

    rtc.writeSqwPinMode(DS3231_OFF);
    rtc.disableAlarm(2);
    

    if(!rtc.setAlarm1(
            rtc.now() + TimeSpan(60),
            DS3231_A1_Second // this mode triggers the alarm when the seconds match. See Doxygen for other options
    )) {
        Serial.println("Error, alarm wasn't set!");
    }else {
        Serial.println("Alarm will happen in 60 seconds!");
    }

    
    char buf2[] = "YYMMDD-hh:mm:ss";
    Serial.println(rtc.now().toString(buf2));

    return wokeFromAlarm;
}