#include "libs/buttons.h"
#include "user/hal/atmega328/pin_hal.h"

// // metti esterno poi
// uint8_t Button_map[BUTTON_MAX_CHANNELS][BUTTON_IO_IDX_SIZE] =
// {
//     {MAIN_BUTTON, MAIN_BUTTON_INIT_VALUE, ACONF_BUTTON_ACTIVE_LOW}  
// };

// // definisci BUTTON_MAX_CHANNELS esterno
// static struct button_data_struct button_data[BUTTON_MAX_CHANNELS];

static struct button_parameters button_data[BUTTON_MAX_CHANNELS];

static uint16_t button_step_time = 0;  // Used to store the button step time at initialization.
static uint8_t  button_deb_time_press = 0;    // Used to store the debounce time at initialization. It will be composed by incremental step_time values.
static uint8_t  button_deb_time_release = 0;
static uint8_t  button_rq = BUTTON_ENUM_IDLE;  // Store the button(s) request status
static uint8_t  buttons_total = 0;
static uint8_t  button_init_index = 0;

/**======================================================================================================================================================
 * @brief      Private function prototypes
 =======================================================================================================================================================*/


static void    button_state_update(void);
static uint8_t button_debouncer(struct button_parameters* btn);



/**======================================================================================================================================================
 * @brief      Private function definitions
 =======================================================================================================================================================*/


/**
 * @brief      Executes the buttons signal processing. The period in which shall be called must be the equal to the step_time
 * TODO: support vistual buttons
 */

static void button_state_update(void)
{
    uint8_t i = 0;
    
    for (i=0; i<buttons_total; i++)
    {
        button_data[i].raw = 0;////pin_RdPin(button_data[i].id);
        button_debouncer(&(button_data[i]));
        if (button_data[i].raw_debounced == 0)
        {
            if (button_data[i].active_state == BUTTON_ACTIVE_LOW)
            {
                button_data[i].current_state = BUTTON_ENUM_PRESSED;  
            }
            else
            {
                button_data[i].current_state = BUTTON_ENUM_IDLE;      
            }
        }
        else
        {
            if (button_data[i].active_state == BUTTON_ACTIVE_HIGH)
            {
                button_data[i].current_state = BUTTON_ENUM_PRESSED;   
            }
            else
            {
                button_data[i].current_state = BUTTON_ENUM_IDLE;      
            }
        }

        if (button_data[i].current_state != button_data[i].prev_state)
        {
            // latch the request for further processing
            button_rq = BUTTON_ENUM_RQ;
        }
    }
}


/**
 * @brief      According to logic values of the button selected, elaborates the filtered (debounced) value of the button
 *
 * @param      btn   The button data struct
 *
 * @return     Return ZERO if the button is assigned, otehrwise the button state is not yet stable
 */
static uint8_t button_debouncer(struct button_parameters* btn)
{
    uint8_t ret = 1;
    if (btn->raw != btn->raw_debounced)   
    {
        // A change happened. Start measuring.
        
        btn->debounce_counter -= button_step_time;

        // Update raw debounced value in function of a press or release time required to filter
        // the signal. 
        if (btn->debounce_counter <= 0)
        {
            // When debounce count is elapsed according to the initial threshold,
            // restore the counter according to the current state of the debounce value nd the active state.
            btn->raw_debounced = btn->raw;
            if  (btn->raw_debounced != 0)
            {
                if (btn->active_state == BUTTON_ACTIVE_HIGH) 
                {
                    // If the rawDebounced corresponds to the activeState, the button is
                    // determined to be pressed, so setup counter for the other event, which is release.
                    // Viceversa apply.
                    btn->debounce_counter = button_deb_time_release;
                }
                else
                {
                    btn->debounce_counter = button_deb_time_press;              
                }
            }
            else
            {
                // Same as the if(), with logic reversed.
                if (btn->active_state == BUTTON_ACTIVE_LOW)  
                {
                    btn->debounce_counter = button_deb_time_release;
                }
                else
                {
                    btn->debounce_counter = button_deb_time_press;              
                }
            }
            ret = 0;
        }
    }
    else
    {
        if  (btn->raw_debounced != 0)
        {
            if (btn->active_state == BUTTON_ACTIVE_HIGH) 
            {
                btn->debounce_counter = button_deb_time_release;
            }
            else
            {
                btn->debounce_counter = button_deb_time_press;              
            }
        }
        else
        {
            if (btn->active_state == BUTTON_ACTIVE_LOW)  
            {
                btn->debounce_counter = button_deb_time_release;
            }
            else
            {
                btn->debounce_counter = button_deb_time_press;              
            }
        }
    }
    return ret;
}


// static void _btn_FSM(BUTTON_DATA* btn)
// {

// }


/**======================================================================================================================================================
 * @brief      Public function definitions
 =======================================================================================================================================================*/

/**
 * @brief      The button handler called with a regualr time equal to step_time
 */
void Button_handler(void)
{
    button_state_update();   
}


/**
 * @brief      Get the status if the selected button. Compared to the btn_InputBtnGetAction(), this just returns tha value of the selected button
 *
 * @param[in]  id    The selected button which requires to be read
 *
 * @return     The updated state of the selected button. If NON ZERO and not a valid button state, indicate a missing selected button
 *  
 */
uint8_t Button_state(uint8_t id)
{
    uint8_t i = 0;
    
    for (i=0; i<buttons_total; i++)
    {
        if (button_data[i].id == id)
        {
            return button_data[i].assigned;
        }
    }
    return 1;
}


/**
 * @brief      Computes the type of the sampled input from the previous call of the function: rise (not press -> press), fall (press -> not press), 
 *             low (not press - likely was preceeded by a fall), high (press - likely was preceeded by a rise).
 *             Each time is called, the previous buttons states are updated. Therefore, if run slower than the phisical button state update (i.e. very fast switching),
 *             this function will detect only the rise and the fall state, or even create aliasing.
 *             
 *             Has to be called faster than the button mechanical switch on-off-on (or off-on-off) to avoid aliasing.    
 *
 * @param[in]  sel   The selected button which requires the update
 *
 * @return     The updated state of the selected button. If NON ZERO and not a valid button state, indicate a missing selected button
 */
uint8_t Button_transition(uint8_t id)
{
    uint8_t ret = BUTTON_ENUM_IDLE;
    uint8_t i = 0;

    if (button_rq != BUTTON_ENUM_NO_RQ)
    {
        for (i=0; i<buttons_total; i++)
        {
            if (button_data[i].id == id)
            {
                if (button_data[i].current_state == BUTTON_ENUM_PRESSED && button_data[i].prev_state == BUTTON_ENUM_PRESSED)
                {
                    ret = BUTTON_ENUM_PRESSED;
                }
                else if (button_data[i].current_state == BUTTON_ENUM_PRESSED && button_data[i].prev_state == BUTTON_ENUM_IDLE)
                {
                    ret = BUTTON_ENUM_RISE;
                }
                else if (button_data[i].current_state == BUTTON_ENUM_IDLE && button_data[i].prev_state == BUTTON_ENUM_PRESSED)
                {
                    ret = BUTTON_ENUM_FALL;
                }
                else
                {
                    ret = BUTTON_ENUM_IDLE;
                }
                button_data[i].prev_state = button_data[i].current_state;
                break;
            }
        }   
    }
    
    return ret;
}


/**
 * @brief      Used to poll if a button state change happened, without scanning all the buttons which happens in the handler polling btn_BtnHnd().
 *
 * @return     The button request state
 */
uint8_t Button_get_rq(void)
{
    return button_rq;
}

/**
 * @brief      Clear button request
 */
void Button_clear_rq(void)
{
    button_rq = BUTTON_ENUM_NO_RQ;
}


/**
 * @brief      Initialize the library and prepare the data with the real IO through the HAL.
               The structure determine a "termination button": this allow the user to initialize different sets of buttons
               in a mutual exclusive way, by calling the function with a different array. To reuse the previous buttons set, the function shall be called again with their corresponding
               button map.
 *
 * @param[in]  step      The step time of the button evaluation state
 * @param[in]  debounce  The debounce
 */
void Button_init(void)
{
    

    // Prepare memory area
    memset(&button_data, 0x00, sizeof(button_data));
    
    //for (i=0; i<BUTTON_MAX_CHANNELS; i++)
    //{
        //buttons_total = ButtonMap[i][BUTTON_IO_NAME_ID];
        //buttons_total = ButtonMap[i][BUTTON_IO_ASSIGNED_VALUE_ID];
        //buttons_total = ButtonMap[i][BUTTON_IO_ACTIVE_VALUE_ID];
        //
        //if((ButtonMap[i][BUTTON_IO_NAME_ID] == BUTTON_END) && 
           //(ButtonMap[i][BUTTON_IO_ASSIGNED_VALUE_ID] == BUTTON_END) && 
           //(ButtonMap[i][BUTTON_IO_ACTIVE_VALUE_ID] == BUTTON_END))
        //{
            //buttons_total = i;
            //break;
        //}
    //}
    //if (buttons_total == 0)
    //{
        ////no termination button found, possible button memory map error
        //ret = -1;
    //}
    //else
    //{
        // for (i=0; i<BUTTON_MAX_CHANNELS; i++)
        // {   
        //     button_data[i].id    = ButtonMap[i][BUTTON_IO_NAME_ID];
        //     pin_MdPin(button_data[i].id, PIN_INPUT, PULLUP_DISABLED);   //  Initialize physical button in/out mode
        //     pin_WrPin(button_data[i].id, ButtonMap[i][BUTTON_IO_ASSIGNED_VALUE_ID]);  //  Write initial value to the input, if supported by mode
        //     button_data[i].active_state = ButtonMap[i][BUTTON_IO_ACTIVE_VALUE_ID];
        //     button_data[i].assigned = BUTTON_IDLE;
        //     button_data[i].prev_assigned = BUTTON_IDLE;
        // }
 
}

void Button_start(uint8_t step, uint8_t debounce_press, uint8_t debounce_release)
{


    //buttons_total = BUTTON_MAX_CHANNELS;
    buttons_total = button_init_index;
	if (step == 0)
	{
		// Avoid integral step time of zero by assigning the minimum system value. 
		// Should never arrive here.
		button_step_time = 1;
	}
	else
	{
		button_step_time = step;
	}
    
	// Allow deboucne time of ZERO
	button_deb_time_press = debounce_press;
	button_deb_time_release = debounce_release;
	//} 
}

void Button_add(uint8_t id, uint8_t active_value, uint8_t init_value, uint8_t pullup_en_opt)
{
    button_data[button_init_index].id = id;
    if (pullup_en_opt == BUTTON_ENUM_PULLUP_ON)
    {
        ////Pin_mode(button_data[button_init_index].id, PIN_INPUT, PULLUP_ENABLED); 
    }
    else
    {
        ////Pin_mode(button_data[button_init_index].id, PIN_INPUT, PULLUP_DISABLED); 
    }
   //// Pin_wr_pin(button_data[button_init_index].id, init_value);
    button_data[button_init_index].active_state = active_value;
    button_data[button_init_index].assigned = BUTTON_ENUM_IDLE;
    button_data[button_init_index].prev_assigned = BUTTON_ENUM_IDLE;
    button_init_index++;
}