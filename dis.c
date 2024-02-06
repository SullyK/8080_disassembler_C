#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNK_SIZE 1024 // 16KiB = 16 * 1024 (reduced to 1KiB for testing)

char *readUserInput(void) {
  printf("Enter your filename, (it has to be in this directory)\n");
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelength;
  linelength = getline(&line, &linecap, stdin);
  if (linelength == -1) {
    printf("Failed to read line");
    free(line);
    return NULL;
  } else if (linelength > 0 && line[linelength - 1] == '\n') {
    line[linelength - 1] = '\0';
    return &line[0];
  }
  free(line);
  return NULL;
}

void processFileChunks(FILE *fp, char *buffer) {
  size_t bytesread;
  while ((bytesread = fread(&buffer[0], 1, CHUNK_SIZE, fp)) > 0) {
    printf("%zu\n", bytesread);
    if (bytesread < (size_t)CHUNK_SIZE) {
      printf("final chunk\n");
      // final chunk, need to figure out a way to deal with this
      break;
    } else {
      printf("a chunk\n");
      char extrabyte;
      int extrabytereturn;
      extrabytereturn = fread(&extrabyte, 1, 1, fp);
      if (extrabytereturn == 0) {
        // final chunk
        printf("final chunk");
        break;
      } else {
        ungetc((int)extrabyte, fp);
      }
    }
  }
  if (ferror(fp)) {
    printf("error reading the file\n");
  }
}

int main(void) {

  char *filename = readUserInput();
  if (filename == NULL) {
    free(filename);
    return -1;
  }
  FILE *fp = fopen(filename, "rb");

  if (fp == NULL) {
    printf("Could not open file, terminating program\n");
    free(filename);
    return -1;
  }

  char buffer[CHUNK_SIZE];
  processFileChunks(fp, &buffer[0]);
  free(filename);

  return 0;
}
