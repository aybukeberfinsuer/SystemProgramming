#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Module.h"

void processWriteCommand(IS is, FILE *os) {
    for (int i = 1; i < is->NF; i += 2) {
        int count = atoi(is->fields[i]);  // Kelimenin sayısını al (örn. 10)
        char *ch = is->fields[i+1];       // Yazılacak karakteri al (örn. 'a')

        if (strcmp(ch, "\\n") == 0) {  // Eğer karakter yeni satır ise
            for (int j = 0; j < count; j++) {
                fprintf(os, "\n");  // Belirtilen sayıda yeni satır ekle
            }
        } else if (strcmp(ch, "\\b") == 0) {  // Eğer karakter boşluk ise
            for (int j = 0; j < count; j++) {
                fprintf(os, " ");  // Belirtilen sayıda boşluk ekle
            }
        } else {  // Normal karakterler için
            for (int j = 0; j < count; j++) {
                fprintf(os, "%s", ch);  // Karakteri belirtilen sayıda yaz
            }
        }
    }
    fflush(os);  // Her yazma işleminden sonra buffer'ı boşalt
}





void processDeleteCommand(IS is, FILE *os) {
    // 'sil:' komutunu işle
}

void processGoToEndCommand(IS is, FILE *os) {
    // Dosyanın sonuna git
    if (fseek(os, 0, SEEK_END) != 0) {
        perror("Dosya sonuna gitme hatası");
        exit(1);
    }
}

void processWriteCommand(IS is, FILE *os) {
    for (int i = 1; i < is->NF; i += 2) {
        int count = atoi(is->fields[i]);  // Kelimenin sayısını al (örn. 10)
        char *ch = is->fields[i+1];       // Yazılacak karakteri al (örn. 'a')

        if (strcmp(ch, "\\n") == 0) {  // Eğer karakter yeni satır ise
            for (int j = 0; j < count; j++) {
                fprintf(os, "\n");  // Belirtilen sayıda yeni satır ekle
            }
        } else if (strcmp(ch, "\\b") == 0) {  // Eğer karakter boşluk ise
            for (int j = 0; j < count; j++) {
                fprintf(os, " ");  // Belirtilen sayıda boşluk ekle
            }
        } else {  // Normal karakterler için
            for (int j = 0; j < count; j++) {
                fprintf(os, "%s", ch);  // Karakteri belirtilen sayıda yaz
            }
        }
    }
    fflush(os);  // Her yazma işleminden sonra buffer'ı boşalt
}


void processCommands(const char* inputFileName, const char* outputFileName) {
    IS is = new_inputstruct(inputFileName);
    if (is == NULL) {
        fprintf(stderr, "%s dosyasi bulunamadi ya da okuma izni yok\n", inputFileName);
        exit(1);
    }

    FILE *os = fopen(outputFileName, "w");
    if (os == NULL) {
        fprintf(stderr, "%s dosyasi yazma için açılamadı\n", outputFileName);
        jettison_inputstruct(is);
        exit(1);
    }

    while (get_line(is) >= 0) {
        if (strcmp(is->fields[0], "yaz:") == 0) {
            processWriteCommand(is, os);
        } else if (strcmp(is->fields[0], "sil:") == 0) {
            processDeleteCommand(is, os);
        } else if (strcmp(is->fields[0], "sonagit:") == 0) {
            processGoToEndCommand(is, os);
        } else if (strcmp(is->fields[0], "dur:") == 0) {
            processStopCommand(is, os);
            break;  // Döngüden çık
        }
    }

    fclose(os);
    jettison_inputstruct(is);
}