#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>      // times()
#include <time.h>
#include <unistd.h>

int busywork(int copias)
{
    struct tms buf;
    clock_t inicio = clock();
    for (int i=0;i<copias;i++) 
    {
        pid_t hijo=fork();
        if(hijo==0){

            printf("soy el hijo %d \n",getpid());
        
        }
        else{
            sleep(20);
            printf("soy el padre y cree el hijo %d \n", hijo);
            
        }
        
        times(&buf);
    }
}

int main(int argc, char *argv[])
{
    busywork(argc);

    exit(EXIT_SUCCESS);
}
