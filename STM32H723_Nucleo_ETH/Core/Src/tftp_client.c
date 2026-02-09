/*
 * tftp_client.c
 *
 *  Created on: Apr 10, 2025
 *      Author: RnD02
 */

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "lwip.h"
#include "lwip/udp.h"
#include "lwip/opt.h"
#include "tftp_client.h"
#include "flash_if.h"
#include "tftp_server.h"
#include "lwip/timeouts.h"

#include <string.h>
/* Private typedef -----------------------------------------------------------*/
struct udp_pcb *tftp_client_pcb;
extern struct netif gnetif;
extern int tsize;
extern _Bool Flag_OTA_File_Receive_Completed;
extern _Bool Flag_TFTP_Error;
extern int16_t Config_Parameters[43];
#define Alt_Server_Address_Byte3	32
extern char OTA_File_Name[31];

enum tftp_error {
  TFTP_ERROR_FILE_NOT_FOUND    = 1,
  TFTP_ERROR_ACCESS_VIOLATION  = 2,
  TFTP_ERROR_DISK_FULL         = 3,
  TFTP_ERROR_ILLEGAL_OPERATION = 4,
  TFTP_ERROR_UNKNOWN_TRFR_ID   = 5,
  TFTP_ERROR_FILE_EXISTS       = 6,
  TFTP_ERROR_NO_SUCH_USER      = 7
};

/* Private define ------------------------------------------------------------*/
#define TFTP_MAX_DATA		512
#define TFTP_HEADER			4

#define FW_FILE					"OTA_FILE.bin"
//#define FW_FILE				"Screenshot (9).png"
#define TFTP_BLKSIZE_OPTION		"blksize"
#define TFTP_BLKSIZE_VALUE      "512"
#define TFTP_TSIZE_OPTION       "tsize"
#define TFTP_TSIZE_VALUE        "0"

#define TFTP_RRQ   		1
#define TFTP_WRQ   		2
#define TFTP_DATA  		3
#define TFTP_ACK   		4
#define TFTP_ERROR 		5
#define TFTP_OACK		6

#define TFTP_RRQ_INTERVAL_MS 1000

/* Private variables ---------------------------------------------------------*/
ip_addr_t tftp_server_ip;
extern _Bool oack_received;
extern _Bool TFTP_CON;
u16_t last_blknum = 0;

/* Private function prototypes -----------------------------------------------*/
int parse_tsize_from_oack(uint8_t* oack_packet, uint16_t len) {
    uint16_t i = 2;
    while (i < len)
    {
        char* option_name = (char*)&oack_packet[i];
        int option_name_len = strlen(option_name);
        i += option_name_len + 1;

        if (i >= len)
        	break;

        char* option_value = (char*)&oack_packet[i];
        int option_value_len = strlen(option_value);
        i += option_value_len + 1;

        if (strcmp(option_name, "tsize") == 0)
        {
            return atoi(option_value);
        }
    }
    return -1; // Not found
}

//void send_rrq_periodic(void *arg)
void send_rrq_periodic(void)
{
//    if (!oack_received)
    {
        send_req();  // your RRQ construction + udp_sendto logic
        printf("RRQ sent, waiting for OACK...\n");

        // Re-arm the timer again after 1 sec
//        sys_timeout(TFTP_RRQ_INTERVAL_MS, send_rrq_periodic, NULL);
    }
}

int tftp_write(u16_t blknum, struct pbuf* p)
{
	static uint32_t flash_write_address = ADDRESS_FOR_OTA;
    uint8_t buffer[TFTP_MAX_DATA];
    uint32_t ramsource = (uint32_t)buffer;
    uint32_t datalen = (p->tot_len + 3) / 4;

    FLASH_If_Init();
    pbuf_copy_partial(p, buffer, p->tot_len, 0);

    if((blknum == 1) && (last_blknum == 0))
    {
        if (FLASH_If_Erase(flash_write_address, 3) != FLASHIF_OK)
        {
            printf("Flash erase failed!\n");
            return -1;
        }
        else
        	printf("\r----------Successfully Flash Erased----------\n");
    }

    if(blknum > last_blknum)
    {
        if (FLASH_If_Write(flash_write_address, (uint32_t*)ramsource, datalen) == FLASHIF_OK)
        {
            printf("Block %d written to flash at address 0x%08lX\n", blknum, flash_write_address);
            flash_write_address += p->tot_len;
            last_blknum = blknum;
            osDelay(10);
        }
        else
        {
            printf("Flash write failed at 0x%08lX\n", flash_write_address);
            return -1;
        }
    }
    else
    	printf("\rDuplicate Block: IGNORED!\n");

    return 0; // 0 = success
}

void tftp_client_close(void)
{
    // Remove the UDP PCB to stop receiving further packets
    if (tftp_client_pcb) {
        udp_remove(tftp_client_pcb);
        tftp_client_pcb = NULL;

        oack_received = 0;
    }
    printf("\rTFTP client session closed.\n");
}


void tftp_init_client(void)
{
    err_t err;

    tftp_client_pcb = udp_new();

    if(!tftp_client_pcb)
    {
    	printf("\rMemory not allocated for the TFTP Client.\n");
    	return;
    }

    err = udp_bind(tftp_client_pcb, IP_ADDR_ANY, 0);
    if(err != ERR_OK)
    {
    	printf("\rFAILED to bind UDP PCB for TFTP Client.\n");
    	udp_remove(tftp_client_pcb);
    	tftp_client_pcb = NULL;
    	return;
    }
    else
    {
    	TFTP_CON = 1;
    }

    udp_recv(tftp_client_pcb, tftp_recv_callback, NULL);

    printf("\rTFTP Client initialized.\n");
    printf("\rIP Address: %s\tPort: %d\n", ip_ntoa(&gnetif.ip_addr), tftp_client_pcb->local_port);

//    IP_ADDR4(&tftp_server_ip, 192, 168, 0, 20);
//    IP_ADDR4(&tftp_server_ip, 192, 168, 0, 16);
    IP_ADDR4(&tftp_server_ip, Config_Parameters[Alt_Server_Address_Byte3],Config_Parameters[Alt_Server_Address_Byte3 + 1],Config_Parameters[Alt_Server_Address_Byte3 + 2],Config_Parameters[Alt_Server_Address_Byte3 + 3]);
    printf("\rTemp_Test.\n");

//  send_rrq_periodic(NULL);
}

//void send_req(void)
//{
//    struct pbuf *p;
//    char buffer[300];  // enough to hold RRQ packet
//    int len = 0;
//
//    // TFTP Opcode (2 bytes): 1 for RRQ
//    buffer[len++] = 0;
//    buffer[len++] = TFTP_RRQ;
//
//    // Filename
//    strcpy(&buffer[len], FW_FILE);
//    len += strlen(FW_FILE) + 1;
//
//    // Mode: "octet"
//    strcpy(&buffer[len], "octet");
//    len += strlen("octet") + 1;
//
//    // Option: blksize
//    strcpy(&buffer[len], TFTP_BLKSIZE_OPTION);
//    len += strlen(TFTP_BLKSIZE_OPTION) + 1;
//
//    strcpy(&buffer[len], TFTP_BLKSIZE_VALUE);
//    len += strlen(TFTP_BLKSIZE_VALUE) + 1;
//
//    // Option: tsize
//    strcpy(&buffer[len], TFTP_TSIZE_OPTION);
//    len += strlen(TFTP_TSIZE_OPTION) + 1;
//
//    strcpy(&buffer[len], TFTP_TSIZE_VALUE);
//    len += strlen(TFTP_TSIZE_VALUE) + 1;
//
//    // Allocate pbuf
//    p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
//    if (p == NULL) {
//        printf("Failed to allocate pbuf for RRQ\n");
//        return;
//    }
//
//    // Fill pbuf with data
//    pbuf_take(p, buffer, len);
//
//    // Send the request to the server's port 69
//    err_t err = udp_sendto(tftp_client_pcb, p, &tftp_server_ip, TFTP_PORT);
//    if (err != ERR_OK) {
//        printf("Failed to send TFTP RRQ: %d\n", err);
//    } else {
//        printf("TFTP Read Request sent to -> %s for file: %s\n", ip_ntoa(&tftp_server_ip), FW_FILE);
//    }
//}

void send_req(void)
{
    struct pbuf *p;
    char buffer[300];  // enough to hold RRQ packet
    int len = 0;

    // TFTP Opcode (2 bytes): 1 for RRQ
    buffer[len++] = 0;
    buffer[len++] = TFTP_RRQ;

    // Filename
    strcpy(&buffer[len], OTA_File_Name);
    len += strlen(OTA_File_Name) + 1;

    // Mode: "octet"
    strcpy(&buffer[len], "octet");
    len += strlen("octet") + 1;

    // Option: blksize
    strcpy(&buffer[len], TFTP_BLKSIZE_OPTION);
    len += strlen(TFTP_BLKSIZE_OPTION) + 1;

    strcpy(&buffer[len], TFTP_BLKSIZE_VALUE);
    len += strlen(TFTP_BLKSIZE_VALUE) + 1;

    // Option: tsize
    strcpy(&buffer[len], TFTP_TSIZE_OPTION);
    len += strlen(TFTP_TSIZE_OPTION) + 1;

    strcpy(&buffer[len], TFTP_TSIZE_VALUE);
    len += strlen(TFTP_TSIZE_VALUE) + 1;

    // Allocate pbuf
    p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
    if (p == NULL) {
        printf("Failed to allocate pbuf for RRQ\n");
        return;
    }

    // Fill pbuf with data
    pbuf_take(p, buffer, len);

    // Send the request to the server's port 69
    err_t err = udp_sendto(tftp_client_pcb, p, &tftp_server_ip, TFTP_PORT);
    if (err != ERR_OK) {
        printf("Failed to send TFTP RRQ: %d\n", err);
    } else {
        printf("TFTP Read Request sent to -> %s for file: %s\n", ip_ntoa(&tftp_server_ip), OTA_File_Name);
    }
}

void tftp_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    u16_t *sbuf = (u16_t *)p->payload;
    u16_t opcode = lwip_ntohs(sbuf[0]);
    u16_t block = 0;

    switch (opcode) {
        case TFTP_OACK: {
            printf("Received OACK from server\n");
//        	Send_String_UART_232("Received OACK from server\n");
            oack_received = 1;

            /*Parsing File size from the option ack.*/
            tsize = parse_tsize_from_oack((uint8_t*)p->payload, p->tot_len);

//            sys_untimeout(send_rrq_periodic, NULL);

            send_ack(block, tftp_client_pcb, addr, port);
            break;
        }

        case TFTP_DATA: {
        	int ret;
            u16_t block = lwip_ntohs(sbuf[1]);

            printf("\rDATA PACKET: %d\n", block);
//            Send_String_UART_232("DATA PACKET\n");

            pbuf_remove_header(p, 4);

            ret = tftp_write(block, p);
            if(ret != 0)
            {
            	printf("\rFAIL: Writing Data in Flash\n");
//            	Send_String_UART_232("FAIL: Writing Data in Flash\n");
            	tftp_client_close();
                Flag_TFTP_Error = 1;
            	//move to TCP
            }

           osDelay(10);

            send_ack(block, pcb, addr, port);

            if (p->tot_len < TFTP_MAX_DATA)
            {
                printf("Received final block, transfer complete.\n");
//                Send_String_UART_232("Received final block, transfer complete.\n");
                tftp_client_close();
                HAL_GPIO_WritePin(GPIOB, LED_GREEN_Pin, SET);
                Flag_OTA_File_Receive_Completed = 1;
            }
            break;
        }

        case TFTP_ERROR: {
            // Handle error from server
            printf("Received TFTP ERROR from server\n");
//            Send_String_UART_232("Received TFTP ERROR from server\n");
//            HAL_Delay(2000);
//            NVIC_SystemReset();

            //move to TCP
            tftp_client_close();
            Flag_TFTP_Error = 1;
            break;
        }

        default:
            printf("Unknown opcode: %d\n", opcode);
//            Send_String_UART_232("Unknown opcode\n");
//            HAL_Delay(2000);
//            NVIC_SystemReset();

            //move to TCP
            tftp_client_close();
            Flag_TFTP_Error = 1;
            break;
    }

    pbuf_free(p); // Free the incoming pbuf
}

void send_ack(uint16_t blknum, struct udp_pcb *pcb, const ip_addr_t *addr, u16_t port)
{
    struct pbuf *ack_buf = pbuf_alloc(PBUF_TRANSPORT, TFTP_HEADER, PBUF_RAM);

    if (ack_buf != NULL) {
        uint16_t *ack_payload = (uint16_t *)ack_buf->payload;
        ack_payload[0] = lwip_htons(TFTP_ACK);
        ack_payload[1] = lwip_htons(blknum);  // block 0 for OACK

        err_t err = udp_sendto(pcb, ack_buf, addr, port);
        if (err == ERR_OK) {
        	printf("\rACK Sent for Block: %d\n", blknum);
        }

        pbuf_free(ack_buf); // Free after sending
    }
}
