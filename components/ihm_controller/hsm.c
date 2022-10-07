#include "hsm.h"
#include "esp_hsm.h"


HSM_STATE IHM_StateOff;
HSM_STATE IHM_StateOn;
HSM_STATE IHM_StateOnShoot;
HSM_STATE IHM_StateOnDisp;
HSM_STATE IHM_StateOnDispPlay;
HSM_STATE IHM_StateOnDispMenu;

HSM_EVENT IHM_StateOffHndlr(HSM *This, HSM_EVENT event, void *param)
{
    if (event == HSME_ENTRY)
    {
        printf("\tEnter Low Power Mode\n");
    }
    else if (event == HSME_EXIT)
    {
        printf("\tExit Low Power Mode\n");
    }
    else if (event == HSME_PWR)
    {
        HSM_Tran(This, &IHM_StateOn, 0, NULL);
        return 0;
    }
    return event;
}

HSM_EVENT IHM_StateOnHndlr(HSM *This, HSM_EVENT event, void *param)
{
    if (event == HSME_ENTRY)
    {
        printf ("\tOpen Lens\n");
    }
    else if (event == HSME_EXIT)
    {
        printf ("\tClose Lens\n");
    }
    else if (event == HSME_INIT)
    {
        HSM_Tran(This, &IHM_StateOnShoot, 0, NULL);
    }
    else if (event == HSME_PWR)
    {
        HSM_Tran(This, &IHM_StateOff, 0, NULL);
        return 0;
    }
    else if (event == HSME_LOWBATT)
    {
        printf("\tBeep low battery warning\n");
        return 0;
    }
    return event;
}

HSM_EVENT IHM_StateOnShootHndlr(HSM *This, HSM_EVENT event, void *param)
{
    if (event == HSME_ENTRY)
    {
        printf("\tEnable Sensor\n");
    }
    else if (event == HSME_EXIT)
    {
        printf("\tDisable Sensor\n");
    }
    else if (event == HSME_RELEASE)
    {
        printf("\tCLICK!, save photo\n");
        return 0;
    }
    else if (event == HSME_MODE)
    {
        HSM_Tran(This, &IHM_StateOnDispPlay, 0, NULL);
        return 0;
    }
    return event;
}

HSM_EVENT IHM_StateOnDispHndlr(HSM *This, HSM_EVENT event, void *param)
{
    if (event == HSME_ENTRY)
    {
        printf("\tTurn on LCD\n");
    }
    else if (event == HSME_EXIT)
    {
        printf("\tTurn off LCD\n");
    }
    return event;
}

HSM_EVENT IHM_StateOnDispPlayHndlr(HSM *This, HSM_EVENT event, void *param)
{
    if (event == HSME_ENTRY)
    {
        printf("\tDisplay Pictures\n");
    }
    else if (event == HSME_MODE)
    {
        HSM_Tran(This, &IHM_StateOnDispMenu, 0, NULL);
        return 0;
    }
    return event;
}

HSM_EVENT IHM_StateOnDispMenuHndlr(HSM *This, HSM_EVENT event, void *param)
{
    if (event == HSME_ENTRY)
    {
        printf("\tDisplay Menu\n");
    }
    else if (event == HSME_MODE)
    {
        HSM_Tran(This, &IHM_StateOnShoot, 0, NULL);
        return 0;
    }
    return event;
}

const char *HSM_Evt2Str(uint32_t event)
{
    switch(event)
    {
    case HSME_PWR:
        return "HSME_PWR";
    case HSME_RELEASE:
        return "HSME_RELEASE";
    case HSME_MODE:
        return "HSME_MODE";
    case HSME_LOWBATT:
        return "HSME_LOWBATT";

    default:
        return NULL;
    }
}

void IhmHSM_init(IhmHSM *This, char *name)
{
    // Step 1: Create the HSM States
    HSM_STATE_Create(&IHM_StateOff, "Off", IHM_StateOffHndlr, NULL);
    HSM_STATE_Create(&IHM_StateOn, "On", IHM_StateOnHndlr, NULL);
    HSM_STATE_Create(&IHM_StateOnShoot, "On.Shoot", IHM_StateOnShootHndlr, &IHM_StateOn);
    HSM_STATE_Create(&IHM_StateOnDisp, "On.Disp", IHM_StateOnDispHndlr, &IHM_StateOn);
    HSM_STATE_Create(&IHM_StateOnDispPlay, "On.Disp.Play", IHM_StateOnDispPlayHndlr, &IHM_StateOnDisp);
    HSM_STATE_Create(&IHM_StateOnDispMenu, "On.Disp.Menu", IHM_StateOnDispMenuHndlr, &IHM_StateOnDisp);
    // Step 2: Initiailize the HSM and starting state
    HSM_Create((HSM *)This, name, &IHM_StateOff);
    // Step 3: [Optional] Enable HSM debug
    HSM_SET_PREFIX((HSM *)This, "[DBG] ");
    HSM_SET_DEBUG((HSM *)This, HSM_SHOW_ALL);
    // Step 4: IHM member initialization
    This->param1 = 0;
    This->param2 = 1;
}