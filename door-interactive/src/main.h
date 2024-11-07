
#define LED_PIN_0 0
#define LED_PIN_1 1
#define KNOCK_PIN A2
#define LIGHT_SENSOR_PIN A3
#define ALARM_PIN 6
#define MOSFET_PIN 7
#define DOOR_KNOB 8
#define WAKEUP_PIN 9
#define KEY_SENSOR 10
#define NUM_LEDS_0 235
#define NUM_LEDS_1 239
#define NUM_LEDS (NUM_LEDS_0+NUM_LEDS_1)

#define INTERACTION_TIMEOUT 30000
#define FINALE_DURATION 20000

#define SECTION_DOOR (1<<0)
#define SECTION_CRACK_L (1<<1)
#define SECTION_CRACK_R (1<<2)

void setLoopFunc(void loopFunc());
inline bool calcDeltaTime();
void printAllSensorValues();