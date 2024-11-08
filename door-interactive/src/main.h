
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

#define SECTION_FLAG    (0b100000000000000)
#define SECTION_DOOR    (0b000001111100000)
#define SECTION_CRACK_L (0b011110000000000)
#define SECTION_CRACK_R (0b000000000011111)

void setLoopFunc(void loopFunc());
inline bool calcDeltaTime();
void printAllSensorValues();