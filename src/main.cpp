#include <knx.h>
#include <Wire.h>
#include "hardware.h"
#include "wiring_private.h" // pinPeripheral() function
#include "Enocean.h"

#include "EnOceanHandle.h"

// XIAO PINS   RX = D3  TX = D2
Uart Serial2(&sercom2, 3, 2, SERCOM_RX_PAD_3, UART_TX_PAD_2);

void SERCOM2_Handler()
{
    Serial2.IrqHandler();
}

EnOceanDevice device[MAX_NUMBER_OF_DEVICES] = {EnOceanDevice()};

void appSetup();
void appLoop();

void setup()
{

    pinMode(PROG_LED_PIN, OUTPUT);
    digitalWrite(PROG_LED_PIN, HIGH);
    //delay(5000);
    digitalWrite(PROG_LED_PIN, LOW);
#ifdef KDEBUG_min
    SERIAL_PORT.begin(115200);
    SERIAL_PORT.println("Startup called...");
    ArduinoPlatform::SerialDebug = &SERIAL_PORT;
#endif

#ifdef LED_YELLOW_PIN
    pinMode(LED_YELLOW_PIN, OUTPUT);
    digitalWrite(LED_YELLOW_PIN, HIGH);
#endif

    // moved to checkBoard!!!
    // Wire.begin();
    knx.readMemory();

    // pin or GPIO the programming led is connected to. Default is LED_BUILDIN
    knx.ledPin(PROG_LED_PIN);
    // is the led active on HIGH or low? Default is LOW
    knx.ledPinActiveOn(PROG_LED_PIN_ACTIVE_ON);
    // pin or GPIO programming button is connected to. Default is 0
    knx.buttonPin(PROG_BUTTON_PIN);
    // Is the interrup created in RISING or FALLING signal? Default is RISING
    knx.buttonPinInterruptOn(PROG_BUTTON_PIN_INTERRUPT_ON);

    // print values of parameters if device is already configured
    appSetup();

    // start the framework.
    knx.start();

    // start Enocean
    for (int i = 0; i < MAX_NUMBER_OF_DEVICES; i++)
    {
        enOcean.configureDevice(device[i], i);
    }

    Serial2.begin(57600); // Change to Serial wenn original Platine
                          // Assign pins 2 & 3 SERCOM functionality
    pinPeripheral(2, PIO_SERCOM_ALT);
    pinPeripheral(3, PIO_SERCOM_ALT);

    enOcean.initSerial(Serial2);
    enOcean.init();

#ifdef  KDEBUG_min
    if (enOcean.getNumberDevices() != MAX_NUMBER_OF_DEVICES)
        SERIAL_PORT.println(F("!!! NUMBER OF DEVICES != MAX DEVICES -> change!!!"));
    else
        SERIAL_PORT.println(F("Ready for normal operation"));
#endif

#ifdef LED_YELLOW_PIN
    digitalWrite(LED_YELLOW_PIN, LOW);
#endif
}

void loop()
{
    // don't delay here to much. Otherwise you might lose packages or mess up the timing with ETS
    knx.loop();

    // only run the application code if the device was configured with ETS
    if (knx.configured())
        appLoop();

}