#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Module.h"

Dllist cursor;  // Global cursor tanımı

// Yazma işlemi için verileri cursor'dan sonrasına listeye ekler
void processWriteCommand(Dllist buffer, IS is) {
    if (cursor == NULL) {
        cursor = buffer->flink;  // Cursor boşsa, listenin başına ayarla
    }

    for (int i = 1; i < is->NF; i += 2) {
        int count = atoi(is->fields[i]);
        char *ch = is->fields[i+1];

        for (int j = 0; j < count; j++) {
            Jval val;
            if (strcmp(ch, "\\n") == 0) {
                val = new_jval_s(strdup("\n"));
            } else if (strcmp(ch, "\\b") == 0) {
                val = new_jval_s(strdup(" "));
            } else {
                val = new_jval_s(strdup(ch));
            }
            dll_insert_a(cursor, val);  // Cursor'un sonrasına ekle
            cursor = cursor->flink;  // Cursor'u eklenen yeni elemana taşı
        }
    }
}

// Silme işlemi için verileri listeden kaldırır ve cursor'u günceller
// Silme işlemi için verileri listeden kaldırır ve cursor'u günceller
void processDeleteCommand(Dllist buffer, IS is) {
    if (cursor == NULL || cursor == buffer) {
        cursor = buffer->blink;  // Cursor boşsa veya listenin başındaysa, listenin sonuna ayarla
    }

    for (int i = 1; i < is->NF; i += 2) {
        int count = atoi(is->fields[i]);
        char *ch = is->fields[i + 1];
        Dllist temp = cursor;  // Her karakter için cursor'un başlangıç konumunu ayarla

        while (temp != buffer && count > 0) {
            if (strcmp(jval_s(temp->val), ch) == 0) {
                Dllist to_delete = temp;
                temp = temp->blink;  // Bir önceki elemana git
                dll_delete_node(to_delete);
                count--;
            } else {
                temp = temp->blink;  // Eşleşmeyen elemanlarda geriye git
            }
        }

        // Bir karakter için silme işlemi bittikten sonra, cursor'u son silinen elemanın bir sonrasına taşı
        cursor = temp;
    }
}


// Dosyanın sonuna gitmek yerine bağlı listenin sonuna cursor'u taşır
void processGoToEndCommand(Dllist buffer) {
    if (buffer->flink == buffer) {
        cursor = buffer;  // Liste boşsa cursor'u listenin kendisine (başlık düğümüne) ayarla
    } else {
        cursor = buffer->blink;  // Cursor'u listenin en son elemanına taşı
    }
}

// Dur komutunda tüm listeyi dosyaya yazdırır ve listeyi temizler
void processStopCommand(Dllist buffer, FILE *os) {
    Dllist node = buffer->flink;
    while (node != buffer) {
        fprintf(os, "%s", jval_s(node->val));
        node = node->flink;
    }
    free_dllist(buffer);
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

    Dllist buffer = new_dllist();
    cursor = NULL;  // Cursor'u başlangıçta NULL olarak ayarla

    while (get_line(is) >= 0) {
        if (strcmp(is->fields[0], "yaz:") == 0) {
            processWriteCommand(buffer, is);
        } else if (strcmp(is->fields[0], "sil:") == 0) {
            processDeleteCommand(buffer, is);
        } else if (strcmp(is->fields[0], "sonagit:") == 0) {
            processGoToEndCommand(buffer);  // 'sonagit:' komutunu işler
        } else if (strcmp(is->fields[0], "dur:") == 0) {
            processStopCommand(buffer, os);
            break;  // Döngüden çık
        }
    }

    printf("Islem tamamlandi.\n");
    fclose(os);
    jettison_inputstruct(is);
    if (!dll_empty(buffer)) free_dllist(buffer);
}
