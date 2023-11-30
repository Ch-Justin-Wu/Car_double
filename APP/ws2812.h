/**
 * ************************************************************************
 * 
 * @file ws2812.h
 * @author Justin Wu (justin.wu@zllcrm.org)
 * @brief ws2812����ɫ�ṹ�嶨�壬��غ�������
 * @version 1.0
 * @date 2023-11-30
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2023 Justin Wu 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#pragma once

#include "main.h"

typedef struct // ��ɫ�ṹ��
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
} RGBColor_TypeDef;

#define RGB_NUM 30 // RGB�Ƶ���������Ϊ����������

// ��λ����
void RGB_RST(void);
// ��ɫ���ú���
void RGB_Set_Color(uint8_t LedId, RGBColor_TypeDef Color);
// RGB ˢ�º���
void RGB_Reflash(uint8_t reflash_num);

// ������ɫ����
void RGB_RED(uint16_t RGB_LEN);		// ��
void RGB_GREEN(uint16_t RGB_LEN);	// ��
void RGB_BLUE(uint16_t RGB_LEN);	// ��
void RGB_YELLOW(uint16_t RGB_LEN);	// ��
//void RGB_MAGENTA(uint16_t RGB_LEN); // ��
//void RGB_BLACK(uint16_t RGB_LEN);	// ��
void RGB_WHITE(uint16_t RGB_LEN);	// ��
void RGB_SKY(uint16_t RGB_LEN);		// ����
void RGB_ORANGE(uint16_t RGB_LEN);	// ��
							/* __WS2812_H */