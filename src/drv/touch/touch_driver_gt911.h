/* MIT License - Copyright (c) 2019-2021 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#ifndef HASP_GT911_TOUCH_DRIVER_H
#define HASP_GT911_TOUCH_DRIVER_H

#ifdef ARDUINO
#include "hasp_conf.h"

#include "Arduino.h"
#include <Wire.h>
#include "Goodix.h"
#include "ArduinoLog.h"

#include "touch_driver.h" // base class

#include "../../hasp/hasp.h" // for hasp_sleep_state
extern uint8_t hasp_sleep_state;

#define INT_PIN (TOUCH_IRQ)
#define RST_PIN (TOUCH_RST) // -1 if pin is connected to VCC else set pin number

static Goodix touch = Goodix();
// static int8_t GT911_num_touches;
// static GTPoint* GT911_points;

// Store touch points into global variable
IRAM_ATTR void GT911_setXY(int8_t contacts, GTPoint* points)
{
    // GT911_num_touches = contacts;
    // GT911_points      = points;

    // LOG_VERBOSE(TAG_GUI, F("Contacts: %d"), contacts);
    // for(int i = 0; i < contacts; i++) {
    //     LOG_VERBOSE(TAG_GUI, F("C%d: #%d %d,%d s:%d"), i, points[i].trackId, points[i].x, points[i].y,
    //     points[i].area); yield();
    // }
}

IRAM_ATTR void touch_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data)
{
    //  LOG_VERBOSE(TAG_GUI, F("Contacts: %d"), GT911_num_touches);
    static GTPoint points[5];

    if(touch.readInput((uint8_t*)&points) > 0) {

        if(hasp_sleep_state != HASP_SLEEP_OFF) hasp_update_sleep_state(); // update Idle

        data->point.x = points[0].x;
        data->point.y = points[0].y;
        data->state   = LV_INDEV_STATE_PR;

    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    touch.loop(); // reset IRQ
}

namespace dev {

class TouchGt911 : public BaseTouch {

  public:
    IRAM_ATTR void read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data)
    {
        touch_read(indev_driver, data);
    }

    void init(int w, int h)
    {
        Wire.begin(TOUCH_SDA, TOUCH_SCL, I2C_TOUCH_FREQUENCY);
        delay(300);

        touch.setHandler(GT911_setXY);

        if(touch.begin(INT_PIN, RST_PIN)) {
            LOG_VERBOSE(TAG_DRVR, F("Goodix GT911 reset OK"));
        } else {
            LOG_WARNING(TAG_DRVR, F("Goodix GT911 reset failed"));
        }

        Serial.print("Check ACK on addr request on 0x");
        //     Serial.print(touch.i2cAddr, HEX);

        //   Wire.beginTransmission(touch.i2cAddr);
        int error;
        // = Wire.endTransmission();
        if(error == 0) {
            //     Serial.println(": SUCCESS");
        } else {
            //     Serial.print(": ERROR #");
            //     Serial.println(error);
        }

        LOG_INFO(TAG_DRVR, F("Goodix GT911 touch driver started"));
    }
};

} // namespace dev

using dev::TouchGt911;
extern dev::TouchGt911 haspTouch;

#endif // ARDUINO

#endif // HASP_GT911_TOUCH_DRIVER_H
