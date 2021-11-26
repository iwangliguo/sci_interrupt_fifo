/*
 * sci_utils.h
 *
 *  Created on: Nov 25, 2021
 *      Author: Administrator
 */

#ifndef SCI_UTILS_H_
#define SCI_UTILS_H_

#include <stdint.h>

#define  SCI_BUFFER_SIZE   6
#define  SCI_RCV_HEAD 0xFC

extern uint16_t sci_send_buffer[];
extern uint16_t sci_rcv_buffer[];
extern uint16_t *sci_trans;
extern uint16_t *sci_recev;

extern uint16_t sci_tx_count;
extern uint16_t sci_rx_count;
extern uint16_t sci_rx_lenth;
extern uint16_t sci_tx_lenth;


extern uint16_t crc_data_cal;
extern uint16_t rcv_begin_flag;
extern uint16_t rcv_crc_right_cnt;
extern uint16_t rcv_right_flag;
extern uint16_t rcv_crc_err_cnt;
extern uint16_t rcv_state;

extern uint16_t cmd_received;

void SCI_receive_string(uint16_t receivedChar);
void SCI_receive_frame(uint16_t RcvData);


#endif /* SCI_UTILS_H_ */
