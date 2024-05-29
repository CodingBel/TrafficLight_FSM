//#include<zephyr/zephyr.h>
#include <zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>
#include <zephyr/smf.h>

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

int32_t sleep_msec = 0; 

/* forward declaration of state table */
static const struct smf_state states[]; 

/* list of traffic light App staets*/
enum state{
        INIT, 
        RED, 
        RED_YELLOW, 
        GREEN, 
        YELLOW
};

/* User defined object*/
struct s_object{
        /* this must be the first attribute*/
        struct smf_ctx ctx;  
}s_obj;

/* state INIT*/
static void init_run(void *o){
        printk("INIT\n");
        gpio_pin_configure_dt(&ledred_spec, GPIO_OUTPUT);
        gpio_pin_configure_dt(&ledyellow_spec, GPIO_OUTPUT);
        gpio_pin_configure_dt(&ledgreen_spec, GPIO_OUTPUT);
        sleep_msec = 0; 
        smf_set_state(SMF_CTX(&s_obj), &states[RED]); // Setting the next state
}

/* state RED*/
static void red_run (void *o){
        printk("Red\n");
        gpio_pin_set_dt(&ledred_spec, 1);
        gpio_pin_set_dt(&ledyellow_spec, 0);
        gpio_pin_set_dt(&ledgreen_spec, 0);
        sleep_msec = RED_SLEEP_TIME_MS; 
        smf_set_state(SMF_CTX(&s_obj), &states[RED_YELLOW]);  
}

/* state RED_YELLOW*/
static void red_yellow_run (void *o){
        printk("Red Yellow\n");
        gpio_pin_set_dt(&ledred_spec, 1);
        gpio_pin_set_dt(&ledyellow_spec, 1);
        gpio_pin_set_dt(&ledgreen_spec, 0);
        sleep_msec = RED_YELLOW_SLEEP_TIME_MS; 
        smf_set_state(SMF_CTX(&s_obj), &states[GREEN]);   
}

/* state GREEN*/
static void green_run(void *o){
        printk ("Green\n");
        gpio_pin_set_dt(&ledred_spec, 0);
        gpio_pin_set_dt(&ledyellow_spec, 0);
        gpio_pin_set_dt(&ledgreen_spec, 1);
        sleep_msec = GREEN_SLEEP_TIME_MS; 
        smf_set_state(SMF_CTX(&s_obj), &states[YELLOW]);  
}

/* state YELLOW*/
static void yellow_run (void *o){
        printk("Yellow\n");
        gpio_pin_set_dt(&ledred_spec, 0);
        gpio_pin_set_dt(&ledyellow_spec, 1);
        gpio_pin_set_dt(&ledgreen_spec, 0);
        sleep_msec = GREEN_SLEEP_TIME_MS; 
        smf_set_state(SMF_CTX(&s_obj), &states[RED]);   
}

/* Populate the states table */
static const struct smf_state states[] = {
        [INIT] = SMF_CREATE_STATE (NULL, init_run, NULL),  // The first NULL is for Entry function 
        [RED] = SMF_CREATE_STATE (NULL, red_run, NULL),    // and the third NULL is for the Exit function 
        [RED_YELLOW] = SMF_CREATE_STATE (NULL, red_yellow_run, NULL), // Which we're not using in this application 
        [GREEN] = SMF_CREATE_STATE (NULL, green_run, NULL), 
        [YELLOW] = SMF_CREATE_STATE (NULL, yellow_run, NULL)   
}; 

int main () {
        int32_t ret; 

        /* set the initial state */
        smf_set_initial(SMF_CTX(&s_obj), &states[INIT]);

	while (1){
                ret = smf_run_state(SMF_CTX(&s_obj));
                if (ret){
                        break;
                } 
                k_msleep(sleep_msec); 
	}
	return 0; 
}