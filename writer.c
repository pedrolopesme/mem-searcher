#include "writer.h"
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <stdio.h>
#include <stdlib.h>

int write(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Uso: %s <PID> <Endereço Hex> <Nova Senha> \n", argv[0]);
    return 1;
  }

  int pid = atoi(argv[1]);
  mach_vm_address_t address = strtoull(argv[2], NULL, 16);
  char *new_data = argv[3];
  mach_msg_type_number_t data_cnt =
      (mach_msg_type_number_t)strlen(new_data) + 1;

  mach_port_t task;

  // Getting authorization to change process
  if (task_for_pid(mach_task_self(), pid, &task) != KERN_SUCCESS) {
    perror("Erro ao obter o task_for_pid (tente como sudo)");
    return 1;
  };

  // Attack: rewrite memory address
  kern_return_t kret =
      mach_vm_write(task, address, (vm_offset_t)new_data, data_cnt);

  if (kret == KERN_SUCCESS) {
    printf("Injeção feita com sucesso");
  } else {
    printf("Falha ao escrever. Codigo de erro %d\n", kret);
  }

  return 0;
}
