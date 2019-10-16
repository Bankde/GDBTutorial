#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** split(char* input) {
  char** result = malloc(10*sizeof(char*));
  result[0] = input;
  int j = 1;
  for (int i=0; i<strlen(input); i++) {
    if (input[i] == ' ') {
      input[i] = '\x00';
      result[j] = input+i+1;
      j++;
    }
  }
  return result;
}

int getInputAndRun() {
  char input[100];
  printf("Put string to split: ");
  fgets(input, 100, stdin);
  if (strcmp(input, "exit") == 0) return -1;
  else {
    char** result = split(input);
    *result[0] = toupper(result[0][0]);
    *result[1] = toupper(result[1][0]);
    printf("First str: %s\n", result[0]);
    printf("Second str: %s\n", result[1]);
    return 0;
  }
}

int main() {
  while (1) {
    if (getInputAndRun() == -1) break;
  }
  return 0;
}
