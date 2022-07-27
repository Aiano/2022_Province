/*
 * bsp_can.h
 *
 *  Created on: 2022年7月27日
 *      Author: Aiano
 */

#ifndef BSP_BSP_CAN_H_
#define BSP_BSP_CAN_H_

#include "main.h"

#define M2006_LOC_DEAD_ZONE 1             /*位置环死区*/
#define M2006_LOC_INTEGRAL_START_ERR 200 /*积分分离时对应的误差范围*/
#define M2006_LOC_INTEGRAL_MAX_VAL 8191     /*积分范围限定，防止积分饱和*/

#define M2006_SPE_DEAD_ZONE 2              /*速度环死区*/
#define M2006_SPE_INTEGRAL_START_ERR 2000 /*积分分离时对应的误差范围*/
#define M2006_SPE_INTEGRAL_MAX_VAL 10000  /*积分范围限定，防止积分饱和*/

extern volatile uint32_t g_ui32MsgCount;
extern volatile bool g_bRXFlag;
extern volatile bool g_bErrFlag;
extern tCANMsgObject sCANMessage;
extern uint8_t pui8MsgData[8];
extern uint16_t m2006_angle[4];
extern int16_t m2006_speed[4];
extern uint16_t m2006_torque[4];

typedef enum
{

    CAN_2006Moto_ALL_ID = 0x200,
    CAN_2006Moto1_ID = 0x201,
    CAN_2006Moto2_ID = 0x202,
    CAN_2006Moto3_ID = 0x203,
    CAN_2006Moto4_ID = 0x204,

}CAN_Message_ID;

typedef struct {
    float target_val; //目标值
    float err;          //偏差值
    float err_last;      //上一个偏差值
    float Kp, Ki, Kd; //比例、积分、微分系数
    float integral;      //积分值
    float output_val; //输出值
} PID;

void bsp_can_init();
void bsp_can_get_feedback();
void bsp_can_set_current(int16_t value, int motor_id);
void bsp_can_pid_param_init();
int16_t speed_pid_realize(PID *pid, float actual_val);
void bsp_can_speed_loop(int motor_id);

#endif /* BSP_BSP_CAN_H_ */
