/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_DNS.c
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2025/07/01
 * Description        : CH394Q DNS program body
 *******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <CH394Q_DNS.h>
#include <CH394Q_SPI.h>   /* g_cur_dev + per-device macros */
#include <CH394Q.h>
#include <CH394Q_NET.h>

/* All DNS state variables (DNS_Xid, DNS_Retries, DNS_Timer, DNS_Buffer,
 * DNS_State, DNS_Socket_Initialized, Timer_Initialized,
 * DNS_SERVERAddr, DNS_Config) are now macros that redirect to
 * g_cur_dev->xxx. See CH394Q_SPI.h. */
const char *domain = "wch.cn";
// DNS flag
#define DNS_FLAG_QUERY 0x0000 // Standard query
#define DNS_FLAG_RD 0x0100    // Expect recursion

/********************************************************************************
 * Function Name  : CH394Q_Init_Timer
 * Description    : Initialize timer
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
//static void CH394Q_Init_Timer(void)
//{
//    if (Timer_Initialized)
//        return;

//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;                           // 1ms Cycle (1MHz Count Frequency)
//    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1; // 1us count
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//    TIM_Cmd(TIM2, ENABLE);

//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

//    Timer_Initialized = 1;
//}

/********************************************************************************
 * Function Name  : CH394Q_Bswap16
 * Description    : Endian conversion
 * Input          : x: Data
 * Output         : None
 * Return         : Data
 *******************************************************************************/
static uint16_t CH394Q_Bswap16(uint16_t x)
{
    return ((x & 0xFF00) >> 8) | ((x & 0x00FF) << 8);
}

/********************************************************************************
 * Function Name  : CH394Q_Init_DNS_Socket
 * Description    : Initialize socket
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void CH394Q_Init_DNS_Socket(void)
{
    if (DNS_Socket_Initialized)
        return;

    if (CH394Q_GetSn_STA(4) != SOCK_UDP)
    {
//        printf("Initialize DNS UDP socket\n");
        CH394Q_Socket_Init(4, Sn_MODE_UDP, DNS_CLIENT_PORT);
    }
    DNS_Socket_Initialized = 1;
}

/********************************************************************************
 * Function Name  : CH394Q_Build_DNS_Query
 * Description    : Build DNS query
 * Input          : buf: Target buffer, used to store DNS messages
                    domain: Domain name
 * Output         : None
 * Return         : Total message length
 *******************************************************************************/
static uint16_t CH394Q_Build_DNS_Query(uint8_t *buf, const char *domain)
{
    dns_header_t *header = (dns_header_t *)buf;
    uint8_t *p = buf + sizeof(dns_header_t);
    uint16_t len;

    // Fill header
    header->id = DNS_Xid;
    header->flags = DNS_FLAG_QUERY | DNS_FLAG_RD;
    header->qdcount = 1;
    header->ancount = 0;
    header->nscount = 0;
    header->arcount = 0;

    // Convert byte order (big endian)
    header->id = CH394Q_Bswap16(header->id);
    header->flags = CH394Q_Bswap16(header->flags);
    header->qdcount = CH394Q_Bswap16(header->qdcount);
    header->ancount = CH394Q_Bswap16(header->ancount);
    header->nscount = CH394Q_Bswap16(header->nscount);
    header->arcount = CH394Q_Bswap16(header->arcount);

    const char *s = domain;
    while (*s)
    {
        const char *dot = strchr(s, '.');
        if (!dot)
            dot = s + strlen(s);
        len = dot - s;
        if (len > 63)
            len = 63;
        *p++ = len;
        memcpy(p, s, len);
        p += len;
        s = *dot ? dot + 1 : dot;
    }
    *p++ = 0;
    *p++ = 0;
    *p++ = DNS_TYPE_A; // A Record
    *p++ = 0;
    *p++ = DNS_CLASS_IN; // Internet Class

    return p - buf; // Return query length
}

/********************************************************************************
 * Function Name  : CH394Q_Parse_DNS_Response
 * Description    : Parse DNS responses
 * Input          : len: Received DNS response length
                    ip: Parsed IP address
 * Output         : None
 * Return         : DNS status
 *******************************************************************************/
static uint8_t CH394Q_Parse_DNS_Response(uint16_t len, uint8_t *ip)
{
    dns_header_t *header = (dns_header_t *)DNS_Buffer;
    uint8_t *p = DNS_Buffer + sizeof(dns_header_t);
    uint8_t *end = DNS_Buffer + len;
    uint16_t qdcount, ancount;

    header->id = CH394Q_Bswap16(header->id);
    header->flags = CH394Q_Bswap16(header->flags);
    qdcount = CH394Q_Bswap16(header->qdcount);
    ancount = CH394Q_Bswap16(header->ancount);

    // Validate response
    if (header->id != DNS_Xid)
    {
//        printf("Invalid DNS response ID\n");
        return DNS_STATUS_INVALID;
    }
    if (!(header->flags & 0x8000))
    {
//        printf("Non-DNS response\n");
        return DNS_STATUS_INVALID;
    }
    if (header->flags & 0x000F)
    {
//        printf("DNS response error code: %d\n", header->flags & 0x000F);
        return DNS_STATUS_FAILED;
    }

    // Skip the question section
    while (qdcount-- && p < end)
    {
        while (*p && p < end)
            p++;
        p++;
        p += 4;
    }

    // Parse the answer section
    while (ancount-- && p < end)
    {
        if ((*p & 0xC0) == 0xC0)
        {
            p += 2;
        }
        else
        {
            while (*p && p < end)
                p++;
            p++;
        }
        uint16_t type = (p[0] << 8) | p[1];
        p += 2; // Type
        p += 2; // Class
        p += 4; // TTL
        uint16_t rdlength = (p[0] << 8) | p[1];
        p += 2; // Data length
        if (type == DNS_TYPE_A && rdlength == 4)
        {
            memcpy(ip, p, 4);
//            printf("Parse IP: %d.%d.%d.%d\n", p[0], p[1], p[2], p[3]);
            return DNS_STATUS_SUCCESS;
        }
        p += rdlength;
    }

//    printf("A record not found\n");
    return DNS_STATUS_FAILED;
}

/********************************************************************************
 * Function Name  : CH394Q_DNS_init
 * Description    : DNS initialization
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_DNS_init(void)
{
    uint8_t i = 0;
    for (i = 0; domain[i] && i < sizeof(DNS_Config.domain) - 1; i++)
    {
        DNS_Config.domain[i] = domain[i];
    }
    DNS_Config.domain[sizeof(DNS_Config.domain) - 1] = '\0'; // Ensure string termination
}

/********************************************************************************
 * Function Name  : CH394Q_DNS_Run
 * Description    : DNS main running function
 * Input          : config: DNS_Config_t type pointer
 * Output         : None
 * Return         : DNS status
 *******************************************************************************/
//uint8_t CH394Q_DNS_Run(DNS_Config_t *config)
//{
//    uint8_t status = DNS_STATUS_IDLE;
//    uint8_t svr_addr[6];
//    uint16_t svr_port;
//    int32_t ret = 0;
//    uint8_t flag = 0;
//    uint16_t len;
//    static uint32_t Last_Print_Time = 0; // Last print time

//    // Initialize timer and socket
//    if (!Timer_Initialized)
//    {
//        CH394Q_Init_Timer();
//    }
//    if (!DNS_Socket_Initialized)
//    {
//        CH394Q_Init_DNS_Socket();
//        CH394Q_DNS_init();
//    }

//    // Print status every 1 second (non-SUCCESS)
//    if (DNS_State != DNS_STATUS_SUCCESS && DNS_Timer - Last_Print_Time >= 1000)
//    {
//        printf("DNS state: %d, Timer: %lu, Retry: %d\n", DNS_State, DNS_Timer / 1000 % 256, DNS_Retries);
//        Last_Print_Time = DNS_Timer;
//    }

//    // State machine
//    switch (DNS_State)
//    {
//    case DNS_STATUS_IDLE:
//        // Verify domain name and DNS server address
//        if (!config->domain[0])
//        {
//            printf("Domain name is empty\n");
//            return DNS_STATUS_FAILED;
//        }
//        if (DNS_SERVERAddr[0] == 0 && DNS_SERVERAddr[1] == 0 && DNS_SERVERAddr[2] == 0 && DNS_SERVERAddr[3] == 0)
//        {
//            printf("Invalid DNS server address\n");
//            return DNS_STATUS_FAILED;
//        }

//        // Send query
//        DNS_Xid++;
//        len = CH394Q_Build_DNS_Query(DNS_Buffer, config->domain);
//        printf("Send DNS query: %s to %d.%d.%d.%d\n", config->domain,
//               DNS_SERVERAddr[0], DNS_SERVERAddr[1], DNS_SERVERAddr[2], DNS_SERVERAddr[3]);
//        ret = CH394Q_SocketSendTo(4, DNS_Buffer, len, DNS_SERVERAddr, DNS_SERVER_PORT);
//        printf("Send result: %ld\n", ret);
//        if (ret < 0)
//        {
//            return DNS_STATUS_FAILED;
//        }

//        DNS_Timer = 0;
//        DNS_Retries = 0;
//        DNS_State = DNS_STATUS_RUNNING;
//        status = DNS_STATUS_RUNNING;
//        break;

//    case DNS_STATUS_RUNNING:
//        if ((len = CH394Q_GetSn_RX_RS(4)) > 0)
//        {
//            len = CH394Q_Socket_UDP_Recv(4, DNS_Buffer, len, svr_addr, &svr_port, &flag);
//            printf("Receive %d Bytes, Port %d\n", len, svr_port);
//            if (svr_port == DNS_SERVER_PORT && len > 0)
//            {
//                uint8_t parse_status = CH394Q_Parse_DNS_Response(len, config->ip);
//                if (parse_status == DNS_STATUS_SUCCESS)
//                {
//                    DNS_State = DNS_STATUS_SUCCESS;
//                    status = DNS_STATUS_SUCCESS;
//                }
//                else
//                {
//                    DNS_State = DNS_STATUS_IDLE;
//                    status = parse_status;
//                }
//            }
//        }
//        // Check timeout
//        if (DNS_Timer >= DNS_TIMEOUT_MS)
//        {
//            if (++DNS_Retries >= DNS_MAX_RETRIES)
//            {
//                printf("DNS query timeout, reached maximum retry count\n");
//                DNS_State = DNS_STATUS_IDLE;
//                status = DNS_STATUS_TIMEOUT;
//            }
//            else
//            {
//                // Resend query
//                len = CH394Q_Build_DNS_Query(DNS_Buffer, config->domain);
//                printf("Retry DNS query (%d/%d)\n", DNS_Retries, DNS_MAX_RETRIES);
//                ret = CH394Q_SocketSendTo(4, DNS_Buffer, len, DNS_SERVERAddr, DNS_SERVER_PORT);
//                printf("Send result: %ld\n", ret);
//                DNS_Timer = 0;
//                status = DNS_STATUS_RUNNING;
//            }
//        }
//        break;

//    case DNS_STATUS_SUCCESS:
//        // Parsing successful, maintain state
//        status = DNS_STATUS_SUCCESS;
//        break;

//    default:
//        // Error state, reset
//        DNS_State = DNS_STATUS_IDLE;
//        status = DNS_STATUS_FAILED;
//        break;
//    }

//    return status;
//}

/********************************************************************************
 * Function Name  : CH394Q_DNS_Tick
 * Description    : 1ms call once
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_DNS_Tick(void)
{
    if (DNS_State == DNS_STATUS_RUNNING)
    {
        DNS_Timer++;
    }
}
