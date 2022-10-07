#ifndef COMMON_H
#define COMMON_H

#include "esp_ao.h"
#include "esp_ao_evt.h"

typedef Signal IHMSignal;

enum IHMSignal {
    IHM_BUTTON_CONTEXT_PRESSED = USER_SIG,
    IHM_BUTTON_POT_PLUS_PRESSED,
    IHM_BUTTON_POT_MINUS_PRESSED,
    IHM_BUTTON_CIL_PLUS_PRESSED,
    IHM_BUTTON_CIL_MINUS_PRESSED,
    IHM_BUTTON_TURB_PLUS_PRESSED,
    IHM_BUTTON_TURB_MINUS_PRESSED,
};

extern Active *AO_ihmManager;

#endif