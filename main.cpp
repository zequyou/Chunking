#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <unistd.h>
#include "golden.h"
#include "chunking.h"

using namespace std;

#define DATA_LENGTH (69079040)
#define BLOCK_MAX (DATA_LENGTH / 3 / 1024)
#define CHUNK_FIND_MAX (10)

uint8_t data[DATA_LENGTH];
block golden[BLOCK_MAX];
uint8_t *chunk_buffer[BLOCK_MAX];
uint32_t chunk_length[BLOCK_MAX];

int main(int argc, char *argv[]) {
    for (auto &block : golden) {
        block.offset = 0;
        block.length = 0;
    }

    for (auto &buffer : chunk_buffer) {
        buffer = new uint8_t[CHUNK_SIZE_MAX];
    }

    // read all data
    ifstream input_stream;
    input_stream.open(argv[1], ios::binary);
    if (input_stream.bad()) {
        exit(1);
    }
    input_stream.read((char *)data, DATA_LENGTH);
    input_stream.close();

    // create pipe
    int pipe_file[2];
    if (pipe2(pipe_file, 0) == -1) {
        cout << "create pipe fail" << endl;
        exit(1);
    }

    // create child process
    pid_t pid = fork();
    if (pid == -1) {
        cout << "fork fail" << endl;
    }
    if (pid == 0) {
        // child here
        uint32_t byte_bias = 0;
        uint32_t total_byte = DATA_LENGTH;
        while (true) {
            uint32_t byte_need_write = total_byte > 4096 ? 4096 : total_byte;
            uint32_t byte_write = write(pipe_file[1], data + byte_bias, byte_need_write);
            if (byte_write == -1) {
                cout << "write fail!" << endl;
                break;
            }
            total_byte = total_byte - byte_write;
            byte_bias += byte_write;
            if (total_byte == 0) {
                break;
            }
        }
        close(pipe_file[0]);
        close(pipe_file[1]);
    } else {
        // parent here
        chunking my_chunking(pipe_file[0], DATA_LENGTH);
        uint32_t chunk_count = 0;

        clock_t start = clock();
        while (true) {
            uint32_t chunk_find = my_chunking.get_chunks(
                    chunk_buffer + chunk_count,
                    chunk_length + chunk_count,
                    CHUNK_FIND_MAX);
            chunk_count += chunk_find;
            if (chunk_find != 10) {
                break;
            }
        }
        cout << "function1 used " << clock() - start << " time" << endl;

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
        uint64_t block_length_total = 0;
        for (int i = 0; i < BLOCK_MAX; i++) {
            if (golden[i].length == 0) {
                block_count = i + 1;
                break;
            }
            block_length_total += golden[i].length;
        }

        printf("block average length %lu\n", block_length_total / block_count);
        printf("block count: %lu\n", block_count);
    }

    return 0;
}
