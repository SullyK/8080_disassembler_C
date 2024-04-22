#define _GNU_SOURCE 1
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

// processOpcode() { //unfinished
//   switch (chunk[prg_cntr]) {
//   case 0x00:
//     printf("NOP");
//     pc += 1;
//   case 0x01:
//     printf("");
//   }
// }

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
// void dis(char *store_output, char *lookaheadbuffer, unsigned char *chunk,
//     unsigned int chunk_size) {
//
//   // on first load, check the look_ahead_buffer to make sure it's empty
//   // if it's empty move on as it means we had no incomplete instructions
//   is_buffer_empty = 1;
//
//   for (int x = 0; x < sizeof(lookaheadbuffer); x++) {
//     if (lookaheadbuffer[x] != 0) {
//       is_buffer_empty = 0;
//       break;
//     }
//   }
//
//   // deal with the buffer here... I need to figure out if I want to
//   // check each indiviual instruction within the buffer as it might or
//   // do I want to do that before this iteration? for now I will do this
//   //
//   // process byte 1 -> store 1,2,3 bytes in buffer -> process buffer -> clear
//   // buffer -> process byte 2 -> store 2,3,4 -> use 2 and 3 (2 byte
//   instruction)
//   // -> clear buffer -> process byte 4 etc
//
//   int pc = 0;
//   // when the buffer isn't empty
//   if (is_buffer_empty != 1) {
//     // need to check the first opcode
//     lookaheadbuffer[0] = processOpcode(
//   }
//   while (pc < chunk_size) {
//     unsigned char opcode = chunk[pc];
//     processopcode(store_output)//this should take the pointer of small buff
//   }
// }

void deal_with_look_ahead(char* lookahead_buffer, uint8_t size){
    

}

int disAndWrite(unsigned char *chunk,char* lookahead_buffer, size_t bytesread, uint8_t *lookahead_size) { // dont think the size is required since
                                        // we have chunk_size

// testing purposes to check the output
//for (int i = 0; i < bytesread; i++) {
//    printf("%d\n" i);
//printf("%02x\n", chunk[i]);
//}
//}

    //check lookahead buffer only on iterations that aren't the first
	//check last 2 instructions, if final - 2 required 3 or 2?
	//can use the size of the buffer with a var to see how many instructions
	//are required to processed in the next lookahead
    
   FILE *fp = fopen("dis.txt", "a");
    for (int i = 0; i < bytesread; ++i){
	if(lookahead_size != 0){
	    // do_look_ahead_stuff()
	}
	//edge-case - 2nd last + instruction size of 3
	if(i == (bytesread- 2) && instructionSize(chunk[i]) == 3){
//fill buffer with the current + last instruction, then set lookahead_siz and return
	    lookahead_buffer[0] = chunk[i];
	    lookahead_buffer[1] = chunk[i+1];
	    *lookahead_size = 2;
	    return;
	}

	//edgecase - last + instruction size of 2 or 3

	if(i == (bytesread-1)){
	    uint8_t size = instructionSize(chunk[i])
	    if(size == 2 || size == 3){
	    lookahead_buffer[0] = chunk[i];
	    lookahead_size = size;
	    }
	}

	//normal case - TODO: tomorrow
   
    }
     return 1;
  }

void processFileChunks(FILE * fp, unsigned char *buffer) {
    char lookahead_buffer[3]; // 3 bytes (don't need null term)
    uint8_t lookahead_size = 0;
    char buffer[25];         // the buffer shouldn't contain more than 20 char
                     // to write to output (this is the opcode, not the bytes)
   
    size_t bytesread;
    while ((bytesread = fread(buffer, 1, CHUNK_SIZE, fp)) >0) {            // chunk size no. into buffer
      disAndWrite(buffer,lookahead_buffer, bytesread, &lookahead_size); // process the buffer (this should be the chunk of
                           // data we read)                                    
			   // the bytes read here, will mean if the chunk_size is smaller then its
			   // fine
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
