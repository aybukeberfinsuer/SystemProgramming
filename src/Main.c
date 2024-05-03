#include <stdio.h>
#include <stdlib.h>
#include "../lib/libfdr/fields.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Kullanim: %s okunacak_dosya yazilacak_dosya\n", argv[0]);
        return 1;
    }

    IS kilit = new_inputstruct(argv[1]); // İlk argüman olarak girilen dosya yolu alınır
    if (kilit == NULL) {
        fprintf(stderr, "%s dosyasi bulunamadi\n", argv[1]);
        return 1;
    }

    // İkinci argüman olarak girilen dosya yoluna yazılacak işlemleri yapın
    FILE *yazilacak_dosya = fopen(argv[2], "w");
    if (yazilacak_dosya == NULL) {
        fprintf(stderr, "%s dosyasi olusturulamadi\n", argv[2]);
        return 1;
    }

    // Okuma ve yazma işlemleri
    int i;
    while (get_line(kilit) >= 0) {
        for (i = 0; i < kilit->NF; i++) {
            fprintf(yazilacak_dosya, "%s ", kilit->fields[i]);
        }
        fprintf(yazilacak_dosya, "\n");
    }

    // Dosyaları kapatın
    fclose(yazilacak_dosya);
    jettison_inputstruct(kilit);

    printf("Islem tamamlandi\n");

    return 0;
}
