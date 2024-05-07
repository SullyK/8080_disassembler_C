#define _GNU_SOURCE 1
#include <stdbool.h>
#include <stdint.h>
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

void process_opcode(unsigned char *buffer) { // unfinished
  switch (buffer[0]) {
  case 0x00:
    printf("NOP\n");
    break;
  case 0x01:
    printf("LXI B, #$%02x%02x\n", buffer[2], buffer[1]); //#$ = how 8080 sees the 16bit number
    break;
  case 0x02:
    printf("STAX B\n");
    break;
  case 0x03:
    printf("INX B\n");
  case 0x04:
    printf("INR B\n");
  }
}

int instructionSize(uint8_t opcode) { // unfinished
  switch (opcode) {
  case 0x01:
  case 0x11:
  case 0x21:
  case 0x22:
  case 0x2a:
  case 0x31:
  case 0x32:
  case 0x3a:
  case 0xc2:
  case 0xc3:
  case 0xc4:
  case 0xca:
  case 0xcc:
  case 0xcd:
  case 0xd2:
  case 0xd4:
  case 0xda:
  case 0xdc:
  case 0xe2:
  case 0xe4:
  case 0xea:
  case 0xec:
  case 0xf2:
  case 0xf4:
  case 0xfa:
  case 0xfc:
    return 3;
  case 0x06:
  case 0x0e:
  case 0x16:
  case 0x1e:
  case 0x26:
  case 0x2e:
  case 0x36:
  case 0x3e:
  case 0xc6:
  case 0xce:
  case 0xd3:
  case 0xd6:
  case 0xdb:
  case 0xde:
  case 0xe6:
  case 0xee:
  case 0xf6:
  case 0xfe:
    return 2;
  default:
    return 1;
  } // todo: check these are correct
}

// testing purposes to check the output
// for (int i = 0; i < bytesread; i++) {
//    printf("%d\n" i);
// printf("%02x\n", chunk[i]);
//}
//}

void disAndWrite(unsigned char *chunk, size_t chunk_size, unsigned char *lookahead_buffer,
                uint8_t *lookahead_req_bytes, bool *ignore_third_byte) {
  //FILE *fp = fopen("dis.txt", "a"); //write to this later (for now testing)
  unsigned char opcode_arr[3];

  for (size_t i = 0; i < chunk_size; ++i) { //TODO: make sure this prefix doesn't mess with anything
    if (i == 0 && *lookahead_req_bytes != 0) {
      // base-case below - the first value could be the instruction
      if (*lookahead_req_bytes == 1) {
        opcode_arr[0] = lookahead_buffer[0];
        if (*ignore_third_byte == true) {
          opcode_arr[1] = chunk[i];
          *ignore_third_byte = false;
        } else {
          opcode_arr[1] = lookahead_buffer[1];
          opcode_arr[2] = chunk[i];
        }
      }

      // TODO: MAJOR ISSUE WHAT IF IT'S ONLY 1 BYTE IN ARRAY (LAST VALUE +
      // ONLY NEEDS 1 MORE?) NEED TO DEAL WITH THIS CASE
      else if (*lookahead_req_bytes == 2) {
        opcode_arr[0] = lookahead_buffer[0];
        opcode_arr[1] = chunk[i];
        // TODO: //need to do a boundary check here in case it's final
        // iteration but if it's final iteration i can check size of the chunk
        // is better than 3?
        opcode_arr[2] = chunk[i + 1];
        i++;
      }

      process_opcode(lookahead_buffer);
      *lookahead_req_bytes = 0;
      continue;
    }

    // lookahead-case - 2nd last + instruction size of 3
    //[4a][9d][] -- example
    // requires the last instruction from next buffer
    // this should short-circuit
    if (i == (chunk_size - 2) && instructionSize(chunk[i]) == 3) {
      lookahead_buffer[0] = chunk[i];
      lookahead_buffer[1] = chunk[i + 1];
      *lookahead_req_bytes = 2;
      return;
    }

    if (i == (chunk_size - 1)) {
      uint8_t i_size = instructionSize(chunk[i]);
      if (i_size == 2 || i_size == 3) {
        lookahead_buffer[0] = chunk[i];
        *lookahead_req_bytes = i_size - 1;
        if (i_size == 2) {
          *ignore_third_byte = true;
        }
      }
    }

    // normal case
    // TODO: change the scope of the instruction size, since we check it a
    // couple times

    uint8_t i_size = instructionSize(chunk[i]);

    // TODO: pretty sure I handle the boundaries for the end of the chunk
    // buffer, so don't need to handle below, but check
    if (i_size == 1) {
      process_opcode(&chunk[i]);
    }
    else if  (i_size == 2 && i + 1 < chunk_size) {
      opcode_arr[0] = chunk[i];
      opcode_arr[1] = chunk[i + 1];
      i++;
      process_opcode(opcode_arr);
    }
    else if (i_size == 3 && i + 2 < chunk_size) {
      opcode_arr[0] = chunk[i];
      opcode_arr[1] = chunk[i + 1];
      opcode_arr[2] = chunk[i + 2];
      i += 2;
      process_opcode(opcode_arr);
    }
  }
}

// process the buffer (this should be the chunk of
// data we read)
// the bytes read here, will mean if the chunk_size is
// smaller then its fine
void processFileChunks(FILE *fp, unsigned char *buffer) {

  unsigned char lookahead_buffer[3];
  uint8_t lookahead_req_bytes = 0;
  size_t bytesread;
  bool ignore_third_byte = false;
  // bytes read into buffer, loops for the whole file
  while ((bytesread = fread(buffer, 1, CHUNK_SIZE, fp)) > 0) {
    disAndWrite(buffer, bytesread, lookahead_buffer, &lookahead_req_bytes,
                &ignore_third_byte);
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
  processFileChunks(fp, buffer);
  free(filename);

  return 0;
}
