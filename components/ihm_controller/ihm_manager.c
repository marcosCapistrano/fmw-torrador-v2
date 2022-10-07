#include "ihm_manager.h"

#include "esp_ao_evt.h"
#include "esp_hsm.h"
#include "hsm.h"
#include "common.h"
#include "uart_task.h"

static StackType_t ihmManager_stack[configMINIMAL_STACK_SIZE]; /* task stack */
static Event *ihmManager_queue[10];

typedef struct {
    Active super;
    IhmHSM hsm;
} IhmManager;

static IhmManager ihmManager;

static void IhmManager_dispatch(IhmManager *const me, Event const *const e) {
    HSM_EVENT event;
    IHMSignal sig = (IHMSignal) e->sig;

    switch (sig) {
        case IHM_BUTTON_CONTEXT_PRESSED:
            event = HSME_PWR;
        break;

        default:
            return;
    }

    HSM_Run((HSM *)&me->hsm, event, 0);
}

Active *IhmManager_ctor() {
    Active_ctor(&ihmManager.super, (DispatchHandler)&IhmManager_dispatch);
    IhmHSM_init(&ihmManager.hsm, "IHM_HSM");

    return &ihmManager.super;
}

void IhmManager_start() {
    Active_start(&ihmManager.super,
                 1U,
                 ihmManager_queue,
                 sizeof(ihmManager_queue) / sizeof(ihmManager_queue[0]),
                 ihmManager_stack,
                 sizeof(ihmManager_stack),
                 0U);

    uart_task_init(AO_ihmManager);
}