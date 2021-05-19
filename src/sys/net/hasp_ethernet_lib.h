/* MIT License - Copyright (c) 2019-2021 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#ifndef HASP_ETHERNET_LIB_H
#define HASP_ETHERNET_LIB_H

#include <SPI.h>
#include "Ethernet.h"
#include "EthernetUdp.h"

#include "ArduinoJson.h"

static bool eth_connected = false;

void ethernetSetup();
void ethernetLoop(void);

bool ethernetEverySecond();
bool ethernetEvery5Seconds();
void ethernet_get_statusupdate(char* buffer, size_t len);

void ethernet_get_info(JsonDocument& doc);

#endif