#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(void) {

  char *filename = readUserInput();
  if (filename != NULL) {
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL) {
      printf("Could not open file, terminating program\n");
      free(filename);
      return -1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) { // start -> end

      printf("Fseek SEEK_END failed, terminating\n");
      free(filename);
      fclose(fp);
      return -1;
    }
    long longfilesize = ftell(fp); // location at the end, so tells file size
                                   // I have to check this isn't neg
                                   // then if it isn't I need to convert to
                                   // size_t so the compliler doesn't moan
    if (longfilesize == -1L) {
      printf("The size returned an error with ftell()\n");
      free(filename);
      fclose(fp);
      return -1;
    }
    size_t filesize = (size_t)longfilesize;

    if (fseek(fp, 0, SEEK_SET) != 0) {
      printf("Fseek SEEK_SET failed, terimating\n");
      free(filename);
      fclose(fp);
      return -1;
    }
    printf("%ld\n", filesize);

    char *filedata = (char *)malloc(filesize * sizeof(char));
    if (filedata == NULL) {
      printf("Failed to allocate memory for filedata, terminating");
      fclose(fp);
      free(filename);
      return -1;
    }

    const size_t retcode =
        fread(&filedata[0], sizeof(*(&filedata[0])), filesize, fp);
    if (retcode == filesize) {
      printf("file read into buffer successfully\n");
      for (size_t x = 0; x < filesize; x++) {
        printf("%c", filedata[x]);
      }
    }

    free(filename);
  }
  return 0;
}
