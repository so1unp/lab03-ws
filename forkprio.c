
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/times.h> // times()
#include <unistd.h>

volatile sig_atomic_t unprocessedSig = 0;

int busywork(void) {

  struct sigaction act;
  struct tms buf;
  for (;;) {
  }
}
void handler(int signo) {
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  printf("soy el hijo %d  tiempo activo %ld \n", getpid(),
         usage.ru_utime.tv_sec + usage.ru_stime.tv_sec);
  // siendo usage.ru_utime el tiempo que mi cpu ejecuto el loop infinito
  // y usage.ru_stime el tiempo que ocupo el sistema operativo en modo kernel
  // la suma de ambos hace el tiempo total que ejecuto el proceso
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

  int copias = atoi(argv[1]);
  int tiempoMuerto = atoi(argv[2]);
  int prioridades = atoi(argv[3]);

  if (copias < 1) {
    perror("copias no puede ser negativo\n");
    exit(1);
  }
//   if (prioridades != 1 | prioridades != 0) {
//     perror("el tercer valor solo pude ser 1 o 0\n");
//     exit(1);
//   }

  pid_t hijos[copias];

  signal(SIGTERM, handler);
  //crear contador que dicte la prioridad

  for (int i = 0; i < copias; i++) {
    hijos[i] = fork();

    if (hijos[i] == 0) {
      if (prioridades == 1) {
        setpriority(PRIO_PROCESS, hijos[i], -20);
        // esta linea hace que cada proceso tenga menos prioridad que el
        // anterior
      }
      busywork();
    }
  }
  if (tiempoMuerto != 0) {
    sleep(tiempoMuerto);
    for (int i = 0; i < copias; i++) {

      kill(hijos[i], SIGTERM);
    }
  } else {
    busywork();
  }
  // aca asesino a todos los hijos, despues del tiempo
  // prioridad: basicamente hacer que los procesos tengan
  // prioridad en el tiempo de cpu

  // poner un handle en p2, cuando llega la senal print del estatus

  exit(EXIT_SUCCESS);
}
