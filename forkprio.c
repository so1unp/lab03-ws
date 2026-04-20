
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/times.h> // times()
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t unprocessedSig = 0;

int busywork(void) {

  struct sigaction act;
  struct tms buf;
  for (;;) {

    if (unprocessedSig) {
      unprocessedSig = 0;
      struct rusage usage;
      getrusage(RUSAGE_SELF, &usage);
      printf("soy el hijo %d  tiempo activo %ld \n", getpid(),
             usage.ru_stime.tv_sec);

      exit(EXIT_SUCCESS);
    }
  }
}
void handler(int signo) {
    unprocessedSig = 1;
}

int main(int argc, char *argv[]) {

    signal(SIGTERM,handler);

  int copias = atoi(argv[1]);
  struct tms buf;
  pid_t hijo;
  clock_t inicio = clock();

  int tiempoMuerto = atoi(argv[2]);

  for (int i = 0; i < copias; i++) {
    hijo = fork();

    if (hijo == 0) {
      busywork();
    }
  }

  sleep(tiempoMuerto);
  kill(hijo, SIGTERM);


  // aca asesino a todos los hijos, despues del tiempo
  // prioridad: basicamente hacer que los procesos tengan
  // prioridad en el tiempo de cpu

  // poner un handle en p2, cuando llega la senal print del estatus

  exit(EXIT_SUCCESS);
}
