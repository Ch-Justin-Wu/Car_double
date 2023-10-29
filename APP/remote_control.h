#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
/* Includes ------------------------------------------------------------------*/
#include "bsp_usart.h"
#include "usart.h"

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
    // trigger
    // ���
    uint16_t L_Trigger;
    uint16_t R_Trigger;
    // Buttons
    // ����
    enum ButtonState buttons[12]; // ʹ��ö�����ʹ洢��ť״̬

    // Joystick input combination
    // �������
    uint8_t combination;
} Controller_t;

extern Controller_t xbox_t;

void DMA_Usart_Rx(uint8_t *Data, uint8_t len);
void Data_Resolve(Controller_t *ptr);
void controller_data(void);
#endif
