#include <stdio.h>
#include <string.h>

#include "secube/L0.h"

int main(void)
{
    se3_disco_it it;
    se3_device device;

    const char *data_in = "You live in my dream state";
    uint16_t data_in_len = strlen(data_in);
    uint8_t data_out[data_in_len + 1];

    uint16_t error_code = SE3_OK;

    memset(data_out, 0, data_in_len + 1);

    L0_discover_init(&it);
    if (!L0_discover_next(&it))
    {
        fprintf(stderr, "ERROR SECube not found.\n");
        return 1;
    }

    error_code = L0_open(&device, &it.device_info, 0);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L0_open [%hd].\n", error_code);
        return 1;
    }

    error_code = L0_echo(&device, data_in, data_in_len, data_out);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L0_echo [%hd].\n", error_code);
        goto cleanup_L0_close;
    }

    printf("DEBUG IN %s\n", data_in);
    printf("DEBUG OUT %s\n", data_out);

return 0;

cleanup_L0_close:
    printf("CLEANUP L0_close\n");
    L0_close(&device);
    return 1;    
}
