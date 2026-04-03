
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void scan_memory(int pid, const char *target) {
  mach_port_t task;

  if (task_for_pid(mach_task_self(), pid, &task) != KERN_SUCCESS) {
    perror("Erro task_for_pid");
    return;
  }

  mach_vm_address_t address = 0;
  mach_vm_size_t size;
  vm_region_basic_info_data_64_t info;
  mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
  mach_port_t object_name;
  size_t target_len = strlen(target);

  printf("Escaneando PID %d por %s...\n", pid, target);

  // loop por todas as regioes de memoria do processo
  while (mach_vm_region(task, &address, &size, VM_REGION_BASIC_INFO_64,
                        (vm_region_info_t)&info, &count,
                        &object_name) == KERN_SUCCESS) {

    // lendo regiao com permissao de leitura e que não esteja protegida
    if (info.protection && VM_PROT_READ) {

      // regioes maiores que 10MB devem ser ignoradas para não gerar um segfault
      if (size > 10 * 1024 * 1024) {
        address += size;
        continue;
      }

      unsigned char *buffer = malloc(size);
      if (buffer == NULL) {
        address += size;
        continue;
      }

      mach_vm_size_t bytes_read;
      if (mach_vm_read_overwrite(task, address, size, (mach_vm_address_t)buffer,
                                 &bytes_read) == KERN_SUCCESS) {

        // procurando a string dentro do bloco de memória lido
        for (size_t i = 0; i < (bytes_read - target_len); i++) {
          if (memcmp(&buffer[i], target, target_len) == 0) {
            printf("encontrado em: 0x%llx\n", address + i);
          }
        }
      }
      free(buffer);
    }
    address += size; // pulando para próxima região da memória
  }
}


