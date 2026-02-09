/*
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************

  File:		  	   tcpClientRAW.c
  Modified By:     ControllersTech.com
  Updated:    	   29-Jul-2021

  ***************************************************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ***************************************************************************************************************
*/


/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Christiaan Simons rewrote this file to get a more stable  application.
 *
 **/

 /* This file was modified by ST */

#include "tcpClientRAW.h"
#include "main.h"
#include "lwip/tcp.h"
#include "cmsis_os.h"

/*  protocol states */
enum tcp_client_states
{
  ES_NONE = 0,
  ES_CONNECTED,
  ES_RECEIVING,
  ES_CLOSING
};

/* structure for maintaining connection infos to be passed as argument
   to LwIP callbacks*/
struct tcp_client_struct
{
  u8_t state;             /* current connection state */
  u8_t retries;
  struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
  struct pbuf *p;         /* pointer on the received/to be transmitted pbuf */
};

/* This callback will be called, when the client is connected to the server */
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);

/* This callback will be called, when the client receive data from the server */
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

/* This callback will be called, when the server Polls for the Client */
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);

/* This callback will be called, when the server acknowledges the data sent by the client */
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);

/* A Function to send the data to the server */
static void tcp_client_send(struct tcp_pcb *tpcb, struct tcp_client_struct *es);

/* Function to close the connection */
//static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct *es);
int tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct *es);

/* This is the part where we are going to handle the incoming data from the server */
static void tcp_client_handle (struct tcp_pcb *tpcb, struct tcp_client_struct *es);


int counter = 0;
uint8_t data[100];

#define Max_Count_Slave_DMA_Buffer_Write_Read	80

extern unsigned char BLE_Received_Data_Length[Max_Count_Slave_DMA_Buffer_Write_Read];
extern unsigned char Slave_receive_buff[Max_Count_Slave_DMA_Buffer_Write_Read][10];
extern uint8_t Flag_Slave_DMA_Buffer_Write;

extern uint32_t BLE_time_taken_for_graph;
extern uint32_t BLE_Display_Time_Execution;
//extern TIM_HandleTypeDef htim24;

extern TIM_HandleTypeDef htim1;

/* create a struct to store data */
struct tcp_client_struct *esTx = 0;
struct tcp_pcb *pcbTx = 0;

extern uint16_t Poll_Count;
extern err_t Poll_Error;
extern uint8_t Poll_State;

extern uint8_t Close_Location;

extern uint8_t  Slave_DMA_Tx_Stage;
extern _Bool Flag_TCP_Connected;
//extern uint8_t Temp_Server_Address;
//extern uint8_t Temp_Network_Address;
extern uint8_t BLE_Reply_Index;

extern int16_t Config_Parameters[43];
#define Alt_Server_Address_Byte3	32
#define Alt_Gateway_Address_Byte3	36

extern int8_t TCP_Connec_Return_Value;
extern _Bool Flag_TCP_Acknowledge_Enable;
extern _Bool Flag_Temp_Close_Connection;
extern _Bool Flag_PCB;

//extern uint16_t Temp_TCP_Receive_Count;

extern char Temp_Job_Name[25];
extern char Temp_File_Name[40];
extern uint16_t pcb_local_port;

extern struct tcp_pcb *tcp_active_pcbs;

extern osMessageQueueId_t TCP_ReadHandle;

//extern uint8_t Pre_Abort_Count, Post_Abort_Count, Abort_Count;

//uint16_t temp_command_count = 0;

void Send_New_Data(char* buf , int len)
{
	//char buf[100];

	/* Prepare the first message to send to the server */
	//int len = sprintf (buf, "Sending TCPclient Message %d\n", counter);

	if (counter !=0)
	{
		/* allocate pbuf */
		esTx->p = pbuf_alloc(PBUF_TRANSPORT, len , PBUF_POOL);

		/* copy data to pbuf */
		pbuf_take(esTx->p, (char*)buf, len);
		LOCK_TCPIP_CORE();
		tcp_client_send(pcbTx, esTx);
		UNLOCK_TCPIP_CORE();
		pbuf_free(esTx->p);
	}

}

/* IMPLEMENTATION FOR TCP CLIENT

1. Create TCP block.
2. connect to the server
3. start communicating
*/

int tcp_client_init(void)
{
	/* 1. create new tcp pcb */

	int temp_ret;

	struct tcp_pcb *tpcb;

	tpcb = tcp_new();

	if(tpcb == NULL)
		return ERR_MEM;

	/* 2. Connect to the server */
	ip_addr_t destIPADDR;
//	IP_ADDR4(&destIPADDR, 192, 168, Temp_Network_Address, Temp_Server_Address);

	IP_ADDR4(&destIPADDR, Config_Parameters[Alt_Server_Address_Byte3], Config_Parameters[Alt_Server_Address_Byte3 + 1], Config_Parameters[Alt_Server_Address_Byte3 + 2], Config_Parameters[Alt_Server_Address_Byte3 + 3]);

//	IP_ADDR4(&destIPADDR, 192, 168, 0, 30);
//	return(tcp_connect(tpcb, &destIPADDR, 5001, tcp_client_connected));
	temp_ret = tcp_connect(tpcb, &destIPADDR, 5001, tcp_client_connected);

	if(temp_ret != ERR_OK)	//Connect Failed
	{
		tcp_abort(tpcb);
		tpcb = NULL;
	}
	return(temp_ret);
}

void close_all_tcp_connections(void)
{
    struct tcp_pcb *pcb = tcp_active_pcbs;
    struct tcp_pcb *next;

    while (pcb != NULL)
    {
        next = pcb->next;

        // Close the connection gracefully
        err_t err = tcp_close(pcb);

        // If closing fails (e.g., data still in buffer), abort it
        if (err != ERR_OK)
        {
            tcp_abort(pcb);
        }
        pcb = next;
    }
}

//void tcp_Close_Connection(void)
int tcp_Close_Connection(void)
{
	int Ret;
	Ret = tcp_client_connection_close(pcbTx, esTx);
//	Pre_Abort_Count = Abort_Count;
	if(pcbTx != NULL)
	{
		tcp_abort(pcbTx);
		pcbTx = NULL;
	}
	close_all_tcp_connections();
	Flag_TCP_Connected = 0;
	return(Ret);
//	Close_Location = 5;
}

/** This callback is called, when the client is connected to the server
 * Here we will initialise few other callbacks
 * and in the end, call the client handle function
  */
static err_t tcp_client_connected(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  err_t ret_err;
  struct tcp_client_struct *es;

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  /* allocate structure es to maintain tcp connection information */
  es = (struct tcp_client_struct *)mem_malloc(sizeof(struct tcp_client_struct));
  if (es != NULL)
  {
    es->state = ES_CONNECTED;
    es->pcb = newpcb;
    es->retries = 0;
    es->p = NULL;

    /* pass newly allocated es structure as argument to newpcb */
    tcp_arg(newpcb, es);

    /* initialize lwip tcp_recv callback function for newpcb  */
    tcp_recv(newpcb, tcp_client_recv);

    /* initialize lwip tcp_poll callback function for newpcb */
    tcp_poll(newpcb, tcp_client_poll, 2);

    /* initialize LwIP tcp_sent callback function */
    tcp_sent(newpcb, tcp_client_sent);

    /* handle the TCP data */
    tcp_client_handle(newpcb, es);

    ret_err = ERR_OK;
    Flag_TCP_Connected = 1;
  }
  else
  {
    /*  close tcp connection */
    tcp_client_connection_close(newpcb, es);
    Close_Location = 1;
    /* return memory error */
    ret_err = ERR_MEM;
  }
  return ret_err;
}


/** This callback is called, when the client receives some data from the server
 * if the data received is valid, we will handle the data in the client handle function
  */
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct tcp_client_struct *es;
  err_t ret_err;

  LWIP_ASSERT("arg != NULL",arg != NULL);

  es = (struct tcp_client_struct *)arg;

  /* if we receive an empty tcp frame from server => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    if(es->p == NULL)
    {
       /* we're done sending, close connection */
       tcp_client_connection_close(tpcb, es);
       Close_Location = 2;
    }
    else
    {
      /* we're not done yet */
//      /* acknowledge received packet */
//      tcp_sent(tpcb, tcp_client_sent);

      /* send remaining data*/
//      tcp_client_send(tpcb, es);
    }
    ret_err = ERR_OK;
  }
  /* else : a non empty frame was received from server but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      es->p = NULL;
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_CONNECTED)
  {
   /* store reference to incoming pbuf (chain) */
    es->p = p;

    // tcp_sent has already been initialized in the beginning.
//    /* initialize LwIP tcp_sent callback function */
//    tcp_sent(tpcb, tcp_client_sent);	//19.12.24

    /* Acknowledge the received data */
    tcp_recved(tpcb, p->tot_len);

    if(osMessageQueueGetSpace(TCP_ReadHandle) != 0)
    {
        tcp_queue *msgr = pvPortMalloc(sizeof(tcp_queue));
        if(msgr != NULL)
        {
      	  msgr->length = p->tot_len;
      	  msgr->data = (uint8_t *)pvPortMalloc(p->tot_len);
      	  if(msgr->data == NULL)
      	  {
      		vPortFree(msgr);
      	  }
      	  pbuf_copy_partial(p,msgr->data,p->tot_len,0);
//     	  memcpy(msgr->data, p->payload, p->tot_len);

    	  if(osMessageQueuePut(TCP_ReadHandle, &msgr, 0, 0) != 0)
    	  {
    		  vPortFree(msgr->data);
    		  vPortFree(msgr);
    	  }
        }
    }

    /* handle the received data */
//    tcp_client_handle(tpcb, es);

    pbuf_free(p);

    ret_err = ERR_OK;
  }
  else if(es->state == ES_CLOSING)
  {
    /* odd case, remote side closing twice, trash data */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  else
  {
    /* unknown es->state, trash data  */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}


static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct tcp_client_struct *es;

  Poll_Count++;

  es = (struct tcp_client_struct *)arg;
  if (es != NULL)
  {
    if (es->p != NULL)
    {
        // tcp_sent has already been initialized in the beginning.
//      tcp_sent(tpcb, tcp_client_sent);
      /* there is a remaining pbuf (chain) , try to send data */
//      tcp_client_send(tpcb, es);
    }
    else
    {
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLOSING)
      {
        /*  close tcp connection */
        tcp_client_connection_close(tpcb, es);
        Close_Location = 3;
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }

  Poll_State = es->state;
  Poll_Error = ret_err;
  return ret_err;
}

/** This callback is called, when the server acknowledges the data sent by the client
 * If there is no more data left to sent, we will simply close the connection
  */
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct tcp_client_struct *es;

  LWIP_UNUSED_ARG(len);

  es = (struct tcp_client_struct *)arg;
  es->retries = 0;

//	if(Slave_DMA_Tx_Stage == 2)
//	{
//		BLE_time_taken_for_graph = __HAL_TIM_GET_COUNTER(&htim24);		//POSITION 1
//		BLE_Display_Time_Execution = BLE_time_taken_for_graph;
//		Slave_DMA_Tx_Stage 	= 0;
		BLE_Reply_Index++;
//	}


  if(es->p != NULL)
  {
//	  BLE_time_taken_for_graph = __HAL_TIM_GET_COUNTER(&htim24);		//POSITION 3
	// tcp_sent has already been initialized in the beginning.
    /* still got pbufs to send */
//    tcp_sent(tpcb, tcp_client_sent);


//    tcp_client_send(tpcb, es);
  }
  else
  {
    /* if no more data to send and client closed connection*/
    if(es->state == ES_CLOSING)
    {
      tcp_client_connection_close(tpcb, es);
      Close_Location = 4;
//      BLE_time_taken_for_graph = __HAL_TIM_GET_COUNTER(&htim24);		//POSITION 2
    }
  }
  return ERR_OK;
}


/** A function to send the data to the server
  */
static void tcp_client_send(struct tcp_pcb *tpcb, struct tcp_client_struct *es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;

  while ((wr_err == ERR_OK) &&
         (es->p != NULL) &&
         (es->p->len <= tcp_sndbuf(tpcb)))
  {

    /* get pointer on pbuf from es structure */
    ptr = es->p;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

//    alt_tcp_wr_count++;
//    alt_tcp_wr_err = wr_err;

    if (wr_err == ERR_OK)
    {
      u16_t plen;
      u8_t freed;

      plen = ptr->len;

      /* continue with next pbuf in chain (if any) */
      es->p = ptr->next;

      if(es->p != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(es->p);
      }

     /* chop first pbuf from chain */
      do
      {
        /* try hard to free pbuf */
        freed = pbuf_free(ptr);
      }
      while(freed == 0);
     /* we can read more data now */
//     tcp_recved(tpcb, plen);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later / harder, defer to poll */
     es->p = ptr;
   }
   else
   {
     /* other problem ?? */
   }

//   if(wr_err != ERR_OK)
//   {
//	   alt_tcp_wr_fail_count++;
//	   if(alt_tcp_wr_fail_count >= alt_tcp_wr_fail_count_limit)
//	   {
//		   alt_tcp_wr_fail_count = 0;
//		   Flag_Temp_Close_Connection = 1;
//	   }
//   }
//   else
//   {
//	   alt_tcp_wr_fail_count = 0;
//   }
  }

//DISABLED 26.11.2024
//  if(wr_err != ERR_OK)
//  {
//	   alt_tcp_wr_fail_count++;
//	   if(alt_tcp_wr_fail_count >= alt_tcp_wr_fail_count_limit)
//	   {
//		   alt_tcp_wr_fail_count = 0;
//		   Flag_Temp_Close_Connection = 1;
//	   }
//  }
//  else
//  {
//	   alt_tcp_wr_fail_count = 0;
//  }

}


//static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct *es)
int tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct *es)
{
  int Ret_err;
  /* remove all callbacks */
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);

  /* delete es structure */
  if (es != NULL)
  {
    mem_free(es);
  }

  /* close tcp connection */
  Ret_err = tcp_close(tpcb);
  return(Ret_err);
//  alt_tcp_wr_fail_count = 0;
}

/* Handle the incoming TCP Data */

static void tcp_client_handle (struct tcp_pcb *tpcb, struct tcp_client_struct *es)
{
	/* get the Remote IP */
	ip4_addr_t inIP = tpcb->remote_ip;
	uint16_t inPort = tpcb->remote_port;

	/* Extract the IP */
	char *remIP = ipaddr_ntoa(&inIP);

//	esTx->state = es->state;
//	esTx->pcb = es->pcb;
//	esTx->p = es->p;

	esTx = es;
	pcbTx = tpcb;

	counter++;

}
