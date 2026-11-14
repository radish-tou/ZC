/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_DHCP.c
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2025/07/01
 * Description        : CH394Q DHCP program body
 *******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <CH394Q_DHCP.h>
#include <CH394Q_SPI.h>   /* g_cur_dev + per-device macros */
#include <CH394Q.h>

/* All DHCP state variables (DHCP_Status, DHCP_Xid, DHCP_Retries, DHCP_Timer,
 * DHCP_Config, Server_IP, DHCP_Buffer, DHCP_Msg, Initialization_Flag,
 * Last_Print_Time, CH394Q_MACAddr, CH394Q_DNSAddr) are now macros that
 * redirect to g_cur_dev->xxx. See CH394Q_SPI.h. */

/********************************************************************************
 * Function Name  : CH394Q_Add_Option_MSG
 * Description    : DHCP option field construction
 * Input          : buf: Destination buffer for storing DHCP options
                    code: Option code (message type, client ID)
                    len: Length of option data
                    data: Content of option data
 * Output         : None
 * Return         : Data length
 *******************************************************************************/
static uint16_t CH394Q_Add_Option_MSG(uint8_t *buf, uint8_t code, uint8_t len, const uint8_t *data)
{
    buf[0] = code;
    buf[1] = len;
    memcpy(&buf[2], data, len);
    return 2 + len;
}

///********************************************************************************
// * Function Name  : CH394Q_Init_DHCP_And_Timer
// * Description    : Initialize DHCP and timer
// * Input          : None
// * Output         : None
// * Return         : None
// *******************************************************************************/
//static void CH394Q_Init_DHCP_And_Timer(void)
//{
//    // DHCP Initialization
//    uint8_t zero_ip[4] = {0};
//    DHCP_Status = DHCP_STATE_INIT;
//    DHCP_Xid = 0x12345678;
//    DHCP_Retries = 0;
//    DHCP_Timer = 0;
//    Last_Print_Time = 0;
//    memset(&DHCP_Config, 0, sizeof(DHCP_Config_t));
//    memset(Server_IP, 0, 4);
//    CH394Q_ResetSW();
//    CH394Q_SetIP(zero_ip);
//    CH394Q_SetSMIP(zero_ip);
//    CH394Q_SetGWIP(zero_ip);
//    CH394Q_GetMAC(CH394Q_MACAddr);

//    // Timer Initialization (TIM2, 1ms Interrupt)
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

//    Initialization_Flag = 1; // Mark initialization complete
//}

/********************************************************************************
 * Function Name  : CH394Q_Init_DHCP_Msg
 * Description    : Initialize DHCP message
 * Input          : msg_type: Message type
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void CH394Q_Init_DHCP_Msg(uint8_t msg_type)
{
    memset(DHCP_Msg, 0, sizeof(DHCP_Msg_t));
    DHCP_Msg->op = DHCP_BOOTREQUEST;
    DHCP_Msg->htype = DHCP_HTYPE_ETHER;
    DHCP_Msg->hlen = DHCP_HLEN_ETHER;
    DHCP_Msg->xid = DHCP_Xid;
    DHCP_Msg->flags = DHCP_FLAGS_BROADCAST;
    memcpy(DHCP_Msg->chaddr, CH394Q_MACAddr, 6);

    // Set magic cookie
    uint8_t *opt = DHCP_Msg->options;
    opt[0] = (DHCP_MAGIC_COOKIE >> 24) & 0xFF;
    opt[1] = (DHCP_MAGIC_COOKIE >> 16) & 0xFF;
    opt[2] = (DHCP_MAGIC_COOKIE >> 8) & 0xFF;
    opt[3] = DHCP_MAGIC_COOKIE & 0xFF;
    opt += 4;

    // Add message type
    opt += CH394Q_Add_Option_MSG(opt, OPT_MSG_TYPE, 1, &msg_type);
    // Add client identifier
    uint8_t client_id[7] = {0x01, CH394Q_MACAddr[0], CH394Q_MACAddr[1], CH394Q_MACAddr[2],
                            CH394Q_MACAddr[3], CH394Q_MACAddr[4], CH394Q_MACAddr[5]};
    opt += CH394Q_Add_Option_MSG(opt, OPT_CLIENT_ID, 7, client_id);

    // Add hostname
    uint8_t hostname[16];
    snprintf((char *)hostname, sizeof(hostname), "CH%02X%02X", CH394Q_MACAddr[4], CH394Q_MACAddr[5]);
    opt += CH394Q_Add_Option_MSG(opt, OPT_HOSTNAME, strlen((char *)hostname), hostname);

    if (msg_type == DHCP_REQUEST)
    {
        // Add requested IP
        opt += CH394Q_Add_Option_MSG(opt, OPT_REQUESTED_IP, 4, DHCP_Config.ip);
        // Add server identifier
        opt += CH394Q_Add_Option_MSG(opt, OPT_SERVER_ID, 4, Server_IP);
    }

    // Add parameter request list
    uint8_t param_list[] = {OPT_SUBNET_MASK, OPT_ROUTER, OPT_DNS, OPT_LEASE_TIME};
    opt += CH394Q_Add_Option_MSG(opt, OPT_PARAM_REQUEST, 4, param_list);

    // End option
    *opt = OPT_END;
}

/********************************************************************************
 * Function Name  : CH394Q_Send_DHCP_Msg
 * Description    : Send DHCP message to server
 * Input          : msg_type: Message type
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void CH394Q_Send_DHCP_Msg(uint8_t msg_type)
{
    int32_t ret = 0;
    CH394Q_Init_DHCP_Msg(msg_type);
    uint8_t dest_ip[4] = {255, 255, 255, 255};
    if (msg_type == DHCP_REQUEST && DHCP_Status == DHCP_STATE_LEASED)
    {
        DHCP_Msg->flags = DHCP_FLAGS_UNICAST;
        memcpy(DHCP_Msg->ciaddr, DHCP_Config.ip, 4);
        memcpy(dest_ip, Server_IP, 4);
    }
//    printf("Send DHCP %s to %d.%d.%d.%d\n",
//           msg_type == DHCP_DISCOVER ? "DISCOVER" : msg_type == DHCP_REQUEST ? "REQUEST"
//                                                                             : "DECLINE",
//           dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
    ret = CH394Q_SocketSendTo(3, (uint8_t *)DHCP_Msg, sizeof(DHCP_Msg_t), dest_ip, DHCP_SERVER_PORT);
//    printf("Send result: %ld\n", ret);
}

/********************************************************************************
 * Function Name  : CH394Q_Parse_DHCP_Msg
 * Description    : Parse message received from DHCP server
 * Input          : None
 * Output         : None
 * Return         : Message type
 *******************************************************************************/
static uint8_t CH394Q_Parse_DHCP_Msg(void)
{
    uint8_t svr_addr[6];
    uint16_t svr_port, len;
    uint8_t msg_type = 0;
    uint8_t flag = 0;

    if ((len = CH394Q_GetSn_RX_RS(3)) > 0)
    {
        len = CH394Q_Socket_UDP_Recv(3, (uint8_t *)DHCP_Msg, len, svr_addr, &svr_port, &flag);
//        printf("Received %d bytes from port %d\n", len, svr_port);
        if (svr_port != DHCP_SERVER_PORT || len == 0)
            return 0;
        if (DHCP_Msg->xid != DHCP_Xid || memcmp(DHCP_Msg->chaddr, CH394Q_MACAddr, 6) != 0)
            return 0;
        uint8_t *p = DHCP_Msg->options + 4; // Skip magic cookie
        uint8_t *end = (uint8_t *)DHCP_Msg + len;

        while (p < end && *p != OPT_END)
        {
            uint8_t code = *p++;
            if (code == OPT_PAD)
                continue;
            uint8_t len = *p++;
            switch (code)
            {
            case OPT_MSG_TYPE:
                msg_type = *p;
//                printf("Received DHCP %s\n",
//                       msg_type == DHCP_OFFER ? "OFFER" : msg_type == DHCP_ACK ? "ACK"
//                                                      : msg_type == DHCP_NAK   ? "NAK"
//                                                                               : "UNKNOWN");
                break;
            case OPT_SUBNET_MASK:
                memcpy(DHCP_Config.mask, p, 4);
                break;
            case OPT_ROUTER:
                memcpy(DHCP_Config.gateway, p, 4);
                break;
            case OPT_DNS:
                memcpy(DHCP_Config.dns, p, 4);
                break;
            case OPT_LEASE_TIME:
                DHCP_Config.lease_time = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
//                printf("Lease time: %lu seconds\n", DHCP_Config.lease_time);
                break;
            case OPT_SERVER_ID:
                memcpy(Server_IP, p, 4);
//                printf("Server IP: %d.%d.%d.%d\n", Server_IP[0], Server_IP[1], Server_IP[2], Server_IP[3]);
                break;
            }
            p += len;
        }
        if (msg_type == DHCP_OFFER)
        {
            memcpy(DHCP_Config.ip, DHCP_Msg->yiaddr, 4);
//            printf("Offered IP: %d.%d.%d.%d\n",
//                   DHCP_Config.ip[0], DHCP_Config.ip[1], DHCP_Config.ip[2], DHCP_Config.ip[3]);
        }
    }
    return msg_type;
}

/********************************************************************************
 * Function Name  : CH394Q_DHCP_Run
 * Description    : DHCP client main run function
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
//uint8_t CH394Q_DHCP_Run(void)
//{
//    uint8_t status = DHCP_STATUS_RUNNING;
//    // Perform initialization (only the first call)
//    if (!Initialization_Flag)
//    {
//        CH394Q_Init_DHCP_And_Timer();
//    }
//    // Print status every 1 second (not entered LEASED)
//    if (DHCP_Status != DHCP_STATE_LEASED && DHCP_Timer - Last_Print_Time >= 1000)
//    {
//        printf("DHCP State: %d, Timer: %lu, Retries: %d\n", DHCP_Status, DHCP_Timer / 1000 % 256, DHCP_Retries);
//        Last_Print_Time = DHCP_Timer;
//    }
//    if (CH394Q_GetSn_STA(3) != SOCK_UDP)
//    {
//        printf("Initializing UDP socket\n");
//        CH394Q_Socket_Init(3, Sn_MODE_UDP, DHCP_CLIENT_PORT);
//    }

//    uint8_t msg_type = CH394Q_Parse_DHCP_Msg();

//    switch (DHCP_Status)
//    {
//    case DHCP_STATE_INIT:
//        DHCP_Xid++;
//        CH394Q_Send_DHCP_Msg(DHCP_DISCOVER);
//        DHCP_Timer = 0;
//        DHCP_Retries = 0;
//        DHCP_Status = DHCP_STATE_DISCOVER;
//        break;

//    case DHCP_STATE_DISCOVER:
//        if (msg_type == DHCP_OFFER)
//        {
//            CH394Q_Send_DHCP_Msg(DHCP_REQUEST);
//            DHCP_Timer = 0;
//            DHCP_Retries = 0;
//            DHCP_Status = DHCP_STATE_REQUEST;
//        }
//        else if (DHCP_Timer >= DHCP_TIMEOUT)
//        {
//            if (++DHCP_Retries >= DHCP_MAX_RETRIES)
//            {
//                printf("DISCOVER failed, max retries reached\n");
//                DHCP_Status = DHCP_STATE_INIT;
//                status = DHCP_STATUS_FAILED;
//            }
//            else
//            {
//                CH394Q_Send_DHCP_Msg(DHCP_DISCOVER);
//                DHCP_Timer = 0;
//            }
//        }
//        break;

//    case DHCP_STATE_REQUEST:
//        if (msg_type == DHCP_ACK)
//        {
//            uint8_t GINT_state = CH394Q_GetGINT();
//            CH394Q_SetIP(DHCP_Config.ip);
//            CH394Q_SetSMIP(DHCP_Config.mask);
//            CH394Q_SetGWIP(DHCP_Config.gateway);
//            CH394Q_SocketSendTo(3, (uint8_t *)"TEST_IP", 7, DHCP_Config.ip, 5000);
//            Delay_Ms(2000);
//            if (GINT_state & GINT_IP_CONFLI)
//            {
//                printf(" IP CONFLICT\r\n");
//                CH394Q_SetGINT(GINT_IP_CONFLI);
//                CH394Q_Send_DHCP_Msg(DHCP_DECLINE);
//                DHCP_Status = DHCP_STATE_INIT;
//            }
//            else
//            {
//                memcpy(CH394Q_DNSAddr, DHCP_Config.dns, 4);
//                printf("DHCP Leased: %d.%d.%d.%d\n",
//                       DHCP_Config.ip[0], DHCP_Config.ip[1], DHCP_Config.ip[2], DHCP_Config.ip[3]);
//                printf("DHCP mask:   %d.%d.%d.%d\n",
//                       DHCP_Config.mask[0], DHCP_Config.mask[1], DHCP_Config.mask[2], DHCP_Config.mask[3]);
//                printf("DHCP gateway:%d.%d.%d.%d\n",
//                       DHCP_Config.gateway[0], DHCP_Config.gateway[1], DHCP_Config.gateway[2], DHCP_Config.gateway[3]);
//                printf("DHCP dns:    %d.%d.%d.%d\n",
//                       DHCP_Config.dns[0], DHCP_Config.dns[1], DHCP_Config.dns[2], DHCP_Config.dns[3]);
//                DHCP_Status = DHCP_STATE_LEASED;
//                status = DHCP_STATUS_LEASED;
//            }
//        }
//        else if (msg_type == DHCP_NAK)
//        {
//            printf("Received NAK, restarting\n");
//            DHCP_Status = DHCP_STATE_INIT;
//        }
//        else if (DHCP_Timer >= DHCP_TIMEOUT)
//        {
//            if (++DHCP_Retries >= DHCP_MAX_RETRIES)
//            {
//                printf("REQUEST failed, max retries reached\n");
//                DHCP_Status = DHCP_STATE_INIT;
//                status = DHCP_STATUS_FAILED;
//            }
//            else
//            {
//                CH394Q_Send_DHCP_Msg(DHCP_REQUEST);
//                DHCP_Timer = 0;
//            }
//        }
//        break;

//    case DHCP_STATE_LEASED:
//        if (DHCP_Config.lease_time != DHCP_DEFAULT_LEASE &&
//            DHCP_Timer >= DHCP_Config.lease_time * 500) // Half-lease time (ms)
//        {
//            printf("Renewing lease\n");
//            DHCP_Xid++;
//            CH394Q_Send_DHCP_Msg(DHCP_REQUEST);
//            DHCP_Timer = 0;
//            DHCP_Retries = 0;
//            DHCP_Status = DHCP_STATE_REQUEST;
//        }
//        status = DHCP_STATUS_LEASED;
//        break;
//    }

//    return status;
//}

/********************************************************************************
 * Function Name  : CH394Q_DHCP_Tick
 * Description    : 1ms call once
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_DHCP_Tick(void)
{
    DHCP_Timer++;
}
