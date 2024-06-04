#ifndef DIS_H
#define DIS_H

#define _GNU_SOURCE 1
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNK_SIZE 1024// 16KiB = 16 * 1024 (reduced to 1KiB for testing)

typedef struct LookaheadInfo {
    unsigned char buffer[3];
    uint8_t bytes_needed;
    unsigned int size;
} LookaheadInfo;

char *read_user_input(void);
void process_opcode(unsigned char *buffer);
uint8_t instructionSize(uint8_t opcode);
bool out_of_bounds(int i, int chunk_size);
void process_lookahead_buffer(unsigned char *chunk, int chunk_size, LookaheadInfo *lookahead_info, int *i);
void fill_lookahead_buffer(unsigned char *chunk, int chunk_size, LookaheadInfo *lookahead_info, int i);
bool is_incomplete_instruction(unsigned char *chunk, int chunk_size, int i);
void process_instructions_in_chunk(unsigned char *chunk, int chunk_size, LookaheadInfo *lookahead_info);
void disassemble_file(FILE *fp, unsigned char *buffer);
int main(void);
#endif
