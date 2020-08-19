#include "xmodem.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct Context
{
    int pty;
};

int test_ready(void *p)
{
    int const fd = ((struct Context *)p)->pty;
    int bytes = 0;
    int const result = ioctl(fd, FIONREAD, &bytes);

    return result == 0 && bytes > 0;
}

int test_get(void *p)
{
    int const fd = ((struct Context *)p)->pty;
    unsigned char buf;
    ssize_t len = read(fd, &buf, sizeof buf);

    return len > 0 ? buf : -1;
}

void test_put(void *p, char c)
{
    int const fd = ((struct Context *)p)->pty;

    write(fd, &c, sizeof c);
}

int test_write(void *p, char *buf, int len)
{
    return write(fileno(stdout), buf, len);
}

void test_sleep(void *p, int ms)
{
    usleep(1000 * ms);
}

int main(int argc, char **argv)
{
    struct Context context = {
        .pty = open("/dev/ptmx", O_RDWR | O_NOCTTY),
    };
    struct XModemReceiver receiver = {
        .ready = test_ready,
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
