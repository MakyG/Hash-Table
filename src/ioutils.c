#include "ioutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void io_utils_get_string(char* line, int len) {
  if (fgets(line, len, stdin) == NULL) {
    printf("End of file, finishing.\n");
    exit(EXIT_SUCCESS);
  }
  int l = strlen(line);
  if (line[l - 1] == '\n') line[l - 1] = '\0';
#ifdef ECHO
  printf("%s", line);
#endif
}

char io_utils_get_char_line() {
  char line[255];
  io_utils_get_string(line, 255);
  return line[0];
}

int io_utils_get_long(long* val) {
  long hodnota;
  int ok;
  char line[255];
  do {
    io_utils_get_string(line, 255);
    ok = sscanf(line, "%ld", &hodnota);
    if (ok != 1) printf("Incorrect format, try again.\n");
  } while (ok != 1);
  *val = hodnota;
  return 1;
}

int io_utils_get_double(double* val) {
  double hodnota;
  int ok;
  char line[255];
  do {
    io_utils_get_string(line, 255);
    ok = sscanf(line, "%lf", &hodnota);
    if (ok != 1) printf("Incorrect format, try again.\n");
  } while (ok != 1);
  *val = hodnota;
  return 1;
}

void io_utils_clear_stdin() {
  while (fgetc(stdin) != '\n')
    ;
}
