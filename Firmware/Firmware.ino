#include "MatrixDisplay.h"
#include "DisplayToolbox.h"

#define setMaster(dispNum, CSPin) initDisplay(dispNum,CSPin,true)
#define setSlave(dispNum, CSPin) initDisplay(dispNum,CSPin,false)

/****************************
All Firmware options go here!
****************************/
// Uncomment below line if EEPROM available on your board
// #define USE_EEPROM

// If not using EEPROM, you can specify device ID here
#ifndef USE_EEPROM
    const uint8_t deviceID = 0;
#endif

// How many leds in your strip?
#define HEIGHT 8
#define WIDTH 128
#define NUM_LEDS HEIGHT * WIDTH

uint8_t _dispBuf[WIDTH];

MatrixDisplay disp(4,6,7, false); //Matrix controller
DisplayToolbox toolbox(&disp); //Display tools

/***************************
End Firmware options
***************************/

#ifdef USE_EEPROM
    #include <EEPROM.h>
#endif

#define FIRMWARE_VER 3

namespace CMDTYPE
{
    enum CMDTYPE
    {
        SETUP_DATA = 1,
        PIXEL_DATA = 2,
        BRIGHTNESS = 3,
        GETID      = 4,
        SETID      = 5,
        GETVER       = 6
    };
}

namespace RETURN_CODES
{
    enum RETURN_CODES
    {
        SUCCESS = 255,
        REBOOT = 42,
        ERROR = 0,
        ERROR_SIZE = 1,
        ERROR_UNSUPPORTED = 2,
        ERROR_PIXEL_COUNT = 3,
        ERROR_BAD_CMD = 4,
    };
}

typedef struct __attribute__((__packed__))
{
    uint8_t type;
    uint16_t pixelCount;
    uint8_t spiSpeed;
} config_t;

uint16_t numLEDs = NUM_LEDS;
uint8_t bytesPerPixel = 3;


void setup()
{
    Serial.begin(1000000);
    Serial.setTimeout(1000);

    memset(_dispBuf, 0b10101010, WIDTH);

    // Prepare displays
    disp.setMaster(0,5);
    disp.setSlave(1,4);
    disp.setSlave(2,3);
    disp.setSlave(3,2);

    disp.clear();
    disp.syncDisplays();
    toolbox.setBrightness(16);
    UpdateDisplay();
}

void UpdateDisplay()
{
    static uint8_t col;
    static uint8_t row;
    static uint8_t dots;
    for (col=0; col< WIDTH; col++)
    {
        dots = _dispBuf[col];
        for (row=0; row < HEIGHT; row++) {
            if (dots & (128>>row))  // only 8 rows.
                toolbox.setPixel(col, row, 1);
            else
                toolbox.setPixel(col, row, 0);
        }
    }

    disp.syncDisplays();
}

#define EMPTYMAX 100
inline void getData()
{
    static char cmd = 0;
    static uint16_t size = 0;
    static uint16_t count = 0;
    static uint8_t emptyCount = 0;
    static size_t c = 0;
    static uint8_t i = 0;
    static byte curRow = 0;
    static uint16_t packSize = WIDTH;

    if (Serial.available())
    {
        cmd = Serial.read();
        size = 0;
        Serial.readBytes((char*)&size, 2);

        if (cmd == CMDTYPE::PIXEL_DATA)
        {
            count = 0;
            emptyCount = 0;
            curRow = -1;
            memset(_dispBuf, 0, WIDTH);

            if (size == packSize)
            {
                while (count < packSize)
                {
                    c = Serial.readBytes(_dispBuf + count, WIDTH);
                    if (c == 0)
                    {
                        emptyCount++;
                        if(emptyCount > EMPTYMAX) break;
                    }
                    else
                    {
                        emptyCount = 0;
                    }

                    count += c;
                }
            }

            uint8_t resp = RETURN_CODES::SUCCESS;
            if (count == packSize)
            {
                UpdateDisplay();
            }
            else
                resp = RETURN_CODES::ERROR_SIZE;

            Serial.write(resp);
        }
        else if(cmd == CMDTYPE::GETID)
        {
            #ifdef USE_EEPROM
                Serial.write(EEPROM.read(16));
            #else
                Serial.write(deviceID);
            #endif
        }
        else if(cmd == CMDTYPE::SETID)
        {
            if(size != 1)
            {
                Serial.write(RETURN_CODES::ERROR_SIZE);
            }
            else
            {
                uint8_t id = Serial.read();
                #ifdef USE_EEPROM
                    EEPROM.write(16, id);
                #endif
                Serial.write(RETURN_CODES::SUCCESS);
            }
        }
        else if (cmd == CMDTYPE::SETUP_DATA)
        {
            uint8_t result = RETURN_CODES::SUCCESS;
            config_t temp;

            if (size != sizeof(config_t))
            {
                result = RETURN_CODES::ERROR_SIZE;
            }
            else
            {
                size_t read = Serial.readBytes((char*)&temp, sizeof(config_t));
                if (read != size)
                {
                    result = RETURN_CODES::ERROR_SIZE;
                }
                else
                {
                    if(temp.pixelCount / bytesPerPixel != NUM_LEDS)
                        result = RETURN_CODES::ERROR_PIXEL_COUNT;
                }
            }

            Serial.write(result);
        }
        else if (cmd == CMDTYPE::BRIGHTNESS)
        {
            uint8_t result = RETURN_CODES::SUCCESS;
            if (size != 1)
                result = RETURN_CODES::ERROR_SIZE;
            else
            {
                uint8_t brightness = 255;
                size_t read = Serial.readBytes((char*)&brightness, 1);
                if (read != size)
                    result = RETURN_CODES::ERROR_SIZE;
                else
                {
                    toolbox.setBrightness(map(brightness, 0, 255, 0, 16));
                }
            }

            Serial.write(result);
        }
        else if (cmd == CMDTYPE::GETVER)
        {
            Serial.write(RETURN_CODES::SUCCESS);
            Serial.write(FIRMWARE_VER);
        }
        else
        {
            Serial.write(RETURN_CODES::ERROR_BAD_CMD);
        }

        Serial.flush();
    }
}

void loop()
{
    getData();
}
