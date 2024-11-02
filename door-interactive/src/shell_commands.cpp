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
    bool alarmSet=false;
    if(argc==2){
        alarm=(DateTime(argv[1]));
        alarmSet=true;
    }
    else if(argc==3){
        alarm=(DateTime(argv[1],argv[2]));
        alarmSet=true;
    }
    else {
        alarm=rtc.getAlarm1();
    }
    if(!alarm.isValid()){
        Serial.println("Invalid Alarm");
        return 1;
    }

    if(alarmSet && !rtc.setAlarm1(
        alarm,
        DS3231_A1_Date
    )){
        Serial.println("Error, alarm wasn't set!");
        return 1;
    }
        
    char buf[] = "hh:mm:ss DD/MM/YY";
    Serial.print("Alarm set for ");
    Serial.println(alarm.toString(buf));
    

}

int32_t sensorSamplesToTake = -1;
void sensorLoop()
{
  if(sensorSamplesToTake==0){
    return;
  }
  if(sensorSamplesToTake>0){
    sensorSamplesToTake--;
  }

  printAllSensorValues();
  Serial.print("\r");
}

extern int32_t sensorSamplesToTake;
int commandSensors(int argc, char **argv)
{
    sensorSamplesToTake = 1;
    if(argc==2){
        sensorSamplesToTake=strtol(argv[1],0,10);
    }    
    setLoopFunc(sensorLoop);
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

extern bool wokeFromAlarm;
int commandBootInfo(int arc,char **argv)
{
    if(wokeFromAlarm){
        Serial.println("Woke from alarm");        
    }
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

void addCommands()
{
    shell.addCommand(F("date"),commandDate);
    shell.addCommand(F("sensors"),commandSensors);
    shell.addCommand(F("sleep"),commandSleep);
    shell.addCommand(F("alarm"),commandAlarm);
    shell.addCommand(F("stop"),commandStop);
    shell.addCommand(F("visuals"),commandVisuals);
    shell.addCommand(F("bootinfo"),commandBootInfo);
}