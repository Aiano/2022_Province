/*
 * bsp_can.c
 *
 *  Created on: 2022年7月27日
 *      Author: Aiano
 */

#include "bsp_can.h"

//*****************************************************************************
//
// A counter that keeps track of the number of times the RX interrupt has
// occurred, which should match the number of messages that were received.
//
//*****************************************************************************
volatile uint32_t g_ui32MsgCount = 0;

//*****************************************************************************
//
// A flag for the interrupt handler to indicate that a message was received.
//
//*****************************************************************************
volatile bool g_bRXFlag = 0;

//*****************************************************************************
//
// A flag to indicate that some reception error occurred.
//
//*****************************************************************************
volatile bool g_bErrFlag = 0;

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************

//*****************************************************************************
//
// This function is the interrupt handler for the CAN peripheral.  It checks
// for the cause of the interrupt, and maintains a count of all messages that
// have been received.
//
//*****************************************************************************
tCANMsgObject TxHeader;
tCANMsgObject sCANMessage;

uint8_t TxData[8] = {0,0,0,0,0,0,0,0};
uint8_t pui8MsgData[8];

uint16_t m2006_angle[4];
int16_t m2006_speed[4];
uint16_t m2006_torque[4];

PID pid_speed[4];

void
CANIntHandler(void)
{
    uint32_t ui32Status;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    //
    // If the cause is a controller status interrupt, then get the status
    //
    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.
        //
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        //
        // Set a flag to indicate some errors may have occurred.
        //
        g_bErrFlag = 1;
    }

    //
    // Check if the cause is message object 1, which what we are using for
    // receiving messages.
    //
    else if(ui32Status == 1)
    {
        //
        // Getting to this point means that the RX interrupt occurred on
        // message object 1, and the message reception is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 1);

        //
        // Increment a counter to keep track of how many messages have been
        // received.  In a real application this could be used to set flags to
        // indicate when a message is received.
        //
        g_ui32MsgCount++;

        //
        // Set flag to indicate received message is pending.
        //
        g_bRXFlag = 1;

        //
        // Since a message was received, clear any error flags.
        //
        g_bErrFlag = 0;
    }

    //
    // Otherwise, something unexpected caused the interrupt.  This should
    // never happen.
    //
    else
    {
        //
        // Spurious interrupt handling can go here.
        //
    }
}

void bsp_can_init(){
    //
    // For this example CAN0 is used with RX and TX pins on port E4 and E5.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.
    // GPIO port B needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Configure the GPIO pin muxing to select CAN0 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PE4_CAN0RX);
    GPIOPinConfigure(GPIO_PE5_CAN0TX);

    //
    // Enable the alternate function on the GPIO pins.  The above step selects
    // which alternate function is available.  This step actually enables the
    // alternate function instead of GPIO for these pins.
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // The GPIO port and pins have been set up for CAN.  The CAN peripheral
    // must be enabled.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    //
    // Initialize the CAN controller
    //
    CANInit(CAN0_BASE);

    //
    // Set up the bit rate for the CAN bus.  This function sets up the CAN
    // bus timing for a nominal configuration.  You can achieve more control
    // over the CAN bus timing by using the function CANBitTimingSet() instead
    // of this one, if needed.
    // In this example, the CAN bus is set to 500 kHz.  In the function below,
    // the call to SysCtlClockGet() or ui32SysClock is used to determine the
    // clock rate that is used for clocking the CAN peripheral.  This can be
    // replaced with a  fixed value if you know the value of the system clock,
    // saving the extra function call.  For some parts, the CAN peripheral is
    // clocked by a fixed 8 MHz regardless of the system clock in which case
    // the call to SysCtlClockGet() or ui32SysClock should be replaced with
    // 8000000.  Consult the data sheet for more information about CAN
    // peripheral clocking.
    //
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    CANBitRateSet(CAN0_BASE, ui32SysClock, 500000);
#else
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 1000000);
#endif

    //
    // Enable interrupts on the CAN peripheral.  This example uses static
    // allocation of interrupt handlers which means the name of the handler
    // is in the vector table of startup code.  If you want to use dynamic
    // allocation of the vector table, then you must also call CANIntRegister()
    // here.
    //
    CANIntRegister(CAN0_BASE, CANIntHandler); // if using dynamic vectors
    //
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    //
    // Enable the CAN interrupt on the processor (NVIC).
    //
    IntEnable(INT_CAN0);

    //
    // Enable the CAN for operation.
    //
    CANEnable(CAN0_BASE);

    //
    // Initialize a message object to be used for receiving CAN messages with
    // any CAN ID.  In order to receive any CAN ID, the ID and mask must both
    // be set to 0, and the ID filter enabled.
    //
    // CAN Transmit Header
    TxHeader.ui32MsgID = 0x200;
    TxHeader.ui32MsgIDMask = 0;
    TxHeader.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    TxHeader.ui32MsgLen = sizeof(TxData);
    TxHeader.pui8MsgData = TxData;
    // CAN Receive Header
    sCANMessage.ui32MsgID = 0;
    sCANMessage.ui32MsgIDMask = 0;
    sCANMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    sCANMessage.ui32MsgLen = 8;

    //
    // Now load the message object into the CAN peripheral.  Once loaded the
    // CAN will receive any message on the bus, and an interrupt will occur.
    // Use message object 1 for receiving messages (this is not the same as
    // the CAN ID which can be any value in this example).
    //
    CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_RX);

    // Init PID
    bsp_can_pid_param_init();
}

void bsp_can_get_feedback(){
    int index = sCANMessage.ui32MsgID - 0x200;

    m2006_angle[index - 1] = pui8MsgData[0] << 8 | pui8MsgData[1];
    m2006_speed[index - 1] = pui8MsgData[2] << 8 | pui8MsgData[3];
    m2006_torque[index - 1] = pui8MsgData[4] << 8 | pui8MsgData[5];

    int16_t u = speed_pid_realize(pid_speed + index - 1, m2006_speed[index - 1]);
    UARTprintf("u=%d\n", u);
//     speed loop
    bsp_can_set_current(u, index);
}

void bsp_can_set_current(int16_t value, int motor_id){
    if(motor_id >= 1 && motor_id <=4){
        // limit
        if(value < -10000) value = -10000;
        else if(value > 10000) value = 10000;

        // control only one motor at a time, leave others unchanged
        TxData[2 * motor_id - 2] = value >> 8;
        TxData[2 * motor_id - 1] = value & 0x00ff;
    }

    CANMessageSet(CAN0_BASE, 1, &TxHeader, MSG_OBJ_TYPE_TX);
    CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_RX);
}

void bsp_can_pid_param_init(){
    // Motor3
    pid_speed[2].target_val = -5000.0;
    pid_speed[2].output_val = 0.0;
    pid_speed[2].err = 0.0;
    pid_speed[2].err_last = 0.0;
    pid_speed[2].integral = 0.0;
    pid_speed[2].Kp = 4;
    pid_speed[2].Ki = 0.0;
    pid_speed[2].Kd = 0.0;
    // Motor4
    pid_speed[3].target_val = 5000.0;
    pid_speed[3].output_val = 0.0;
    pid_speed[3].err = 0.0;
    pid_speed[3].err_last = 0.0;
    pid_speed[3].integral = 0.0;
    pid_speed[3].Kp = 4;
    pid_speed[3].Ki = 0.0;
    pid_speed[3].Kd = 0.0;
}

int16_t speed_pid_realize(PID *pid, float actual_val) {
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;

    /* 设定闭环死区 */
    if ((pid->err > -M2006_SPE_DEAD_ZONE) && (pid->err < M2006_SPE_DEAD_ZONE)) {
        pid->err = 0;
        pid->integral = 0;
        pid->err_last = 0;
    }

    /*积分项，积分分离，偏差较大时去掉积分作用*/
    if (pid->err > -M2006_SPE_INTEGRAL_START_ERR && pid->err < M2006_SPE_INTEGRAL_START_ERR) {
        pid->integral += pid->err;
        /*积分范围限定，防止积分饱和*/
        if (pid->integral > M2006_SPE_INTEGRAL_MAX_VAL) {
            pid->integral = M2006_SPE_INTEGRAL_MAX_VAL;
        } else if (pid->integral < -M2006_SPE_INTEGRAL_MAX_VAL) {
            pid->integral = -M2006_SPE_INTEGRAL_MAX_VAL;
        }
    }

    /*PID算法实现*/
    pid->output_val = pid->Kp * pid->err +
                      pid->Ki * pid->integral +
                      pid->Kd * (pid->err - pid->err_last);

    /*误差传递*/
    pid->err_last = pid->err;

    /*返回当前实际值*/
    return (int16_t) pid->output_val;
}
