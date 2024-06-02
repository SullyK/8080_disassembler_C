#ifndef DIS_H
#define DIS_H

#define _GNU_SOURCE 1
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNK_SIZE 1024 // 16KiB = 16 * 1024 (reduced to 1KiB for testing)

//char *read_user_input(void);
//void process_opcode(unsigned char *buffer);
//uint8_t instructionSize(uint8_t opcode);
//void disAndWrite(unsigned char *chunk, size_t chunk_size,
//                 unsigned char *lookahead_buffer, uint8_t *lookahead_req_bytes);
//void processFileChunks(FILE * fp, unsigned char *buffer);

#endif
