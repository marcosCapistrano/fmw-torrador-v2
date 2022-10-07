#include "driver/gpio.h"
#include "esp_log.h"
#include "ihm_manager.h"
#include "esp_ao.h"

static const char *TAG = "MAIN";

Active *AO_ihmManager;

void app_main(void) {
    AO_ihmManager = IhmManager_ctor();
    IhmManager_start();
}
