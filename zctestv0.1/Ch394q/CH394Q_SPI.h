/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_SPI.h
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2025/07/01
 * Description        : CH394Q SPI header file (dual-channel refactored)
 *******************************************************************************/
#ifndef CH394Q_SPI_H_
#define CH394Q_SPI_H_
#include "Bsp.h"
#include "CH394Q_DHCP.h" /* for DHCP_Config_t, DHCP_Msg_t */
#include "CH394Q_DNS.h"  /* for DNS_Config_t, DNS_BUFFER_SIZE */

#define SPI_MODE 1
#define SPI_DMA_MODE 0

#define IPv6_MODE 0 // Warning!!!  Must be noted, switch to IPv6 mode, mode is irreversible

/* ===================== Dual-Channel Device Context ===================== */
/* One struct per CH394Q chip. g_cur_dev points to the active device.
 * In bare-metal single-threaded main loop, set g_cur_dev before calling
 * any CH394Q function for that channel. */

typedef struct
{
    /* --- SPI hardware --- */
    SPI_HandleTypeDef *spi; /* &hspi1 or &hspi2 */
    SPI_TypeDef *spi_reg;   /* SPI1 or SPI2 instance (for register-level access) */

    /* --- GPIO pins --- */
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef *rst_port;
    uint16_t rst_pin;
    GPIO_TypeDef *int_port; /* optional INT pin */
    uint16_t int_pin;

    /* --- Network config (was global in CH394Q_NET.c) --- */
    uint8_t mac[6];
    uint8_t ip[4];
    uint8_t mask[4];
    uint8_t gw[4];
    uint16_t port;
    uint8_t socketmode[8];
    uint8_t mybuffer[8][2048]; /* socket RX buffer */

    /* --- DHCP state (was static in CH394Q_DHCP.c) --- */
    uint8_t dhcp_status;
    uint32_t dhcp_xid;
    uint8_t dhcp_retries;
    uint32_t dhcp_timer;
    DHCP_Config_t dhcp_config;
    uint8_t dhcp_server_ip[4];
    uint8_t dhcp_buffer[1024];
    uint8_t dhcp_initialized;
    uint32_t dhcp_last_print_time;
    uint8_t dns_addr[4]; /* CH394Q_DNSAddr */

    /* --- DNS state (was static in CH394Q_DNS.c) --- */
    uint16_t dns_xid;
    uint8_t dns_retries;
    uint32_t dns_timer;
    uint8_t dns_buffer[DNS_BUFFER_SIZE];
    uint8_t dns_state;
    uint8_t dns_socket_init;
    uint8_t dns_timer_init;
    uint8_t dns_server_addr[4];
    DNS_Config_t dns_config;
    uint32_t dns_last_print_time;

    /* --- Interrupt flag --- */
    volatile uint8_t stCH394Q_IntUB;

} CH394Q_Dev_t;

/* Global pointer to the currently active device.
 * Set this before calling any CH394Q function for a specific channel. */
extern CH394Q_Dev_t *g_cur_dev;

/* ===================== CS control (uses g_cur_dev) ===================== */
/* STM32F4: BSRR low 16-bit = Set(HIGH), BSRR high 16-bit = Reset(LOW) */
/* CS??(START)  ? ? BSRR ?16?  = pin << 16 */
/* CS??(END)    ? ? BSRR ?16?  = pin       */
#define SPICH394QStart() (g_cur_dev->cs_port->BSRR = ((uint32_t)g_cur_dev->cs_pin << 16))
#define SPIEndCH394Q() (g_cur_dev->cs_port->BSRR = (uint32_t)g_cur_dev->cs_pin)
#define Execute_IPv6() delay_us(1)

/* ===================== Per-device variable redirects ===================== */
/* These macros redirect old global/static variable names to g_cur_dev members,
 * so existing code bodies in CH394Q_NET.c, CH394Q_DHCP.c, CH394Q_DNS.c stay unchanged. */

/* NET.c globals */
#define CH394Q_MACAddr (g_cur_dev->mac)
#define CH394Q_IPAddr (g_cur_dev->ip)
#define CH394Q_GWIPAddr (g_cur_dev->gw)
#define CH394Q_IPMask (g_cur_dev->mask)
#define CH394Q_Port (g_cur_dev->port)
#define socketmode (g_cur_dev->socketmode)
#define MyBuffer (g_cur_dev->mybuffer)
#define CH394Q_DNSAddr (g_cur_dev->dns_addr)

/* DHCP.c statics */
#define DHCP_Status (g_cur_dev->dhcp_status)
#define DHCP_Xid (g_cur_dev->dhcp_xid)
#define DHCP_Retries (g_cur_dev->dhcp_retries)
#define DHCP_Timer (g_cur_dev->dhcp_timer)
#define DHCP_Config (g_cur_dev->dhcp_config)
#define Server_IP (g_cur_dev->dhcp_server_ip)
#define DHCP_Buffer (g_cur_dev->dhcp_buffer)
#define DHCP_Msg ((DHCP_Msg_t *)(g_cur_dev->dhcp_buffer))
#define Initialization_Flag (g_cur_dev->dhcp_initialized)
#define Last_Print_Time (g_cur_dev->dhcp_last_print_time)

/* DNS.c statics */
#define DNS_Xid (g_cur_dev->dns_xid)
#define DNS_Retries (g_cur_dev->dns_retries)
#define DNS_Timer (g_cur_dev->dns_timer)
#define DNS_Buffer (g_cur_dev->dns_buffer)
#define DNS_State (g_cur_dev->dns_state)
#define DNS_Socket_Initialized (g_cur_dev->dns_socket_init)
#define Timer_Initialized (g_cur_dev->dns_timer_init)
#define DNS_SERVERAddr (g_cur_dev->dns_server_addr)
#define DNS_Config (g_cur_dev->dns_config)

/* ===================== SPI function prototypes ===================== */
void CH394Q_Write(uint32_t addr, uint8_t data);
uint8_t CH394Q_Read(uint32_t addr);
uint16_t CH394Q_WriteBuf(uint32_t addr, uint8_t *buf, uint16_t len);
uint16_t CH394Q_ReadBuf(uint32_t addr, uint8_t *buf, uint16_t len);

#endif /* CH394Q_SPI_H_ */
