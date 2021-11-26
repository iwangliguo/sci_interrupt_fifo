/*
 * sci_utils.c
 *
 *  Created on: Nov 25, 2021
 *      Author: Administrator
 */

#include "sci_utils.h"
#include "driverlib.h"

//SCI General Definition
uint16_t sci_send_buffer[SCI_BUFFER_SIZE];
uint16_t sci_rcv_buffer[SCI_BUFFER_SIZE];

uint16_t *sci_trans = sci_send_buffer;
uint16_t *sci_recev = sci_rcv_buffer;

uint16_t sci_tx_count = 0;
uint16_t sci_rx_count = 0;
uint16_t sci_rx_lenth = sizeof(sci_send_buffer);
uint16_t sci_tx_lenth = sizeof(sci_rcv_buffer);

//SCI Frame Definition
uint16_t crc_data_cal = 0;
uint16_t rcv_begin_flag = 0;
uint16_t rcv_crc_right_cnt = 0;
uint16_t rcv_right_flag = 0;
uint16_t rcv_crc_err_cnt = 0;
uint16_t rcv_state = 0;

//SCI String Definition
//���������ɱ�־�����յ��س����Ž������س�����\r\n��ASCII
//��ֱ�Ϊ10,13
uint16_t cmd_received = 0;

void SCI_receive_string(uint16_t receivedChar)
{
    //���Ϊ�˸��
    if( receivedChar == '\b')
    {
      //���ָ�벻������Ŀ�ʼλ��
      if(sci_rx_count)
      {
        SCI_writeCharBlockingFIFO(SCIA_BASE, ('\b'));
        SCI_writeCharBlockingFIFO(SCIA_BASE, (' '));
        SCI_writeCharBlockingFIFO(SCIA_BASE, ('\b'));
        sci_rx_count--;
        sci_rcv_buffer[sci_rx_count]=0x00;
      }
    }
    //��������˸��
    else
    {
      //��������������sci_rcv_buffer
      //���ҽ������һ��Ԫ�������������������д�����һ��Ԫ��Ϊֹ
      if(sci_rx_count < (SCI_BUFFER_SIZE - 1))
      {
        sci_rcv_buffer[sci_rx_count] =  receivedChar;
        sci_rcv_buffer[sci_rx_count + 1]=0x00;
        sci_rx_count++;
      }
      else
      {
        sci_rcv_buffer[sci_rx_count - 1] =  receivedChar;
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

#define  SCI_RCV_HEAD 0xFC
void SCI_receive_frame(uint16_t RcvData)
{
    if(sci_rx_count < (sci_rx_lenth - 1)){
        sci_recev[sci_rx_count] = RcvData;
    }
    sci_rx_count ++;
    switch(rcv_state){
        case 0:
            if(SCI_RCV_HEAD == RcvData){
                rcv_state = 0x01;
                rcv_begin_flag = 1;
                sci_rx_count = 1;
                sci_recev[sci_rx_lenth-1] = 0;
            }else{
                sci_rx_count = 0;
            }
        break;
        case 1:
            sci_recev[sci_rx_lenth-1] += sci_recev[sci_rx_count - 1];
            if(sci_rx_count == (sci_rx_lenth - 1)){
                rcv_state = 0x02;
            }
            crc_data_cal = sci_recev[sci_rx_lenth - 1];
        break;
        case 2:
            rcv_begin_flag = 0;
            if((sci_recev[sci_rx_lenth-1]&0x00FF) == RcvData){
                rcv_right_flag = 1;
                rcv_crc_right_cnt ++;
            }else{
                rcv_right_flag = 0;
                rcv_crc_err_cnt ++;
            }
            sci_recev[sci_rx_lenth-1] = RcvData;
            rcv_state = 0x00;
            sci_rx_count = 0;
        break;
    }
}
