#include <stdio.h>
#include <string.h>

#include "secube/L1.h"

#define KEY_COUNT 500
#define KEY_SIZE 32

typedef struct secube
{
    se3_disco_it it;
    se3_device dev;
    se3_session session;
} secube_t;

const uint8_t admin_pin[SE3_L1_PIN_SIZE] = {
    't', 'e', 's', 't', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void print_usage(void)
{
    printf("Usage: keys COMMAND\n");
    printf("List of valid commands:\n");
    printf("  list\n");
    printf("  add\n");
    printf("  clear\n");
}

uint16_t list_keys(secube_t secube)
{
    uint16_t error_code = 0;
    uint16_t count = 0;
    se3_key keys[KEY_COUNT];

    memset(keys, 0, sizeof(keys));

    error_code = L1_key_list(&secube.session, 0, sizeof(keys) / sizeof(se3_key), keys, &count);
    if (error_code != SE3_OK)
    {
        printf("ERROR: Failed to _L1_key_list [%x].\n", error_code);
        return error_code;
    }

    printf("INFO: Keys:\n");
    for (int i = 0; i < count; i++)
    {
        printf("(%u) %s\n", keys[i].id, keys[i].name);
        printf("  data_size: %hu\n", keys[i].data_size);
    }

    return error_code;
}

uint16_t populate_keys(secube_t secube)
{
    uint16_t error_code = 0;

    se3_key key;
    uint8_t key_data[KEY_SIZE];
    uint16_t rng_out_len = KEY_SIZE;

    printf("INFO: Keys added:\n");
    for (int i = 0; i < KEY_COUNT; i++)
    {
        memset(&key, 0, sizeof(key));
        memset(key_data, 0, KEY_SIZE);
        rng_out_len = KEY_SIZE;

        error_code = L0_rand(&secube.dev, KEY_SIZE, &rng_out_len, key_data);
        if (error_code != SE3_OK)
        {
            printf("ERROR: Failed to L0_rand [%x].\n", error_code);
            return error_code;
        }
        if (rng_out_len != KEY_SIZE)
        {
            printf("ERROR: Failed to generate all bytes\n");
            return 1;
        }

        sprintf(key.name, "S1%04u", i);
        key.name_size = 5;
        key.validity = time(0) + (24 * 60 * 60);
        key.data_size = KEY_SIZE;
        key.data = key_data;
        key.id = 10000 + i;

        error_code = L1_key_edit(&secube.session, SE3_KEY_OP_INSERT, &key);
        if (error_code != SE3_OK)
        {
            printf("ERROR: Failed to L1_key_edit [%x].\n", error_code);
            return error_code;
        }
        printf("(%u) %s - data_size: %hu\n", key.id, key.name, key.data_size);
    }

    return error_code;
}

uint16_t clear_keys(secube_t secube)
{
    uint16_t error_code = 0;
    uint16_t count = 0;
    se3_key keys[KEY_COUNT];

    memset(keys, 0, sizeof(keys));

    error_code = L1_key_list(&secube.session, 0, sizeof(keys) / sizeof(se3_key), keys, &count);
    if (error_code != SE3_OK)
    {
        printf("ERROR: Failed to L1_key_list [%x].\n", error_code);
        return error_code;
    }

    printf("INFO: Keys deleted:\n");
    for (int i = 0; i < count; i++)
    {
        se3_key key;
        key.id = keys[i].id;

        uint16_t tmp_error_code = L1_key_edit(&secube.session, SE3_KEY_OP_DELETE, &key);
        if (tmp_error_code != SE3_OK)
        {
            error_code = tmp_error_code;
            continue;
        }
        printf("(%u) %s\n", keys[i].id, keys[i].name);

    }

    return error_code;
}

int main(int argc, char **argv)
{
    uint16_t error_code = 0;
    secube_t secube;

    uint16_t (*handler)(secube_t secube);

    if (argc != 2)
    {
        print_usage();
        error_code = 1;
        goto exit;
    }

    if (strcmp("list", argv[1]) == 0)
    {
        handler = &list_keys;
    }
    else if (strcmp("add", argv[1]) == 0)
    {
        handler = &populate_keys;
    }
    else if (strcmp("clear", argv[1]) == 0)
    {
        handler = &clear_keys;
    }
    else
    {
        printf("ERROR: Invalid command.\n");
        print_usage();
        error_code = 1;
        goto exit;
    }

    memset(&secube, 0, sizeof(secube_t));

    // Look for SEcube
    L0_discover_init(&secube.it);
    if (!L0_discover_next(&secube.it))
    {
        error_code = 1;
        printf("ERROR: SEcube not found.\n");
        goto exit;
    }
    printf("INFO: SEcube found.\n");

    // L0_open
    error_code = L0_open(&secube.dev, &secube.it.device_info, 0);
    if (error_code != SE3_OK)
    {
        printf("ERROR: Failed to L0_open [%x].\n", error_code);
        goto exit;
    }
    printf("INFO: L0 opened.\n");

    // L1_login
    error_code = L1_login(&secube.session, &secube.dev, admin_pin, SE3_ACCESS_ADMIN, false);
    if (error_code == SE3_ERR_OPENED)
    {
        printf("INFO: An existing session exists. Trying again with force logout.\n");
        error_code = L1_login(&secube.session, &secube.dev, admin_pin, SE3_ACCESS_ADMIN, true);
    }
    if (error_code != SE3_OK)
    {
        printf("ERROR: Failed to L1_login [%x].\n", error_code);
        goto cleanup_l0;
    }
    printf("INFO: L1 login successful.\n");

    // Execute action
    error_code = (*handler)(secube);

cleanup_l1:
    printf("CLEANUP: L1_logout\n");
    error_code = L1_logout(&secube.session);
    if (error_code != SE3_OK)
        printf("ERROR: Failed to L1_logout [%hd].\n", error_code);

cleanup_l0:
    printf("CLEANUP: L0_close\n");
    L0_close(&secube.dev);

exit:
    return error_code;
}