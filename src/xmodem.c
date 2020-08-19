#include "xmodem.h"

#define HEADER_LEN (3)
#define PAYLOAD_LEN (128)
#define FOOTER_LEN (1)
#define BLOCK_LEN (HEADER_LEN + PAYLOAD_LEN + FOOTER_LEN)

#define SOH (0x01)
#define EOT (0x04)
#define ACK (0x05)
#define NAK (0x15)
#define CAN (0x18)

static int checksum(char const *buf)
{
    unsigned char sum = 0;

    for (int i = 0; i < BLOCK_LEN - 1; ++i)
    {
        sum += buf[i];
    }

    return sum == buf[BLOCK_LEN - 1];
}

void xmodem_receive(struct XModemReceiver *p)
{
    char buf[BLOCK_LEN];

    // start
    for (;;)
    {
        p->put(p->context, NAK);

        if (p->ready(p->context) > 0)
        {
            break;
        }
        else
        {
            p->sleep(p->context, 3000);
        }
    }

    // run
    for (;;)
    {
        buf[0] = p->get(p->context);

        switch (buf[0])
        {
        case SOH:
            break;

        case EOT:
            return;

        default:
            goto reject;
        }

        for (int i = 1; i < BLOCK_LEN; ++i)
        {
            buf[i] = p->get(p->context);
        }

        if (buf[0] == SOH && buf[1] == ~buf[2] && checksum(buf))
        {
            p->write(p->context, &buf[HEADER_LEN], PAYLOAD_LEN);
            p->put(p->context, ACK);
        }
        else
        {
        reject:
            while (p->ready(p->context) > 0)
            {
                p->get(p->context);
            }
            p->put(p->context, NAK);
        }
    }
}
