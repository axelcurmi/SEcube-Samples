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
    uint16_t count = 0;

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
        return 0;
    }

    error_code = L0_open(&device, &device_info, 0);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L0_open [%hd].\n", error_code);
        return 0;
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

    se3_algo algorithms[10];
    memset(algorithms, 0, sizeof(se3_algo) * 10);

    error_code = L1_get_algorithms(&session, 0, 10, algorithms, &count);
    if (error_code != SE3_OK)
    {
        fprintf(stderr, "ERROR Failed to L1_get_algorithms [%hd].\n", error_code);
        goto cleanup_L1_logout;
    }

    printf("DEBUG Algorithms count=%hd\n", count);
    for (int i = 0; i < count; i++)
    {
        printf("DEBUG %s\n", algorithms[i].name);
    }

    se3_key keys[10];
    memset(keys, 0, sizeof(se3_key) * 10);

    count = 0;
    error_code = L1_key_list(&session, 0, 10, keys, &count);
    if (error_code != SE3_OK)
    {
            fprintf(stderr, "ERROR Failed to L1_key_list [%hd].\n", error_code);
            goto cleanup_L1_logout;
    }

    printf("DEBUG count=%hd\n", count);
    for (int i = 0; i < count; i++)
    {
            printf("DEBUG (%d) %s\n", keys[i].id, keys[i].name);
    }

    if (count == 1)
    {
        se3_key key;
        key.id = 0;

        error_code = L1_key_edit(&session, SE3_KEY_OP_DELETE, &key);
        if (error_code != SE3_OK)
        {
            fprintf(stderr, "ERROR Failed to L1_key_edit (DELETE) [%hd].\n", error_code);
            goto cleanup_L1_logout;
        }
        printf("DEBUG Deleted key successfully.\n");
        count -= 1;
    }

    if (count == 0)
    {
        uint8_t data[32];
        se3_key key;

        memset(data, 0, 32);

        key.id = 0;
        key.data = data;
        key.data_size = 32;
        key.name_size = sprintf(key.name, "GKE Key");
        key.validity = time(0) + (365 * 24 * 60);

        error_code = L1_key_edit(&session, SE3_KEY_OP_INSERT, &key);
        if (error_code != SE3_OK)
        {
            fprintf(stderr, "ERROR Failed to L1_key_edit (INSERT) [%hd].\n", error_code);
            goto cleanup_L1_logout;
        }
        printf("DEBUG Added new key successfully.\n");
    }

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
