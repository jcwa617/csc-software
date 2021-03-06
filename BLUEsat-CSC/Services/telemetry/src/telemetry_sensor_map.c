#include "FreeRTOS.h"
#include "telemetry_sensor_map.h"

char telemetry_sensor_map[4][10][8] = {
        {{0x10, 0x85, 0x7D, 0x84, 0x02, 0x08, 0x00, 0x95}, /* TX */
         {0x10, 0x97, 0x95, 0x84, 0x02, 0x08, 0x00, 0x44},
         {0x10, 0x44, 0x1F, 0x82, 0x02, 0x08, 0x00, 0xA6},
         {0x10, 0xA1, 0x97, 0x84, 0x02, 0x08, 0x00, 0x98},
         {0x10, 0xBA, 0x79, 0x84, 0x02, 0x08, 0x00, 0x43},
         {0x10, 0x7A, 0x7C, 0x84, 0x02, 0x08, 0x00, 0x0E},
         {0x10, 0x00, 0x96, 0x2C, 0x02, 0x08, 0x00, 0x8B},
         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

        {{0x10, 0x40, 0x79, 0x84, 0x02, 0x08, 0x00, 0xFE}, /* Battery */
         {0x10, 0x6F, 0x88, 0x84, 0x02, 0x08, 0x00, 0x07},
         {0x10, 0xAE, 0x7A, 0x84, 0x02, 0x08, 0x00, 0x8A},
         {0x10, 0x77, 0x90, 0x84, 0x02, 0x08, 0x00, 0xBF},
         {0x10, 0xB9, 0x7D, 0x84, 0x02, 0x08, 0x00, 0x05},
         {0x10, 0x13, 0x83, 0x84, 0x02, 0x08, 0x00, 0x92},
         {0x10, 0x27, 0x87, 0x84, 0x02, 0x08, 0x00, 0xBC},
         {0x10, 0x96, 0x7A, 0x84, 0x02, 0x08, 0x00, 0xC6},
         {0x10, 0xDE, 0x7B, 0x2C, 0x02, 0x08, 0x00, 0x6A},
         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

        {{0x10, 0x6A, 0x96, 0x84, 0x02, 0x08, 0x00, 0x32}, /* CSC */
         {0x10, 0x4D, 0x8B, 0x84, 0x02, 0x08, 0x00, 0x91},
         {0x10, 0xFE, 0x81, 0x84, 0x02, 0x08, 0x00, 0x72},
         {0x10, 0xE8, 0x95, 0x84, 0x02, 0x08, 0x00, 0xF8},
         {0x10, 0xB9, 0x40, 0x82, 0x02, 0x08, 0x00, 0x64},
         {0x10, 0x80, 0x60, 0x81, 0x02, 0x08, 0x00, 0x6F},
         {0x10, 0x6E, 0x7E, 0x84, 0x02, 0x08, 0x00, 0x0A},
         {0x10, 0xC3, 0x7E, 0x84, 0x02, 0x08, 0x00, 0x1C},
         {0x10, 0x74, 0xFF, 0x81, 0x02, 0x08, 0x00, 0x19},
         {0x10, 0xF8, 0x46, 0x92, 0x02, 0x08, 0x00, 0x82}},

        {{0x10, 0xC1, 0x96, 0x9B, 0x02, 0x08, 0x00, 0x34}, /* RX */
         {0x10, 0x6E, 0x99, 0x9B, 0x02, 0x08, 0x00, 0x23},
         {0x10, 0xD9, 0x91, 0x9B, 0x02, 0x08, 0x00, 0x9F},
         {0x10, 0x88, 0x83, 0x9B, 0x02, 0x08, 0x00, 0x79},
         {0x10 ,0xBE, 0x8E, 0x9B, 0x02, 0x08, 0x00, 0xCA},
         {0x10, 0xA3, 0x82, 0x9B, 0x02, 0x08, 0x00, 0xFA},
         {0x10, 0x9F, 0x84, 0x9B, 0x02, 0x08, 0x00, 0xF6},
         {0x10, 0x29, 0xAA, 0x9B, 0x02, 0x08, 0x00, 0x19},
         {0x10, 0x60, 0xC1, 0x2C, 0x02, 0x08, 0x00, 0x8C},
         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
};

