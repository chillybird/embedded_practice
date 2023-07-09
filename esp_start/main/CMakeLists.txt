idf_component_register(SRCS "esp_start.c" 

                            # fs
                            "src/fs/ds_spiffs.c" 

                            # driver
                            "src/driver/ds_ft6336.c"
                            "src/driver/ds_screen.c"

                            # hal
                            "src/hal/ds_gpio.c" 
                            "src/hal/ds_i2c.c" 
                            "src/hal/ds_pwm.c"
                            "src/hal/ds_uart.c"
                            "src/hal/ds_spi.c"
                            "src/hal/ds_timer.c"
                            "src/hal/ds_nvs.c"  
                            "src/hal/ds_adc.c"  
                            
                            # data
                            "src/ds_system_data.c" 
                            "src/data/ds_data_num.c"

                            # ui
                            "src/ui/ds_ui_timepage.c"
                            "src/ui/ds_ui_page_manage.c"

                            # net
                            "src/net/ds_wifi_ap.c"
                            "src/net/ds_http_server.c"
                            "src/net/ds_wifi_sta.c"
                            "src/net/ds_wifi_ap_sta.c"
                            "src/net/ds_wifi_scan.c"
                            "src/net/ds_http_request.c"

                            # function tests
                            "src/ds_tests.c"

                    INCLUDE_DIRS "." 
                                "./include" 
                                "./include/hal" 
                                "./include/fs" 
                                "./include/driver"
                                "./include/data"
                                "./include/ui"
                                "./include/net"

                    EMBED_FILES "html/favicon.ico"
                                "html/setting.html")
