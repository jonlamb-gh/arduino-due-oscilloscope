/**
 * @file oscilloscope.ino
 * @brief Oscilloscope Firmware.
 *
 * Board: 3.3v Due
 * Arduino: 1.8.5
 *
 */

//#define WATCHDOG_THRESHOLD (WDTO_4S)

#define START_BYTE (0xFF)

#ifndef PIN_LED
#define PIN_LED (13)
#endif

static const int AIN_PINS[] = {A0, A1};
//static const int AIN_PINS[] = {A0, A1, A2, A3, A4};
#define AIN_NUM_CHANNELS (sizeof(AIN_PINS)/sizeof(AIN_PINS[0]))
static uint16_t AIN_VALUES[AIN_NUM_CHANNELS];

static void write_channel(
        const uint8_t channel,
        const uint16_t value)
{
    const uint8_t buffer[] =
    {
        START_BYTE,
        channel,
        (uint8_t) (value & 0xFF),
        (uint8_t) ((value >> 8) & 0xFF)
    };

    Serial.write(buffer, sizeof(buffer));
}

void setup(void)
{
    //wdt_disable();
    //wdt_enable(WATCHDOG_THRESHOLD);
    //wdt_reset();

    pinMode(PIN_LED, OUTPUT);

    analogReadResolution(12);

    Serial.begin(115200);

    digitalWrite(PIN_LED, HIGH);
}

void loop(void)
{
    uint8_t idx;

    for(idx = 0; idx < AIN_NUM_CHANNELS; idx += 1)
    {
        AIN_VALUES[idx] = (uint16_t) analogRead(AIN_PINS[idx]);
    }

    for(idx = 0; idx < AIN_NUM_CHANNELS; idx += 1)
    {
        write_channel(idx, AIN_VALUES[idx]);
    }
}
