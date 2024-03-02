#include <stdio.h>
#include <string.h>

#include "secube/L0.h"

void print_buffer(uint16_t buff_len, uint8_t *buff)
{
    int i;
    for (i = 0; i < buff_len; i++)
    {
        printf("%02x ", buff[i]);
    }
}

int main(void)
{
    se3_disco_it it;
    se3_device device;

    uint16_t error_code = SE3_OK;
    uint16_t bytes_to_generate = 64;
    uint16_t buff_len = 64;
    uint16_t tmp_buff_len = buff_len; // create this temp variable as SEcube will overwrite our length value
    uint8_t buff[buff_len];

    if (bytes_to_generate > buff_len)
    {
        fprintf(stderr, "ERROR Trying to generate more bytes than the buffer size.\n");
        return 0;
    }

    memset(buff, 0, buff_len);

    L0_discover_init(&it);
    if (!L0_discover_next(&it))
    {
        fprintf(stderr, "ERROR SECube not found.\n");
        return 0;
    }

    error_code = L0_open(&device, &it.device_info, 0);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L0_open [%hd].\n", error_code);
        return 0;
    }

    printf("DEBUG BEFORE\t");
    print_buffer(buff_len, (uint8_t *)buff);
    printf("\n");

    error_code = L0_rand(&device, bytes_to_generate, &tmp_buff_len, buff);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L0_rand [%02x].\n", error_code);
        goto cleanup_L0_close;
    }

    printf("DEBUG AFTER\t");
    print_buffer(buff_len, (uint8_t *)buff);
    printf("\n");

    printf("DEBUG All bytes generated? %s\n",
        bytes_to_generate == tmp_buff_len ? "YES" : "NO");

cleanup_L0_close:
    printf("CLEANUP L0_close\n");
    L0_close(&device);
    return 0;
}
