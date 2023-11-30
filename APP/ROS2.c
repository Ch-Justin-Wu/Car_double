/**
 * ************************************************************************
 * 
 * @file ROS2.c
 * @author Justin Wu (justin.wu@zllcrm.org)
 * @brief ros数据接收解析
 * @version 1.0
 * @date 2023-11-30
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2023 Justin Wu 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#include "ROS2.h"
#if defined(ROS)
Ros_cmd Ros2={0};


/**
 * ************************************************************************
 * @brief ros数据包解析
 * Data base:0xA5 vx1 vx2 vy1 vy2 ω1 ω2 0xA6
 * @param[in] ptr  ros数据结构体指针
 *
 * ************************************************************************
 */
void ros2_data_resolve(Ros_cmd* ptr)
{
    // 计数 count
    uint8_t temp_cnt = 0;

    // Takes the cached array address
    uint8_t *rx_p = rx_buffer1;

    // Find header 0xA5 with loop optimization
    while (*rx_p != 0xA5 && temp_cnt <= 9)
    {
        temp_cnt++;
        rx_p++;
    }
    // Determine the frame header and end of the frame
    if (rx_p[0] == 0xA5 && rx_p[7] == 0xA6)
    {
        ptr->Vx = (rx_p[1]) | (rx_p[2] << 8);
        ptr->Vy = (rx_p[3] ) | (rx_p[4]<< 8);
        ptr->Wz = ((rx_p[5] ) | (rx_p[6]<< 8));
    }
}

/**
 * ************************************************************************
 * @brief ros数据接收
 * 
 * 
 * ************************************************************************
 */
void ROS2_data_rx(void)
{
    if (recv_end_flag1 == 1 && rx_len1 == DATA_FRAME_LENGTH1)
    {
        ros2_data_resolve(&Ros2);

        // Clear the count
        //  清除计数
        rx_len1 = 0;
        // Clear the Receive End flag
        //  清除接收结束标志
        recv_end_flag1 = 0;

        // Clear the receive cache to set each byte in the receive buffer to 0
        // 清除接收缓存,将接收缓存区的每个字节都置0
        memset(rx_buffer1, 0, rx_len1);
    }
    // Turn DMA reception back on
    //  重新打开DMA接收
    HAL_UART_Receive_DMA(&c_huart1, rx_buffer1, BUF_SIZE1);
}


#endif
