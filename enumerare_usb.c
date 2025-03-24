#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

int main(void) {
    libusb_context *context = NULL;
    libusb_device **list = NULL;
    ssize_t count;
    int rc;


    rc = libusb_init(&context);
    if (rc < 0) {
        fprintf(stderr, "Eroare libusb: %s\n", libusb_strerror(rc));
        return EXIT_FAILURE;
    }


    count = libusb_get_device_list(context, &list);
    if (count < 0) {
        fprintf(stderr, "Eroare: %s\n", libusb_strerror((int)count));
        libusb_exit(context);
        return EXIT_FAILURE;
    }

    printf("USB gasite: %zd\n", count);


    for (ssize_t idx = 0; idx < count; idx++) {
        libusb_device *device = list[idx];
        struct libusb_device_descriptor desc;


        rc = libusb_get_device_descriptor(device, &desc);
        if (rc < 0) {
            fprintf(stderr, "Eroare: %s\n", libusb_strerror(rc));
            continue;
        }

        printf("\nDispozitiv %zd:\n", idx);
        printf("  ID Vendor : 0x%04x\n", desc.idVendor);
        printf("  ID Product: 0x%04x\n", desc.idProduct);

  
        libusb_device_handle *handle;
        rc = libusb_open(device, &handle);
        if (rc == 0 && handle) {
            unsigned char string[256];

            if (desc.iManufacturer) {
                if (libusb_get_string_descriptor_ascii(handle, desc.iManufacturer,
                                                       string, sizeof(string)) > 0) {
                    printf("  Producator: %s\n", string);
                }
            }
            if (desc.iProduct) {
                if (libusb_get_string_descriptor_ascii(handle, desc.iProduct,
                                                       string, sizeof(string)) > 0) {
                    printf("  Produs: %s\n", string);
                }
            }

            libusb_close(handle);
        } else {
            printf("  (Nu s-a putut deschide dispozitivul)\n");
        }
    }

    libusb_free_device_list(list, 1);
    libusb_exit(context);

    return EXIT_SUCCESS;
}
