#include <RTClib.h>
#include <WString.h>
#include <SimpleSerialShell.h>

extern RTC_DS3231 rtc;

int commandDate(int argc, char **argv)
{
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

void addCommands()
{
    shell.addCommand(F("date"),commandDate);    
}