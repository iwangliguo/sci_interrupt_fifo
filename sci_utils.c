/*
 * sci_utils.c
 *
 *  Created on: Nov 25, 2021
 *      Author: Administrator
 */

#include "sci_utils.h"
#include "driverlib.h"

uint16_t scia_send_buffer[SCIA_BUFFER_SIZE];
uint16_t scia_rcv_buffer[SCIA_BUFFER_SIZE];

uint16_t *scia_trans = scia_send_buffer;
uint16_t *scia_recev = scia_rcv_buffer;

uint16_t scia_tx_count = 0;
uint16_t scia_rx_count = 0;
uint16_t scia_rx_lenth = 0;
uint16_t scia_tx_lenth = 0;

//命令接收完成标志，接收到回车符号结束，回车换行\r\n的ASCII
//码分别为10,13
uint16_t cmd_received = 0;

void SCI_readstring(uint16_t receivedChar)
{
    //如果为退格键
    if( receivedChar == '\b')
    {
      //如果指针不在数组的开始位置
      if(scia_rx_count)
      {
        SCI_writeCharBlockingFIFO(SCIA_BASE, ('\b'));
        SCI_writeCharBlockingFIFO(SCIA_BASE, (' '));
        SCI_writeCharBlockingFIFO(SCIA_BASE, ('\b'));
        scia_rx_count--;
        scia_rcv_buffer[scia_rx_count]=0x00;
      }
    }
    //如果不是退格键
    else
    {
      //将数据填入数组scia_rcv_buffer
      //并且将后面的一个元素清零如果数组满了则写入最后一个元素为止
      if(scia_rx_count < (SCIA_BUFFER_SIZE - 1))
      {
        scia_rcv_buffer[scia_rx_count] =  receivedChar;
        scia_rcv_buffer[scia_rx_count + 1]=0x00;
        scia_rx_count++;
      }
      else
      {
        scia_rcv_buffer[scia_rx_count - 1] =  receivedChar;
        SCI_writeCharBlockingFIFO(SCIA_BASE, ('\b'));
      }
      //如果为回车键，则开始处理串口数据
      if( receivedChar == 13 ||  receivedChar == 10)
      {
          cmd_received = 1;
      }
      else
      {
        SCI_writeCharBlockingFIFO(SCIA_BASE, receivedChar);
      }
    }
}
