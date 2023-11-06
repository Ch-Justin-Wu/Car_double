#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
/* Includes ------------------------------------------------------------------*/
#include "bsp_usart.h"
#include "usart.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

// ���尴ť״̬��ö��
enum ButtonState
{
    BUTTON_NOT_PRESSED,
    BUTTON_PRESSED
};

// Remote control data structure
// ң�������ݽṹ��
typedef __packed struct
{
    // Joystick horizontal stroke[0] + vertical stroke[1]
    // ҡ�� ˮƽ�г�[0]+��ֱ�г�[1]
    uint16_t L_Joystick[2];
    uint16_t R_Joystick[2];

    uint16_t L_Joystick_Hor;
    uint16_t L_Joystick_Ver;
    uint16_t R_Joystick_Hor;
    uint16_t R_Joystick_Ver;
    // trigger
    // ���
    uint16_t L_Trigger;
    uint16_t R_Trigger;
    // Buttons
    // ����
    uint8_t A;
    uint8_t B;
    uint8_t X;
    uint8_t Y;
    uint8_t LB;
    uint8_t RB;
    uint8_t View;
    uint8_t Menu;
    uint8_t Xbox;
    uint8_t Share;
    // Joystick buttons
    // ҡ�˰���
    uint8_t press_L;
    uint8_t press_R;
    // Joystick input combination
    // �������
    uint8_t combination;
} Controller_t;

extern Controller_t xbox_t;

void DMA_Usart_Rx(uint8_t *Data, uint8_t len);

void controller_data(void);
#endif
