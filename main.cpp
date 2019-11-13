#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include "golden.h"
#include "chunking.h"

using namespace std;


#define DATA_LENGTH (200273920)
#define BLOCK_MAX (DATA_LENGTH / 3 / 1024)
#define CHUNK_FIND_MAX (10)

uint8_t data[DATA_LENGTH];
block golden[BLOCK_MAX];
uint8_t chunk_temp[8192 * CHUNK_FIND_MAX];
uint8_t *chunk_buffer[BLOCK_MAX];
uint32_t chunk_length[BLOCK_MAX];
uint32_t chunk_offset[BLOCK_MAX];

int main(int argc, char *argv[]) {
    for (auto &block : golden) {
        block.offset = 0;
        block.length = 0;
    }

    for (auto &buffer : chunk_buffer) {
        buffer = new uint8_t[CHUNK_SIZE_MAX];
    }

    ifstream input_stream;
    input_stream.open(argv[1], ios::binary);
    if (input_stream.bad()) {
        exit(1);
    }

    // my chunking
    chunking my_chunking(&input_stream);
    uint32_t chunk_count = 0;

    clock_t start = clock();
    while (true) {
        uint32_t chunk_find = my_chunking.get_chunks(
                chunk_temp,
                chunk_offset + chunk_count,
                chunk_length + chunk_count,
                CHUNK_FIND_MAX);
        for (uint32_t i = 0; i < chunk_find; i++) {
            memcpy(chunk_buffer[chunk_count + i],
                    chunk_temp + chunk_offset[chunk_count + i],
                    chunk_length[chunk_count + i]);
        }
        chunk_count += chunk_find;
        if (chunk_find != 10) {
            break;
        }
    }
    cout << "function1 used " << clock() - start << " time" << endl;
    input_stream.close();

    input_stream.open(argv[1], ios::binary);
    input_stream.read((char *)data, DATA_LENGTH);
    input_stream.close();
    // golden
    start = clock();
    uint32_t golden_chunk_count = chunking_golden(data, DATA_LENGTH, golden);
    cout << "function2 used " << clock() - start << " time" << endl;

    uint32_t bias = 0;
    if (chunk_count != golden_chunk_count) {
        cerr << "chunk count mismatch" << endl;
        cerr << "my chunk count mismatch " << chunk_count << endl;
        cerr << "golden count mismatch " << golden_chunk_count << endl;
        // exit(0);
    }
    for (uint32_t i = 0; i < chunk_count; i++) {
        if (golden[i].length != chunk_length[i]) {
            cerr << "chunk length at chunk " << i << " mismatch!" << endl;
            cerr << "my length " << chunk_length[i] << endl;
            cerr << "golden length " << golden[i].length << endl;
            exit(0);
        }
        for (uint32_t j = 0; j < golden[i].length; j++) {
            if (chunk_buffer[i][j] != data[bias + j]) {
                cerr << "chunk " << i << " data mismatch!" << endl;
                cerr << "at index " << j << endl;
                cerr << "golden value " << (uint32_t) data[bias + j] << endl;
                cerr << "my data value " << (uint32_t) chunk_buffer[i][j] << endl;
                exit(0);
            }
        }
        bias += chunk_length[i];
    }

    uint64_t block_count = 0;
    uint64_t chunk_6k_count = 0;
    uint64_t block_length_total = 0;
    for (int i = 0; i < BLOCK_MAX; i++) {
        if (golden[i].length == 0) {
            block_count = i + 1;
            break;
        }
        if (golden[i].length >= 6 * 1024) {
            chunk_6k_count++;
        }
        block_length_total += golden[i].length;
    }

    printf("block average length %lu\n", block_length_total / block_count);
    printf("block count: %lu\n", block_count);
    printf("block length greater than 6k: %lu\n", chunk_6k_count);
    printf("percent %lf\n", (double)chunk_6k_count / block_count);

    return 0;
}
