#ifndef ESP_AO_EVT_H
#define ESP_AO_EVT_H

#include <stdint.h>

/* Event facilities.. */
typedef uint32_t Signal; /* event signal */

enum ReservedSignals {
    INIT_SIG, /* dispatched to AO before entering event-loop */
    USER_SIG  /* first signal available to the users */
};

/* Event base class */
typedef struct {
    Signal sig; /* event signal */
    /* event parameters added in subclasses of Event */
} Event;

#endif