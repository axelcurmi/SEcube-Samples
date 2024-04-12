#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "secube/L0.h"

int main(int argc, char **argv)
{
    se3_disco_it it;
    se3_device device;
    FILE *file;

    if (argc != 3)
    {
        printf("usage: random2file FILENAME SIZE\n");
        return 1;
    }

    uint16_t error_code = SE3_OK;
    uint16_t bytes_to_generate = atoi(argv[2]);
    uint16_t buffer_length = bytes_to_generate;
    uint8_t *buffer = malloc(bytes_to_generate);
    char *filename = argv[1];
    
    memset(buffer, 0, bytes_to_generate);

    L0_discover_init(&it);
    if (!L0_discover_next(&it))
    {
        fprintf(stderr, "ERROR SECube not found.\n");
        free(buffer);
        return 1;
    }

    error_code = L0_open(&device, &it.device_info, 0);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L0_open [%hd].\n", error_code);
        free(buffer);
        return 1;
    }

    error_code = L0_rand(&device, bytes_to_generate, &buffer_length, buffer);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L0_rand [%02x].\n", error_code);
        free(buffer);
        L0_close(&device);
        return 1;
    }

    if (bytes_to_generate != buffer_length)
    {
        printf("ERROR Failed to generate all bytes.\n");
        free(buffer);
        L0_close(&device);
        return 1;
    }

    printf("Successfully generated all bytes.\n");
    if((file = fopen(filename, "wb")) == NULL)
    {
        printf("ERROR Failed to open file to write.\n");
        free(buffer);
        L0_close(&device);
        return 1;
    }

    fwrite(buffer, bytes_to_generate, 1, file);

    fclose(file);
    free(buffer);
    L0_close(&device);
    return 0;
}
