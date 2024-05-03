#include <stdio.h>
#include <stdlib.h>
#include "../lib/libfdr/fields.h"


int main(){
    
    IS kilit = new_inputstruct(".kilit"); // kilit dosyasi okunur
    if (kilit == NULL)                    // kilit dosyasi yoksa hata verir ve cikiş yapar
    {
        fprintf(stderr, ".kilit dosyasi bulunamadi");
        return 0;
    }
}