#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char letter;
    int position;
    int rank;
} KeyMapping;

int compareKeyMapping(const void* a, const void* b) {
    return ((KeyMapping*)a)->letter - ((KeyMapping*)b)->letter;
}

char* prepareText(const char* text) {
    int len = strlen(text);
    char* result = (char*)malloc(len + 1);
    int j = 0;
    
    for (int i = 0; i < len; i++) {
        if (!isspace(text[i])) {
            result[j++] = toupper(text[i]);
        }
    }
    result[j] = '\0';
    return result;
}

void analyzeKeyword(const char* keyword, KeyMapping* keyMap, int* ranks) {
    int klen = strlen(keyword);
    
    for (int i = 0; i < klen; i++) {
        keyMap[i].letter = toupper(keyword[i]);
        keyMap[i].position = i;
    }
    
    qsort(keyMap, klen, sizeof(KeyMapping), compareKeyMapping);

    int currentRank = 0;
    keyMap[0].rank = currentRank;
    
    for (int i = 1; i < klen; i++) {
        if (keyMap[i].letter != keyMap[i-1].letter) {
            currentRank++;
        }
        keyMap[i].rank = currentRank;
    }
    
    for (int i = 0; i < klen; i++) {
        ranks[keyMap[i].position] = keyMap[i].rank;
    }
}

void myszkowskiEncrypt(const char* plaintext, const char* keyword, char* ciphertext) {
    int plen = strlen(plaintext);
    int klen = strlen(keyword);
    
    KeyMapping* keyMap = (KeyMapping*)malloc(klen * sizeof(KeyMapping));
    int* ranks = (int*)malloc(klen * sizeof(int));
    
    analyzeKeyword(keyword, keyMap, ranks);
    
    int maxRank = 0;
    for (int i = 0; i < klen; i++) {
        if (ranks[i] > maxRank) {
            maxRank = ranks[i];
        }
    }
    
    int idx = 0;
    
    for (int rank = 0; rank <= maxRank; rank++) {
        for (int col = 0; col < klen; col++) {
            if (ranks[col] == rank) {
                for (int row = 0; row * klen + col < plen; row++) {
                    int pos = row * klen + col;
                    if (pos < plen) {
                        ciphertext[idx++] = plaintext[pos];
                    }
                }
            }
        }
    }
    
    ciphertext[idx] = '\0';
    
    free(keyMap);
    free(ranks);
}

void myszkowskiDecrypt(const char* ciphertext, const char* keyword, char* plaintext) {
    int clen = strlen(ciphertext);
    int klen = strlen(keyword);
    
    KeyMapping* keyMap = (KeyMapping*)malloc(klen * sizeof(KeyMapping));
    int* ranks = (int*)malloc(klen * sizeof(int));
    
    analyzeKeyword(keyword, keyMap, ranks);
    
    int maxRank = 0;
    for (int i = 0; i < klen; i++) {
        if (ranks[i] > maxRank) {
            maxRank = ranks[i];
        }
    }
    
    int completeRows = clen / klen;
    int remaining = clen % klen;
    
    int* colSize = (int*)calloc(klen, sizeof(int));
    for (int col = 0; col < klen; col++) {
        colSize[col] = completeRows;

        int colPos = 0;
        for (int i = 0; i <= ranks[col]; i++) {
            for (int j = 0; j < klen; j++) {
                if (ranks[j] == i) {
                    colPos++;
                    if (colPos <= remaining) {
                        colSize[j]++;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < clen; i++) {
        plaintext[i] = ' ';
    }
    plaintext[clen] = '\0';
    
    int idx = 0;
    for (int rank = 0; rank <= maxRank; rank++) {
        for (int col = 0; col < klen; col++) {
            if (ranks[col] == rank) {
                for (int row = 0; row < colSize[col]; row++) {
                    int pos = row * klen + col;
                    if (pos < clen) {
                        plaintext[pos] = ciphertext[idx++];
                    }
                }
            }
        }
    }
    
    free(keyMap);
    free(ranks);
    free(colSize);
}

int main() {
    char plaintext[1000];
    char keyword[100];
    char ciphertext[1000];
    char decrypted[1000];
    
    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; 
    
    printf("Enter keyword: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    char* preparedText = prepareText(plaintext);
    
    myszkowskiEncrypt(preparedText, keyword, ciphertext);
    printf("Encrypted: %s\n", ciphertext);
    
    myszkowskiDecrypt(ciphertext, keyword, decrypted);
    printf("Decrypted: %s\n", decrypted);
    
    free(preparedText);
    return 0;
}