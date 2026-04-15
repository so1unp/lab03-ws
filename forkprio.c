#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>      // times()

int busywork(void)
{
    struct tms buf;
    for (;;) {
        times(&buf);
    }
}

int main(int argc, char *argv[])
{
    exit(EXIT_SUCCESS);
}
