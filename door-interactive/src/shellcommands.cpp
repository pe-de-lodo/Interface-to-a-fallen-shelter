#include <RTClib.h>
#include <WString.h>
#include <SimpleSerialShell.h>
#include "main.h"
#include "sleep.h"

extern RTC_DS3231 rtc;

int commandDate(int argc, char **argv)
{

    if(argc==2){
        rtc.adjust(DateTime(argv[1]));
    }
    if(argc==3){
        rtc.adjust(DateTime(argv[1],argv[2]));
    }

    char buf2[] = "hh:mm:ss DD/MM/YY";
    bool noClock = rtc.now().year()==2000;

    if(noClock){
        Serial.print("no time");
    }
    else {
        Serial.print(rtc.now().toString(buf2));
    }

    return 0;
}

int commandAlarm(int argc, char **argv)
{
    DateTime alarm;
    if(argc==2){
        alarm=(DateTime(argv[1]));
    }
    if(argc==3){
        alarm=(DateTime(argv[1],argv[2]));
    }
    if(!alarm.isValid()){
        Serial.println("Invalid Alarm");
        return 1;
    }

    if(!rtc.setAlarm1(
        alarm,
        DS3231_A1_Date
        //DS3231_A1_Second
        )){
            Serial.println("Error, alarm wasn't set!");
        }
        else {
            char buf[] = "hh:mm:ss DD/MM/YY";
            Serial.print("Alarm set for ");
            Serial.println(alarm.toString(buf));
        }

}

void sensorLoop()
{
  printAllSensorValues();
  Serial.print("\r");
}

int commandSensors(int argc, char **argv)
{
    if(argc==2 && strncasecmp(argv[1],"true",88)==0){
        setLoopFunc(sensorLoop);
    }
    printAllSensorValues();
    Serial.println();
    return 0;
}

int commandSleep(int argc, char **argv)
{
    Serial.println("going to sleep");
    delay(1000);
    sleep();
    return 0;
}

void addCommands()
{
    shell.addCommand(F("date"),commandDate);
    shell.addCommand(F("sensors"),commandSensors);
    shell.addCommand(F("sleep"),commandSleep);
    shell.addCommand(F("alarm"),commandAlarm);
    
}