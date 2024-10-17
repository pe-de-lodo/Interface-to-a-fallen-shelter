#include <RTClib.h>
RTC_DS3231 rtc;

void initWakeAtTime()
{

    Wire.setPins(SDA,SCL);
    Wire.begin();
    while(false){
        Wire.requestFrom(0x68, 1);     
        while(Wire.available())    // secondary may send less than requested
        { 
            char c = Wire.read(); // receive a byte as character
            Serial.print(c);         // print the character
        }
        delay(1000);
    }
    // initializing the rtc
    if(!rtc.begin(&Wire)) {
        Serial.println("Couldn't find RTC!");
        Serial.flush(); 
        while (1) delay(10);
    }

    if(rtc.lostPower()) {
        // this will adjust to the date and time at compilation
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

     rtc.disable32K();

    rtc.clearAlarm(1);
    rtc.clearAlarm(2);

    if(!rtc.setAlarm1(
            rtc.now() + TimeSpan(60),
            DS3231_A1_Second // this mode triggers the alarm when the seconds match. See Doxygen for other options
    )) {
        Serial.println("Error, alarm wasn't set!");
    }else {
        Serial.println("Alarm will happen in 10 seconds!");
    }
}