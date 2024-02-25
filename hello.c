#include <stdio.h>

#include "secube/L1.h"

void print_serial(uint8_t *serial)
{
    printf("SERIAL ");
    for (int i = 0; i < SE3_SN_SIZE; i++)
    {
        printf("%02d ", serial[i]);
    }
    printf("\n");
}

int main(void)
{
    se3_disco_it it;
    se3_device_info device_info;
    se3_device device;
    se3_session session;

    uint16_t error_code = SE3_OK;

    const uint8_t admin_pin[SE3_L1_PIN_SIZE] = {
        't', 'e', 's', 't', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    L0_discover_init(&it);
    if (L0_discover_next(&it))
    {
        device_info = it.device_info;
        printf("HELLO %s\n", device_info.hello_msg);
        print_serial(device_info.serialno);
    }
    else
    {
        fprintf(stderr, "ERROR SECube not found.\n");
        return 1;
    }

    error_code = L0_open(&device, &device_info, 0);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L0_open [%hd].\n", error_code);
        return 1;
    }

    error_code = L1_login(&session, &device, admin_pin, SE3_ACCESS_ADMIN, false);
    if (error_code == SE3_ERR_OPENED)
    {
        fprintf(stderr, "Failed to L1_login [SE3_ERR_OPENED]. Trying again...\n");
        error_code = L1_login(&session, &device, admin_pin, SE3_ACCESS_ADMIN, true);
    }
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L1_login [%hd].\n", error_code);
        goto cleanup_L0_close;
    }

    printf("DEBUG Login successful!\n");

cleanup_L1_logout:
    printf("CLEANUP L1_logout\n");
    error_code = L1_logout(&session);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L1_logout [%hd].\n", error_code);
    }

cleanup_L0_close:
    printf("CLEANUP L0_close\n");
    L0_close(&device);

    return 0;
}
