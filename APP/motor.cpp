
#include "motor.h"

using namespace std;

moto_measure_t moto_chassis[motor_num] = {0}; // 4 chassis moto



void motor_move(void)
{
    // ���pid���Ʋ���
	// pid_reset(&pid_angle[1], kp_ang_2, ki_ang_2, kd_ang_2);
	// ���ٶȼ���

	// angle_setspeed_1=pid_calc(&pid_angle[0], (float)moto_chassis[0].total_angle/ 8192.0f * 360.0f-(float)moto_chassis[1].total_angle/ 8192.0f * 360.0f, 0.0f);
	//  PID �������ؼ���

	short torque1 = pid_calc(&pid_motor[0], (float)moto_chassis[0].speed_rpm, 0);
	// can_cmd_send(torque1, torque2);
	// ���ٶȼ���
	// angle_setspeed_2 = angle_speed_cacl(&moto_chassis[1],1, set_round2);
	// �������ǶȲ���� PID��� ʵ�ֵ���������
	// angle_setspeed_2=pid_calc(&pid_angle[1], (float)moto_chassis[1].total_angle/ 8192.0f * 360.0f-(float)moto_chassis[0].total_angle/ 8192.0f * 360.0f, 0.0f);

	//  PID �������ؼ���

	short torque2 = pid_calc(&pid_motor[1], (float)moto_chassis[1].speed_rpm, 0);
	// real_speed2 = moto_chassis[1].speed_rpm;
}


// MotorData_t motors[motor_num];

// /**
//  * @brief ��ʼ���������
//  */
// void init_motor_data() {
//     for (int i = 0; i < motor_num; i++) {
//         motors[i].real_total_angle = 0.0f;
//         motors[i].angle_setspeed = 0.0f;
//         motors[i].set_angle = 0.0f;
//         motors[i].actual_round = 0.0f;
//         motors[i].pid_angle = &pid_angle[i];
//     }
// }

// /**
//  * @brief ���ٶȼ���
//  *
//  * @param moto_chassis
//  * @param k
//  * @param set_round
//  * @return float
//  */
// float angle_speed_cacl(moto_measure_t moto_chassis[], u8 k, float set_round)
// {
//     MotorData_t* motor = &motors[k];
    
//     get_total_angle(&moto_chassis[k]);
//     // ���ٱ�3591/187  �����趨�ܽǶ�
//     motor->set_angle = (float)set_round * 360.0f * 3591.0f / 187.0f;
    
//     // ����ʵ���ܽǶ�
//     motor->real_total_angle = (float)moto_chassis[k].total_angle / 8192.0f * 360.0f;

//     motor->actual_round = (float)moto_chassis[k].total_angle / 8192.0f * 187.0f / 3591.0f;
    
//     // ͨ���ǶȻ������趨(��)�ٶ�
//     motor->angle_setspeed = pid_calc(motor->pid_angle, motor->real_total_angle, motor->set_angle);

//     return motor->angle_setspeed;
// }


// float real_total_angle[motor_num];
// float angle_setspeed[motor_num];
// float set_angle[motor_num];
// // float actual_round;

// /**
//  * @brief ���ٶȼ���
//  *
//  * @param set_round
//  * @return float
//  */
// float angle_speed_cacl(moto_measure_t moto_chassis[], u8 k, float set_round)
// {
    
//     set_angle[k] = set_round * 360.0f * 3591.0f / 187.0f;                  // ���ٱ�3591/187  �����趨�ܽǶ� 36.0f/1.0f
//     real_total_angle[k] = moto_chassis[k].total_angle / 8192.0f * 360.0f; // ����ʵ���ܽǶ�
//     actual_round[k] = (float)moto_chassis[k].total_angle / 8192.0f * 187.0f / 3591.0f;
//     angle_setspeed[k] = pid_calc(&pid_angle[k], real_total_angle[k], set_angle[k]); // ͨ���ǶȻ������趨(��)�ٶ�
//     return angle_setspeed[k];
// }





