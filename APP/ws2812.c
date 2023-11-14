/*
 * ws2812.c
 *
 *  Created on: Mar 27, 2023
 *      Author: mxia2
 */

#include "ws2812.h"
#include "spi.h"
#include "dma.h"

// ?????????????
const RGBColor_TypeDef RED = {30, 0, 0};
const RGBColor_TypeDef GREEN = {0, 50, 0};
const RGBColor_TypeDef BLUE = {0, 0, 100};
const RGBColor_TypeDef YELLOW = {30, 30, 0};
const RGBColor_TypeDef MAGENTA = {30, 0, 30};
const RGBColor_TypeDef BLACK = {0, 0, 0};
const RGBColor_TypeDef WHITE = {80, 80, 80};

// ??bit?:0xC0 ? 0,0xF8 ? 1
const uint8_t code[] = {0xC0, 0xF8};

// ??????????30???????
RGBColor_TypeDef RGB_DAT[RGB_NUM];

// SPI??????????24???????1??
extern DMA_HandleTypeDef hdma_spi1_tx;

static void SPI_Send(uint8_t *SPI_RGB_BUFFER)
{
    /* ????DMA??????? */
    while (HAL_DMA_GetState(&hdma_spi1_tx) != HAL_DMA_STATE_READY)
        ;
    /* ????(24bit)? RGB ??? 2812 */
    HAL_SPI_Transmit_DMA(&hspi1, SPI_RGB_BUFFER, 24);
}

// ????????? ID ? ??????????
void RGB_Set_Color(uint8_t LedId, RGBColor_TypeDef Color)
{
    if (LedId < RGB_NUM)
    {
        RGB_DAT[LedId].G = Color.G;
        RGB_DAT[LedId].R = Color.R;
        RGB_DAT[LedId].B = Color.B;
    }
}

// ??????????????WS2812?????????????
void RGB_Reflash(uint8_t reflash_num)
{
    static uint8_t RGB_BUFFER[24] = {0};
    uint8_t dat_b, dat_r, dat_g;
    // ???????? 24 ?????????
    if (reflash_num > 0 && reflash_num <= RGB_NUM)
    {
        for (int i = 0; i < reflash_num; i++)
        {
            dat_g = RGB_DAT[i].G;
            dat_r = RGB_DAT[i].R;
            dat_b = RGB_DAT[i].B;
            for (int j = 0; j < 8; j++)
            {
                RGB_BUFFER[7 - j] = code[dat_g & 0x01];
                RGB_BUFFER[15 - j] = code[dat_r & 0x01];
                RGB_BUFFER[23 - j] = code[dat_b & 0x01];
                dat_g >>= 1;
                dat_r >>= 1;
                dat_b >>= 1;
            }
            SPI_Send(RGB_BUFFER);
        }
    }
}

// ????
void RGB_RST(void)
{
    uint8_t dat[100] = {0};
    /* ????DMA??????? */
    while (HAL_DMA_GetState(&hdma_spi1_tx) != HAL_DMA_STATE_READY)
        ;
    /* RGB RESET */
    HAL_SPI_Transmit_DMA(&hspi1, dat, 100);
    HAL_Delay(10);
}

// ???????????

void RGB_RED(uint16_t RGB_LEN)
{
    uint8_t i;
    for (i = 0; i < RGB_LEN; i++)
    {
        RGB_Set_Color(i, RED);
    }
    RGB_Reflash(RGB_LEN);
}

void RGB_GREEN(uint16_t RGB_LEN)
{
    uint8_t i;
    for (i = 0; i < RGB_LEN; i++)
    {
        RGB_Set_Color(i, GREEN);
    }
    RGB_Reflash(RGB_LEN);
}

void RGB_BLUE(uint16_t RGB_LEN)
{
    uint8_t i;
    for (i = 0; i < RGB_LEN; i++)
    {
        RGB_Set_Color(i, BLUE);
    }
    RGB_Reflash(RGB_LEN);
}

void RGB_WHITE(uint16_t RGB_LEN)
{
    uint8_t i;
    for (i = 0; i < RGB_LEN; i++)
    {
        RGB_Set_Color(i, WHITE);
    }
    RGB_Reflash(RGB_LEN);
}