#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Module.h"
#include "../lib/libfdr/fields.h"
#include "../lib/libfdr/dllist.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Kullanim: %s okunacak_dosya yazilacak_dosya\n", argv[0]);
        return 1;
    }

    processCommands(argv[1], argv[2]);
    printf("Islem tamamlandi\n");
    return 0;
}