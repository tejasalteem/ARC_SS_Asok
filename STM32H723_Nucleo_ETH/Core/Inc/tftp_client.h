/*
 * tftp_client.h
 *
 *  Created on: Apr 10, 2025
 *      Author: RnD02
 */

#ifndef INC_TFTP_CLIENT_H_
#define INC_TFTP_CLIENT_H_

#include "lwip/udp.h"

void send_rrq_periodic(void);
void send_req(void);
void tftp_init_client(void);
void tftp_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void tftp_client_cleanup(void);
void send_ack(uint16_t blknum, struct udp_pcb *pcb, const ip_addr_t *addr, u16_t port);
void tftp_client_close(void);

#endif /* INC_TFTP_CLIENT_H_ */
