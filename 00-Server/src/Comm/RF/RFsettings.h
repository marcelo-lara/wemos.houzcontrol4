#pragma once

// RF channels /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define rfChannel      0x5B   
#define rf_server_tx   0xA0
#define rf_server_rx   0xB0
#define rf_office_tx   0xA1
#define rf_office_rx   0xB1
#define rf_suite_tx    0xA2
#define rf_suite_rx    0xB2
#define rf_living_tx   0xA3
#define rf_living_rx   0xB3
#define rf_wemos_tx    0xA4
#define rf_wemos_rx    0xB4

// commands
#define RFCMD_QUERY    0xA
#define RFCMD_VALUE    0xB
#define RFCMD_SET      0xC
#define RFCMD_EVENT    0xD
#define RFCMD_STATUS   0xE

