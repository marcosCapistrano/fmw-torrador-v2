idf_component_register(SRCS "hsm.c" "ihm_manager.c" "uart_task.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_ao_hsm common)
