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

//���������ɱ�־�����յ��س����Ž������س�����\r\n��ASCII
//��ֱ�Ϊ10,13
uint16_t cmd_received = 0;

void SCI_readstring(uint16_t receivedChar)
{
    //���Ϊ�˸��
    if( receivedChar == '\b')
    {
      //���ָ�벻������Ŀ�ʼλ��
      if(scia_rx_count)
      {
        SCI_writeCharBlockingFIFO(SCIA_BASE, ('\b'));
        SCI_writeCharBlockingFIFO(SCIA_BASE, (' '));
        SCI_writeCharBlockingFIFO(SCIA_BASE, ('\b'));
        scia_rx_count--;
        scia_rcv_buffer[scia_rx_count]=0x00;
      }
    }
    //��������˸��
    else
    {
      //��������������scia_rcv_buffer
      //���ҽ������һ��Ԫ�������������������д�����һ��Ԫ��Ϊֹ
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
      //���Ϊ�س�������ʼ����������
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
