#include "xmodem.h"

#define BUF_LEN (133)

static int const EOF = -1;

static char const SOH = 0x01;
static char const EOT = 0x04;
static char const ACK = 0x06;
static char const NAK = 0x15;

struct Receiver
{
    struct XModemReceiver *p;
    size_t len;
    char buf[BUF_LEN];
};

void xmodem_receive(struct XModemReceiver *p)
{
    struct Receiver self = {
        .p = p,
        .len = 0,
        .buf = {0},
    };

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
        self.buf[self.len] = p->get(p->context);

        if (self.len == 0)
        {
            switch (self.buf[0])
            {
            }
        }
    }
}
