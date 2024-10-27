#include <RTClib.h>
#include "main.h"
#include "visuals.h"
#include "sections/knockdetection.h"
#include "sleep.h"
RTC_DS3231 rtc;

struct Time{
    int hour;
    int minutes;
};

struct Time alarmTimes[] = {
        {19,00},
        {20,00},
        {21,00}
};

void printDateTime()
{
    char buf2[] = "hh:mm:ss DD/MM/YY";
    bool noClock = rtc.now().year()==2000;
    if(noClock){
        Serial.print("no time");
    }
    else {
        Serial.print(rtc.now().toString(buf2));
    }
}

void printDateTime(DateTime time)
{
    char buf2[] = "hh:mm:ss DD/MM/YY";
    bool noClock = time==2000;
    if(noClock){
        Serial.print("no time");
    }
    else {
        Serial.print(time.toString(buf2));
    }
}

bool initWakeAlarm()
{
    bool wokeFromAlarm = digitalRead( ALARM_PIN ) == LOW;

    // initializing the rtc
    if(!rtc.begin(&Wire)) {
        Serial.println("??? Couldn't find RTC! ???");
        return wokeFromAlarm;
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
    
    
    int alarmTimeIndex = 0;
    DateTime now = rtc.now() + TimeSpan(60*10);
    DateTime nextAlarm(now.year(),now.month(),now.day(),alarmTimes[alarmTimeIndex].hour,alarmTimes[alarmTimeIndex].minutes);

    int len = sizeof(alarmTimes)/sizeof(Time);
    while(alarmTimeIndex<len && nextAlarm<now){
        nextAlarm = DateTime(now.year(),now.month(),now.day(),alarmTimes[alarmTimeIndex].hour,alarmTimes[alarmTimeIndex].minutes);
        alarmTimeIndex++;
    }
    if(nextAlarm<now){
        nextAlarm = DateTime(now.year(),now.month(),now.day(),alarmTimes[0].hour,alarmTimes[0].minutes);
    }


    if(!rtc.setAlarm1(
            nextAlarm,
            DS3231_A1_Second // this mode triggers the alarm when the seconds match. See Doxygen for other options
    )) {
        Serial.println("Error, alarm wasn't set!");
    }else {
        Serial.println("Alarm will happen at ");
        printDateTime(nextAlarm);
        Serial.println(" seconds!");
    }

    
    char buf2[] = "YYMMDD-hh:mm:ss";
    Serial.println(rtc.now().toString(buf2));

    return wokeFromAlarm;
}


uint32_t alarmElapsed;
extern uint32_t deltaTime;
void initAlarmAttractor()
{
    Serial.println("playing alarm attractor");

    alarmElapsed = 0;
    setLoopFunc(loopAlarmAttractor);
    playPatternAlarmAttractorPattern();
}

void loopAlarmAttractor()
{
    // leds[0] = ColorFromPalette(alarmAttractorGradient,(uint8_t)(millis()*3*255/1000));
    // FastLED.show();
    alarmElapsed+=deltaTime; 
    if(digitalRead(WAKEUP_PIN)==HIGH){
        setLoopFunc(initKnock);
        return;
    }
    if(alarmElapsed>30000){
        sleep();
    }
}