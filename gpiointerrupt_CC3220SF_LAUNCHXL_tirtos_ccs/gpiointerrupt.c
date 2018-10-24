/*
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

/* Example/Board Header files */
#include "Board.h"
//timer header files
#include <ti/drivers/Timer.h>
//I2C header files
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>
#include <unistd.h>
static Display_Handle display;

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on Board_GPIO_BUTTON0.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(Board_GPIO_LED0);
}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on Board_GPIO_BUTTON1.
 *  This may not be used for all boards.
 */
void gpioButtonFxn1(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(Board_GPIO_LED1);
}
/* Callback used for toggling the LED. */
void timerCallback(Timer_Handle myHandle);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    //GPIO EVERYTHING
    /* Call driver init functions */
    GPIO_init();

    /* Configure the LED and button pins */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_LED1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_BUTTON0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    /* install Button callback */
    GPIO_setCallback(Board_GPIO_BUTTON0, gpioButtonFxn0);

    /* Enable interrupts */
    GPIO_enableInt(Board_GPIO_BUTTON0);

    /*
     *  If more than one input pin is available for your device, interrupts
     *  will be enabled on Board_GPIO_BUTTON1.
     */
    if (Board_GPIO_BUTTON0 != Board_GPIO_BUTTON1) {
        /* Configure BUTTON1 pin */
        GPIO_setConfig(Board_GPIO_BUTTON1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

        /* Install Button callback */
        GPIO_setCallback(Board_GPIO_BUTTON1, gpioButtonFxn1);
        GPIO_enableInt(Board_GPIO_BUTTON1);
    }
    //END GPIO
    //all timer code 100% scalped from timerled.c in timerled example
    //START TIMER
    /* Period and duty in microseconds */
    Timer_Handle timer0;
    Timer_Params params;
    Timer_init();
    /* Setting up the timer in continuous callback mode that calls the callback
         * function every 1,000,000 microseconds, or 1 second.
         */
        Timer_Params_init(&params);
        params.period = 1000000;
        params.periodUnits = Timer_PERIOD_US;
        params.timerMode = Timer_CONTINUOUS_CALLBACK;
        params.timerCallback = timerCallback;

        timer0 = Timer_open(Board_TIMER0, &params);

        if (timer0 == NULL) {
            /* Failed to initialized timer */
            while (1);
        }

        if (Timer_start(timer0) == Timer_STATUS_ERROR) {
            /* Failed to start timer */
            while (1);
        }
        //END TIMER
        //START I2C
        unsigned int    i;
        uint16_t        temperature;
        uint8_t         txBuffer[1];
        uint8_t         rxBuffer[2];
        I2C_Handle      i2c;
        I2C_Params      i2cParams;
        I2C_Transaction i2cTransaction;

        /* Call driver init functions */
        Display_init();
        I2C_init();
        /* Open the HOST display for output */
            display = Display_open(Display_Type_UART, NULL);
            if (display == NULL) {
                while (1);
            }
    return (NULL);
}

void timerCallback(Timer_Handle myHandle)
{
    printf("i want to die\n");
}
