//#include<zephyr/zephyr.h>
#include <zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>

#define RED_SLEEP_TIME_MS 		2000
#define RED_YELLOW_SLEEP_TIME_MS	500
#define GREEN_SLEEP_TIME_MS		2000
#define YELLOW_SLEEP_TIME_MS		500

#define RED_NODE DT_NODELABEL(ledred)
#define YELLOW_NODE DT_NODELABEL(ledyellow)
#define GREEN_NODE DT_NODELABEL(ledgreen)

static const struct gpio_dt_spec ledred_spec = GPIO_DT_SPEC_GET(RED_NODE, gpios);
static const struct gpio_dt_spec ledyellow_spec = GPIO_DT_SPEC_GET(YELLOW_NODE, gpios);
static const struct gpio_dt_spec ledgreen_spec = GPIO_DT_SPEC_GET(GREEN_NODE, gpios);

// define the Finite State Machine (FSM)
typedef enum {
        INIT, 
        RED, 
        RED_YELLOW, 
        GREEN, 
        YELLOW
}ApplicationState_t;
static ApplicationState_t state = INIT;  

int main () {
	while (1){
                switch (state){
                        case INIT: 
                        gpio_pin_configure_dt(&ledred_spec, GPIO_OUTPUT);
                        gpio_pin_configure_dt(&ledyellow_spec, GPIO_OUTPUT);
                        gpio_pin_configure_dt(&ledgreen_spec, GPIO_OUTPUT);
                        state = RED; 
                        break; 

                        case RED: 
                        gpio_pin_set_dt(&ledred_spec, 1);
                        gpio_pin_set_dt(&ledyellow_spec, 0);
                        gpio_pin_set_dt(&ledgreen_spec, 0);
                        state = RED_YELLOW; 
                        k_msleep(RED_SLEEP_TIME_MS);
                        break; 

                        case RED_YELLOW: 
                        gpio_pin_set_dt(&ledred_spec, 1); 
                        gpio_pin_set_dt(&ledyellow_spec, 1);
                        gpio_pin_set_dt(&ledgreen_spec, 0);
                        state = GREEN; 
                        k_msleep(RED_YELLOW_SLEEP_TIME_MS);  
                        break; 

                        case GREEN: 
                        gpio_pin_set_dt(&ledred_spec, 0);
                        gpio_pin_set_dt(&ledyellow_spec, 0);
                        gpio_pin_set_dt(&ledgreen_spec, 1);
                        state = YELLOW; 
                        k_msleep(GREEN_SLEEP_TIME_MS);
                        break; 

                        case YELLOW: 
                        gpio_pin_set_dt(&ledred_spec, 0);
                        gpio_pin_set_dt(&ledyellow_spec, 1);
                        gpio_pin_set_dt(&ledgreen_spec, 0);
                        state = RED; 
                        k_msleep(YELLOW_SLEEP_TIME_MS);
                        break;      
                }   
	}
	return 0; 
}