/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_DHCP.h
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2025/07/01
 * Description        : CH394Q DHCP program header file
 *******************************************************************************/
#ifndef CH394Q_DHCP_H_
#define CH394Q_DHCP_H_
#include <CH394Q.h>

// DHCP Status
#define DHCP_STATE_INIT 0x00     // Initialization
#define DHCP_STATE_DISCOVER 0x01 // Send DISCOVER
#define DHCP_STATE_REQUEST 0x02  // Send REQUEST
#define DHCP_STATE_LEASED 0x03   // Lease obtained

// DHCP return value
#define DHCP_STATUS_RUNNING 0x00 // Running
#define DHCP_STATUS_LEASED 0x01  // The IP is leased
#define DHCP_STATUS_FAILED 0x02  // DHCP failed

// DHCP message type
#define DHCP_BOOTREQUEST 0x01 // Client request
#define DHCP_BOOTREPLY 0x02   // Server response
#define DHCP_DISCOVER 0x01    // DISCOVER message
#define DHCP_OFFER 0x02       // OFFER message
#define DHCP_REQUEST 0x03     // REQUEST message
#define DHCP_DECLINE 0x04     // DECLINE message
#define DHCP_ACK 0x05         // ACK message
#define DHCP_NAK 0x06         // NAK message

// DHCP option code
#define OPT_PAD 0x00           // Fill
#define OPT_END 0xFF           // Options end
#define OPT_MSG_TYPE 0x35      // Message type
#define OPT_CLIENT_ID 0x3D     // Client identifier
#define OPT_HOSTNAME 0x0C      // Hostname
#define OPT_REQUESTED_IP 0x32  // Requested IP
#define OPT_SERVER_ID 0x36     // Server ID
#define OPT_PARAM_REQUEST 0x37 // Parameter request
#define OPT_SUBNET_MASK 0x01   // Subnet mask
#define OPT_ROUTER 0x03        // Gateway
#define OPT_DNS 0x06           // DNS server
#define OPT_LEASE_TIME 0x33    // Lease time

// DHCP constant
#define DHCP_HTYPE_ETHER 0x01         // Ethernet hardware type
#define DHCP_HLEN_ETHER 0x06          // Ethernet hardware address length
#define DHCP_FLAGS_BROADCAST 0x8000   // Broadcast flag
#define DHCP_FLAGS_UNICAST 0x0000     // Unicast flag
#define DHCP_MAGIC_COOKIE 0x63825363  // Magic Cookie
#define DHCP_TIMEOUT 5000             // Timeout (ms)
#define DHCP_MAX_RETRIES 3            // Maximum retry count
#define DHCP_SERVER_PORT 67           // DHCP server port
#define DHCP_CLIENT_PORT 68           // DHCP client port
#define DHCP_DEFAULT_LEASE 0xFFFFFFFF // Default lease term (infinite)

// DHCP message structure
typedef struct
{
    uint8_t op;           // Operation type (BOOTREQUEST or BOOTREPLY)
    uint8_t htype;        // Hardware type
    uint8_t hlen;         // Hardware address length
    uint8_t hops;         // Number of hops
    uint32_t xid;         // Transaction ID
    uint16_t secs;        // Seconds elapsed
    uint16_t flags;       // Flag
    uint8_t ciaddr[4];    // Client IP
    uint8_t yiaddr[4];    // Assigned IP
    uint8_t siaddr[4];    // Server IP
    uint8_t giaddr[4];    // Gateway IP
    uint8_t chaddr[16];   // Client hardware address (MAC)
    uint8_t sname[64];    // Server Name
    uint8_t file[128];    // Startup file name
    uint8_t options[312]; // Option field
} DHCP_Msg_t;

// Network configuration structure
typedef struct
{
    uint8_t ip[4];       // IP address
    uint8_t mask[4];     // Subnet mask
    uint8_t gateway[4];  // Gateway
    uint8_t dns[4];      // DNS server
    uint32_t lease_time; // Lease Time (seconds)
} DHCP_Config_t;

uint8_t CH394Q_DHCP_Run(void);
void CH394Q_DHCP_Tick(void);

#endif // _DHCP_H_
