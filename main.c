#include "reader.h"
#include "writer.h"
#include "scanner.h"
#include <stdio.h> // For printf
#include <stdlib.h> // For atoi

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Uso: %s <PID> <String>\n", argv[0]);
    return 1;
  }

  scan_memory(atoi(argv[1]), argv[2]);
  return 0;
}
