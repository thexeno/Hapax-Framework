#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "utils/sys_types.h"

#define BUTTON_MAX_CHANNELS 2U
// Logical state definitions
#define BUTTON_ACTIVE_LOW 0x00
#define BUTTON_ACTIVE_HIGH 0x01



//extern uint8_t btn_ButtonMap[BUTTON_MAX_CHANNELS][BUTTON_IO_IDX_SIZE];

// enum {
//  BUTTON_IO_NAME_ID = 0U,
//  BUTTON_IO_ASSIGNED_VALUE_ID,
//  BUTTON_IO_ACTIVE_VALUE_ID,
//  BUTTON_IO_ID_SIZE
// };



/* Button request enumeration */
enum button_enums {
	BUTTON_ENUM_NO_RQ = 0U,
	BUTTON_ENUM_RQ    = 1U,
	BUTTON_ENUM_RISE = 2U,
	BUTTON_ENUM_FALL = 3U,
	BUTTON_ENUM_PRESSED = 4U,
	BUTTON_ENUM_IDLE = 5U,
	BUTTON_ENUM_PULLUP_ON,
	BUTTON_ENUM_PULLUP_OFF
} ;

/* Button data struct */
struct button_parameters
{
	uint8_t id;
	uint8_t mode;
	uint8_t value;
	uint8_t raw_debounced;
	uint8_t raw;
	uint8_t assigned;
	uint8_t prev_assigned;
	uint8_t current_state;
	uint8_t prev_state;
	uint8_t active_state;
	int16_t debounce_counter;
	uint8_t stop_poll;
	uint8_t delay;
};


/**======================================================================================================================================================
 * @brief      Public function prototypes
 =======================================================================================================================================================*/


void Button_add(uint8_t id, uint8_t active_value, uint8_t init_value, uint8_t pullup_en_opt);
void Button_start(uint8_t step, uint8_t debounce_press, uint8_t debounce_release);
void Button_clear_rq(void);
uint8_t Button_get_rq(void);
uint8_t Button_transition(uint8_t id);
uint8_t Button_state(uint8_t id);
void Button_handler(void);
void Button_init(void);


 

#endif
