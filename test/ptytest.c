#include "xmodem.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Context
{
    int pty;
};

int test_get(void *context)
{
    return EOF;
}

void test_put(void *context, char c)
{
}

int test_write(void *context, char *buf, int len)
{
    return 0;
}

void test_sleep(void *context, int ms)
{
    usleep(1000 * ms);
}

int main(int argc, char **argv)
{
    struct Context context = {
        .pty = open("/dev/ptmx", O_RDWR | O_NOCTTY),
    };
    struct XModemReceiver receiver = {
        .get = test_get,
        .put = test_put,
        .write = test_write,
        .sleep = test_sleep,
        .context = &context,
    };

    if (context.pty < 0)
    {
        perror("could not create pseudo terminal");
        exit(EXIT_FAILURE);
    }

    grantpt(context.pty);
    unlockpt(context.pty);

    puts(ptsname(context.pty));

    xmodem_receive(&receiver);

    close(context.pty);

    exit(EXIT_SUCCESS);
}
