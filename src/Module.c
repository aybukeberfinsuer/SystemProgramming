#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Module.h"

// Yazma işlemi için verileri listeye ekler
void processWriteCommand(Dllist buffer, IS is) {
    for (int i = 1; i < is->NF; i += 2) {
        int count = atoi(is->fields[i]);  // Kelimenin sayısını al (örn. 10)
        char *ch = is->fields[i+1];       // Yazılacak karakteri al (örn. 'a')

        for (int j = 0; j < count; j++) {
            Jval val;
            if (strcmp(ch, "\\n") == 0) {  // Yeni satır karakteri kontrolü
                val = new_jval_s(strdup("\n"));  // Yeni satır karakteri olarak ekle
            } else if (strcmp(ch, "\\b") == 0) {  // Boşluk karakteri kontrolü
                val = new_jval_s(strdup(" "));  // Boşluk karakteri olarak ekle
            } else {
                val = new_jval_s(strdup(ch));  // Normal karakterleri ekle
            }
            dll_append(buffer, val);  // Listeye ekle
        }
    }
}

// Silme işlemi için verileri listeden kaldırır
void processDeleteCommand(Dllist buffer, IS is) {
    int count = atoi(is->fields[1]);
    char *ch = is->fields[2];

    Dllist tmp;
    dll_traverse(tmp, buffer) {
        if (count == 0) break;  // Silinecek miktar kadar silme işlemi yapıldıysa döngüden çık
        if (strcmp(jval_s(tmp->val), ch) == 0) {
            Dllist to_delete = tmp;
            tmp = tmp->blink;  // Geriye doğru ilerle
            dll_delete_node(to_delete);
            count--;
        }
    }
}

// Dosyanın sonuna gitmek için bir işlem yapar
void processGoToEndCommand(Dllist buffer, FILE *os) {
    fseek(os, 0, SEEK_END);  // Dosyanın sonuna git
}

// Dur komutunda tüm listeyi dosyaya yazdırır ve listeyi temizler
void processStopCommand(Dllist buffer, FILE *os) {
    Dllist node;
    dll_traverse(node, buffer) {
        fprintf(os, "%s", jval_s(node->val));  // Listede bulunan her öğeyi dosyaya yaz
    }
    free_dllist(buffer);  // Listeyi boşalt ve belleği serbest bırak
}

// Komutları işler ve uygun işlevleri çağırır
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

    Dllist buffer = new_dllist();  // Yeni bağlı liste oluştur

    while (get_line(is) >= 0) {
        if (strcmp(is->fields[0], "yaz:") == 0) {
            processWriteCommand(buffer, is);
        } else if (strcmp(is->fields[0], "sil:") == 0) {
            processDeleteCommand(buffer, is);
        } else if (strcmp(is->fields[0], "sonagit:") == 0) {
            processGoToEndCommand(buffer, os);
        } else if (strcmp(is->fields[0], "dur:") == 0) {
            processStopCommand(buffer, os);
            break;  // Döngüden çık
        }
    }

    fclose(os);
    jettison_inputstruct(is);
    if (!dll_empty(buffer)) free_dllist(buffer);  // Eğer liste boş değilse temizle
}