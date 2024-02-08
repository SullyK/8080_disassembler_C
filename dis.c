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

dis(char *store_output, char *lookaheadbuffer, unsigned char *chunk, unsigned int chunk_size) {
  
    // on first load, check the look_ahead_buffer to make sure it's empty
    // if it's empty move on as it means we had no incomplete instructions
    is_buffer_empty = 1;
    
    for (int x = 0; x<sizeof(lookaheadbuffer);x++){
	if(lookaheadbuffer[x] != 0){
	    is_buffer_empty = 0;
	    break;
	}
    }

    //deal with the buffer here... I need to figure out if I want to 
    //check each indiviual instruction within the buffer as it might or
    //do I want to do that before this iteration? for now I will do this
    //
    //process byte 1 -> store 1,2,3 bytes in buffer -> process buffer -> clear buffer -> process byte 2 -> store 2,3,4 -> use 2 and 3 (2 byte instruction) -> clear buffer -> process byte 4 etc
    int pc = 0;

  while (pc < chunk_size) {
    switch (chunk[prg_cntr]) {
    case 0x00:
      printf("NOP");
      pc += 1;
    case 0x01:
      printf("");

    }
  }
}

int disAndWrite(unsigned char *chunk, int size) {

  // for (int i = 0; i < CHUNK_SIZE; i++) {
  // printf("%02x\n", chunk[i]);
  //}
  // testing purposes to check the output
  char lookaheadbuffer[3]; // 3 bytes (don't need null term)
  char buffer[25]; // the buffer shouldn't contain more than 20 char
                   // to write to output (this is the opcode, not the bytes)
  FILE *fp = fopen("dis.txt", "a");
  for (int i = 0; i < CHUNK_SIZE)
    return 1;
}

void processFileChunks(FILE *fp, unsigned char *buffer) {
  size_t bytesread;
  while ((bytesread = fread(&buffer[0], 1, CHUNK_SIZE, fp)) > 0) {
    disAndWrite(buffer);
  }

  if (ferror(fp)) {
    printf("error reading the file\n");
    exit(1);
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

  unsigned char buffer[CHUNK_SIZE];
  processFileChunks(fp, &buffer[0]);
  free(filename);

  return 0;
}
