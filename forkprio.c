

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/times.h> // times()
#include <unistd.h>

int busywork(void) {

  for (;;) {
  }
}
void handler_padre() { kill(0, SIGINT); }

void handler() {
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  printf("soy el hijo %d prioridad %d tiempo activo %ld \n", getpid(),
         getpriority(PRIO_PROCESS, (id_t)getpid()),
         usage.ru_utime.tv_sec + usage.ru_stime.tv_sec);
  // siendo usage.ru_utime el tiempo que mi cpu ejecuto el loop infinito
  // y usage.ru_stime el tiempo que ocupo el sistema operativo en modo kernel
  // la suma de ambos hace el tiempo total que ejecuto el proceso
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    perror("uso: %s <copias> <tiempoMuerto> <prioridades>\n");
    exit(EXIT_FAILURE);
  }

  int copias = atoi(argv[1]);
  int tiempoMuerto = atoi(argv[2]);
  int prioridades = atoi(argv[3]);

  if (copias < 1) {
    perror("copias no puede ser negativo\n");
    exit(1);
  }

  pid_t hijos[copias];

  signal(SIGINT, handler);
  // crear contador que dicte la prioridad

  for (int i = 0; i < copias; i++) {
    hijos[i] = fork();

    if (hijos[i] == 0) {
      if (prioridades == 1) {
        setpriority(PRIO_PROCESS, 0, i);
        // esta linea hace que cada proceso tenga menos prioridad que el
        // anterior
      }
      busywork();
    }
  }
  if (tiempoMuerto == 0) {
    signal(SIGINT, handler_padre);
    busywork();
  } else {
    sleep((unsigned int)tiempoMuerto);
  }
  for (int i = 0; i < copias; i++) {

    kill(hijos[i], SIGTERM);
  }
  // aca asesino a todos los hijos, despues del tiempo
  // prioridad: basicamente hacer que los procesos tengan
  // prioridad en el tiempo de cpu
  // como hago que con tiempo muerto 0 matar al padre?
  // poner un handle en p2, cuando llega la senal print del estatus

  exit(EXIT_SUCCESS);
  // aa
}
