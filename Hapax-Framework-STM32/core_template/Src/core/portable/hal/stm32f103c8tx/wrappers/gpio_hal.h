#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <sys_types.h>
#include "hal.h"
#include "conf.h"

#define GPIO_TOTAL_PIN CONF_GPIO_ENUM_UNUSED
typedef GPIO_TypeDef *gpio_hal_port_t;

typedef enum {  // up to the nearest upper power of 2 (excluded), enum is guaranteed to preserve the value
                // hence bit wise operations are allowed with enum
                // And with this is guaranteed to use code with the correct data type
    GPIO_HAL_PIN_0 = 0x0001,   /* Pin 0 selected    */
    GPIO_HAL_PIN_1 = 0x0002,   /* Pin 1 selected    */
    GPIO_HAL_PIN_2 = 0x0004,   /* Pin 2 selected    */
    GPIO_HAL_PIN_3 = 0x0008,   /* Pin 3 selected    */
    GPIO_HAL_PIN_4 = 0x0010,   /* Pin 4 selected    */
    GPIO_HAL_PIN_5 = 0x0020,   /* Pin 5 selected    */
    GPIO_HAL_PIN_6 = 0x0040,   /* Pin 6 selected    */
    GPIO_HAL_PIN_7 = 0x0080,   /* Pin 7 selected    */
    GPIO_HAL_PIN_8 = 0x0100,   /* Pin 8 selected    */
    GPIO_HAL_PIN_9 = 0x0200,   /* Pin 9 selected    */
    GPIO_HAL_PIN_10 = 0x0400,  /* Pin 10 selected   */
    GPIO_HAL_PIN_11 = 0x0800,  /* Pin 11 selected   */
    GPIO_HAL_PIN_12 = 0x1000,  /* Pin 12 selected   */
    GPIO_HAL_PIN_13 = 0x2000,  /* Pin 13 selected   */
    GPIO_HAL_PIN_14 = 0x4000,  /* Pin 14 selected   */
    GPIO_HAL_PIN_15 = 0x8000,  /* Pin 15 selected   */
    GPIO_HAL_PIN_ALL = 0xffff  // for boundary definition
} gpio_hal_pin_t;

#define GPIO_HAL_PORTA ((gpio_hal_port_t)GPIOA)
#define GPIO_HAL_PORTB ((gpio_hal_port_t)GPIOB)
#define GPIO_HAL_PORTC ((gpio_hal_port_t)GPIOC)
#define GPIO_HAL_PORTD ((gpio_hal_port_t)GPIOD)

typedef enum
{
    GPIO_HAL_MASK_AND_NONE = 0, // for boundary definition
    GPIO_HAL_MASK_AND_UNUSED = (uint32_t)(-1)
} gpio_hal_mask_t;

typedef enum
{
    GPIO_HAL_MODE_IN = 0,
    GPIO_HAL_MODE_OUT,
    GPIO_HAL_MODE_MUX
} gpio_hal_mode_t;

typedef enum
{
    GPIO_HAL_VAL_FALSE = 0,
    GPIO_HAL_VAL_TRUE,
    GPIO_HAL_VAL_UNDEF
} gpio_hal_val_t;

typedef uint32_t gpio_hal_pull_t;

typedef enum
{
    GPIO_OK = 0,
    GPIO_INIT_PEND,
    GPIO_ERR_PIN,
    GPIO_ERR_PORT
} gpio_hal_err_t;

typedef struct
{
    conf_gpio_e pin_enum;
    gpio_hal_port_t port;
    gpio_hal_pin_t pin;
    gpio_hal_mode_t mode_io;
    gpio_hal_pull_t mode_pull;
    gpio_hal_val_t val;
    gpio_hal_err_t status;
} gpio_hal_cfg_t;
//#define GPIO_TOTAL_CONF

const gpio_hal_cfg_t *const Gpio_hal_conf_get(void);
base_t Gpio_hal_conf_get_size(void);

void Gpio_hal_init(const gpio_hal_cfg_t *handle);
gpio_hal_err_t Gpio_hal_write_value(conf_gpio_e pin_enum, gpio_hal_val_t val, gpio_hal_mask_t mask); // per il gpio_hal_val_t metti un redefine di quello ufficiale stm32
gpio_hal_val_t Gpio_hal_read_value(conf_gpio_e pin_enum, gpio_hal_mask_t mask);

#endif
