#include "reader.h"
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <stdio.h>
#include <stdlib.h>

int read(int argc, char *argv[]) {

  if (argc < 3) {
    printf("Uso: %s <PID <Endereço hexadecimal>\n", argv[0]);
    return 1;
  }

  int pid = atoi(argv[1]);

  mach_vm_address_t address = strtoull(argv[2], NULL, 16);
  mach_port_t task;

  // Getting OS task for the PID
  kern_return_t kret = task_for_pid(mach_task_self(), pid, &task);
  if (kret != KERN_SUCCESS) {
    fprintf(stderr,
            "Erro: não foi possivel obter a task para o PID %d; (Use sudo?)\n",
            pid);
    return 1;
  }

  pointer_t buffer;
  mach_msg_type_number_t size;

  // Reading memoroy
  kret = mach_vm_read(task, address, 9, &buffer, &size);

  if (kret == KERN_SUCCESS) {
    printf("Sucesso! Conteúdo lido no endereço %p: %s\n", (void *)address,
           (char *)buffer);
  } else {
    printf("Falha ao ler a memória. Erro: %d directory\n", kret);
    ;
  }
  return 0;
}
