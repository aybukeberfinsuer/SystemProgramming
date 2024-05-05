#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Module.h"
#include "../lib/libfdr/fields.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Kullanim: %s okunacak_dosya yazilacak_dosya\n", argv[0]);
        return 1;
    }

    // Okunacak dosyanın varlığı ve okuma izni kontrolü
    FILE *temp_file = fopen(argv[1], "r");
    if (temp_file == NULL) {
        fprintf(stderr, "%s dosyasi bulunamadi ya da okuma izni yok\n", argv[1]);
        return 1;
    }
    fclose(temp_file); // Dosyayı kontrol için açıp kapattık

    IS is_ = new_inputstruct(argv[1]); // İlk argüman olarak girilen dosya yolu alınır
    if (is_ == NULL) {
        fprintf(stderr, "%s dosyasi işlenemiyor\n", argv[1]);
        return 1;
    }

    // Çıktı dosyasını sadece varsa aç
    FILE *os_ = fopen(argv[2], "r");
    if (os_ == NULL) {
        fprintf(stderr, "%s dosyasi bulunamadi\n", argv[2]);
        jettison_inputstruct(is_);
        return 1;
    }
    fclose(os_);  // Kontrol için açılan dosyayı kapat

    // Çıktı dosyasını yazma modunda aç
    os_ = fopen(argv[2], "w");
    if (os_ == NULL) {
        fprintf(stderr, "%s dosyasi yazma için açılamadı\n", argv[2]);
        jettison_inputstruct(is_);
        return 1;
    }

    while (get_line(is_) >= 0) {
        fprintf(os_, "Satır %d: %s\n", is_->line, is_->text1); // Dosyaya satır numarası ve metni yazdır
        for (int i = 0; i < is_->NF; i++) {
            fprintf(os_, "Kelime %d: %s\n", i + 1, is_->fields[i]); // Dosyaya her kelimeyi yazdır
        }
    }

    // (Burada dosya işlemlerinizi yapabilirsiniz.)

    // Dosyaları kapatın
    fclose(os_);
    jettison_inputstruct(is_);

    printf("Islem tamamlandi\n");

    return 0;
}
