#ifndef HSM_H
#define HSM_H

#include "esp_hsm.h"

// IhmManager HSM Events
#define HSME_PWR        (HSME_START)
#define HSME_RELEASE    (HSME_START + 1)
#define HSME_MODE       (HSME_START + 2)
#define HSME_LOWBATT    (HSME_START + 3)

typedef struct IhmHSMTag
{
    HSM parent;
    // Child members
    char param1;
    char param2;
} IhmHSM;

HSM_EVENT IHM_StateOffHndlr(HSM *This, HSM_EVENT event, void *param);
HSM_EVENT IHM_StateOnHndlr(HSM *This, HSM_EVENT event, void *param);
HSM_EVENT IHM_StateOnShootHndlr(HSM *This, HSM_EVENT event, void *param);
HSM_EVENT IHM_StateOnDispHndlr(HSM *This, HSM_EVENT event, void *param);
HSM_EVENT IHM_StateOnDispPlayHndlr(HSM *This, HSM_EVENT event, void *param);
HSM_EVENT IHM_StateOnDispMenuHndlr(HSM *This, HSM_EVENT event, void *param);
const char *HSM_Evt2Str(uint32_t event);

void IhmHSM_init(IhmHSM *This, char *name);

#endif