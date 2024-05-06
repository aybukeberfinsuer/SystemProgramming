#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Module.h"
#include "../lib/libfdr/fields.h"
#include "../lib/libfdr/dllist.h"

int checkFileExtension(const char *filename, const char *extension) {
    const char *dot = strrchr(filename, '.'); // Dosya adındaki son noktayı bul
    if (!dot || dot == filename) return 0; // Nokta yoksa veya dosya adı ile aynıysa hata
    return strcmp(dot + 1, extension) == 0; // Uzantıyı kontrol et
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Kullanim sekli su sekilde olmali 2 parametre almali: %s okunacak_dosya yazilacak_dosya\n", argv[0]);
        return 1;
    }

    // Dosya uzantısını kontrol et
    if (!checkFileExtension(argv[1], "dat") && !checkFileExtension(argv[1], "txt")) {
        fprintf(stderr, "Hata: Giris dosyasi uygun formatta degil (sadece .dat veya .txt uzantili dosyalar desteklenir).\n");
        return 1;
    }

    // İşlem komutlarını çalıştır
    processCommands(argv[1], argv[2]);
    return 0;
}