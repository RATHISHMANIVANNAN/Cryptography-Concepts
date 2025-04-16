#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int mod26(int x) {
    return (x % 26 + 26) % 26;
}

void ngramEncrypt(const char* plaintext, char* ciphertext, int n) {
    int len = strlen(plaintext);
    int i = 0;
    
    while (i < len) {
        for (int j = 0; j < n && i + j < len; j++) {
            if (isalpha(plaintext[i + j])) {
                int shift = 3 + j;  
                char base = isupper(plaintext[i + j]) ? 'A' : 'a';
                char ch = plaintext[i + j];
                
                ciphertext[i + j] = base + mod26((ch - base) + shift);
            } else {
                ciphertext[i + j] = plaintext[i + j];
            }
        }
        i += n;  
    }
    
    ciphertext[len] = '\0';  
}

void ngramDecrypt(const char* ciphertext, char* plaintext, int n) {
    int len = strlen(ciphertext);
    int i = 0;
    
    while (i < len) {
        for (int j = 0; j < n && i + j < len; j++) {
            if (isalpha(ciphertext[i + j])) {

                int shift = 3 + j; 
                char base = isupper(ciphertext[i + j]) ? 'A' : 'a';
                char ch = ciphertext[i + j];
                
                plaintext[i + j] = base + mod26((ch - base) - shift);
            } else {
                plaintext[i + j] = ciphertext[i + j];
            }
        }
        i += n;  
    }
    
    plaintext[len] = '\0';  
}

int main() {
    char plaintext[1000];
    char ciphertext[1000];
    char decrypted[1000];
    int n;
    
    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    
    size_t len = strlen(plaintext);
    if (len > 0 && plaintext[len-1] == '\n') {
        plaintext[len-1] = '\0';
        len--;
    }
    
    printf("Enter the n-gram size (e.g., 2 for bigram, 3 for trigram): ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Error: n-gram size must be positive\n");
        return 1;
    }
    
    ngramEncrypt(plaintext, ciphertext, n);
    printf("Encrypted: %s\n", ciphertext);
    
    ngramDecrypt(ciphertext, decrypted, n);
    printf("Decrypted: %s\n", decrypted);
    
    if (strcmp(plaintext, decrypted) == 0) {
        printf("Verification: Successful! Decryption matches original plaintext.\n");
    } else {
        printf("Verification: Failed! Decryption does not match original plaintext.\n");
    }
    
    return 0;
}