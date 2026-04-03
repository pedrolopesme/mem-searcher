#include <stdio.h>
#include <unistd.h>

int main() {
  char secret_code[] = "SENHA123";

  while (1) {
    printf("PID: %d | A senha esta no endereço %p\n", getpid(),
           (void *)secret_code);
    printf("Conteúdo atual: %s\n", secret_code);
    sleep(5);
  }
  return 0;
}
