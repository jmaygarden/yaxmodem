#pragma once

struct XModemReceiver
{
    int (*ready)(void *context);
    int (*get)(void *context);
    void (*put)(void *context, char c);
    int (*write)(void *context, char *buf, int len);
    void (*sleep)(void *context, int ms);
    void *context;
};

extern void xmodem_receive(struct XModemReceiver *);
