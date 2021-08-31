#include "DataLinkLayerInterface.h"
#include "Application.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void show_chunk(char * chunk, int chunk_len)
{
    for (int i = 0; i < chunk_len; i++)
        printf("%c", chunk[i]);
}

void process_file(char *file_path)
{
    FILE *fp;

    printf("%s\n", file_path);
    fp = fopen(file_path, "r");

    if (fp == NULL) {
        //Tratar erro de forma eficiente
        printf("%s\n", strerror(errno));
        exit(0);
    }

    int file_size = 0;

    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);

    rewind(fp);

    char chunk[CQ_DATA_MAX_LEN];

    int bytes_read;
    printf("Sending file: %s\n", file_path);
    while (bytes_read = fread(chunk + CQ_HEADER_LEN, sizeof(char), CQ_MESSAGE_LEN, fp)) {
        *((int *)chunk) = bytes_read;

#ifdef DEBUG
        show_chunk(chunk, CQ_DATA_MAX_LEN);
#endif

        send_data_to_dll(chunk, CQ_DATA_MAX_LEN);
    }
    memset(chunk, 0x0, CQ_DATA_MAX_LEN);
    send_data_to_dll(chunk, CQ_DATA_MAX_LEN);

    fclose(fp);
}

void mount_file(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "w");

    char chunk_data[CQ_DATA_MAX_LEN];
    int chunk_len;

    while(1) {
        get_data_from_dll(chunk_data, &chunk_len);

        int useful_msg_len = *((int *)chunk_data);
        if (useful_msg_len == 0)
            break;

#ifdef DEBUG
        show_chunk(chunk_data, CQ_DATA_MAX_LEN);
#endif

        for (int i = CQ_HEADER_LEN; i < CQ_HEADER_LEN + useful_msg_len; i++) {
            fputc(chunk_data[i], fp);
        }
    }

    fclose(fp);
}