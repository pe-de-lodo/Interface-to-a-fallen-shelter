#include <RTClib.h>
#include <WString.h>
#include <SimpleSerialShell.h>
#include "main.h"
#include "sleep.h"
#include "visuals.h"

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
    if(argc==2 && strncasecmp(argv[1],"true",5)==0){
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

int commandStop(int argc, char **argv)
{
    setLoopFunc(NULL);
    return 0;
}

extern bool sendVisualsOverUart;
int commandVisuals(int argc, char **argv)
{
    if(argc==2){
        if( strncmp(argv[1], "uart", 4)==0 ){
            Serial.println("enabling uart output");
            sendVisualsOverUart = true;
        }
        else {
            Serial.println("enabling led output");
            sendVisualsOverUart = false;
        }
    }
    else {
        Serial.println("visuals uart|led");
    }
    return 0;
}


int commandPattern(int argc, char **argv)
{
    if(argc==2){
        playPattern(atoi(argv[1]), 500);
    }
    else {
        Serial.println("pattern #");
    }
    return 0;
}

int commandMask(int argc, char **argv)
{
    if(argc==3){
        int offsetLED = atoi(argv[1]);
        int numLED = atoi(argv[2]);
        playMaskedPattern(offsetLED, numLED);
    }
    else {
        Serial.println("mask offset number");
    }
    return 0;
}

void addCommands()
{
    shell.addCommand(F("date"),commandDate);
    shell.addCommand(F("sensors"),commandSensors);
    shell.addCommand(F("sleep"),commandSleep);
    shell.addCommand(F("alarm"),commandAlarm);
    shell.addCommand(F("stop"),commandStop);
    shell.addCommand(F("visuals"),commandVisuals);
    shell.addCommand(F("pattern"),commandPattern);
    shell.addCommand(F("mask"),commandMask);
}