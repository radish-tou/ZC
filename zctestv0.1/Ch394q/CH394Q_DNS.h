/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_DNS.h
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2025/07/01
 * Description        : CH394Q DNS program header file
 *******************************************************************************/
#ifndef CH394Q_DNS_H_
#define CH394Q_DNS_H_

#include <stdint.h>
#include <CH394Q.h>

#define DNS_SERVER_PORT     53      // DNS server port
#define DNS_CLIENT_PORT     49152   // DNS Client Port (Dynamic Port)
#define DNS_TIMEOUT_MS      5000    // Query timeout (ms)
#define DNS_MAX_RETRIES     3       // Maximum retry count
#define DNS_BUFFER_SIZE     512     // DNS message buffer size

// DNS query type
#define DNS_TYPE_A          1       // A Record (IPv4 address)
#define DNS_CLASS_IN        1       // Internet Class

// DNS returns status
#define DNS_STATUS_IDLE     0       // Idle (not started or completed)
#define DNS_STATUS_RUNNING  1       // Querying
#define DNS_STATUS_SUCCESS  2       // Parse success
#define DNS_STATUS_FAILED   3       // Parse fail
#define DNS_STATUS_TIMEOUT  4       // Timeout
#define DNS_STATUS_INVALID  5       // Invalid response

// DNS header structure
typedef struct
{
    uint16_t id;          // Transaction ID
    uint16_t flags;       // Flags (Inquiries/Responses)
    uint16_t qdcount;     // Number of questions
    uint16_t ancount;     // Number of answers
    uint16_t nscount;     // Number of authoritative records
    uint16_t arcount;     // Number of additional records
} dns_header_t;

// DNS configuration structure
typedef struct
{
    char domain[64];      // Domain name to be resolved
    uint8_t ip[4];        // Resolved IP address
} DNS_Config_t;

uint8_t CH394Q_DNS_Run(DNS_Config_t *config);
void CH394Q_DNS_init(void);
void CH394Q_DNS_Tick(void);

#endif // _DNS_H_

