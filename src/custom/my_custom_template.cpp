/* MIT License - Copyright (c) 2019-2021 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#include "hasplib.h"

#if HASP_USE_CUSTOM > 0 && false // <-- set this to true in your code

#include "custom/my_custom.h"

//custom publish message processing
void publishmessageprocessing(const char* subtopic, const char* payload)
{
    //LOG_TRACE(99, F("%s => %s"), subtopic, payload);
    if(!strcmp_P(subtopic, PSTR("idle"))){
        if(!strcmp_P(payload, PSTR("short"))){
            dispatch_text_line("backlight {state:1,brightness:122}", 99);
        
        } else if(!strcmp_P(payload, PSTR("long"))){
            dispatch_text_line("backlight {state: 0}", 99);
        
        } 

    } else if(!strcmp_P(subtopic, PSTR("p1b1"))){


        if(strlen(payload) > 0) {

            size_t maxsize = (128u * ((strlen(payload) / 128) + 1)) + 128;
            DynamicJsonDocument json(maxsize);

            // Note: Deserialization needs to be (const char *) so the objects WILL be copied
            // this uses more memory but otherwise the mqtt receive buffer can get overwritten by the send buffer !!
            DeserializationError jsonError = deserializeJson(json, payload);
            json.shrinkToFit();

            if(jsonError) { // Couldn't parse incoming JSON command
                dispatch_json_error(99, jsonError);
            } else {

                const char* event_2;
                bool val_2;


                JsonVariant event      = json[F("event")];
                JsonVariant val      = json[F("val")];

                event_2 = event.as<const char*>();
                val_2       = val.as<int>();

                LOG_TRACE(99, F("%d ====> %s"), val_2, event_2);


                }

   
        }
    } //else if(subtopic == strstr_P(subtopic, PSTR("input"))) {}



}


void custom_setup()
{
    // Initialization code here
    randomSeed(millis());
}

void custom_loop()
{
    // Non-blocking code here, this should execute very fast!
}

void custom_every_second()
{
    Serial.print("#");
}

void custom_every_5seconds()
{
    Log.verbose(TAG_CUSTOM, "5 seconds have passsed...");
    dispatch_state_subtopic("my_sensor","{\"test\":123}");
}

bool custom_pin_in_use(uint pin)
{
    if(pin == 1024) return true; // fictuous used pin

    // otherwise the pin is not used
    return false;
}

void custom_get_sensors(JsonDocument& doc)
{
    /* Sensor Name */
    JsonObject sensor = doc.createNestedObject(F("Custom"));

    /* Key-Value pair of the sensor value */
    sensor[F("Random")] = random(256);
}

#endif // HASP_USE_CUSTOM
