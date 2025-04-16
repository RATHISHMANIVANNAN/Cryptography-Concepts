#include <stdio.h>
#include <string.h>

void caesar_encrypt(char *text, int shift, char *result) {
    shift = ((shift % 26) + 26) % 26;
    
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (ch >= 'a' && ch <= 'z') {
            result[i] = ((ch - 'a' + shift) % 26) + 'a';
        } else if (ch >= 'A' && ch <= 'Z') {
            result[i] = ((ch - 'A' + shift) % 26) + 'A';
        } else {
            result[i] = ch;
        }
    }
    result[strlen(text)] = '\0';
}

void caesar_decrypt(char *text, int shift, char *result) {
    shift = ((shift % 26) + 26) % 26;
    caesar_encrypt(text, 26 - shift, result);
}

int main() {
    char plaintext[100];
    char encrypted[100];
    char decrypted[100];
    int shift;
    
    printf("Enter the text to encrypt: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    
    size_t len = strlen(plaintext);
    if (len > 0 && plaintext[len-1] == '\n') {
        plaintext[len-1] = '\0';
    }
    
    printf("Enter the shift value: ");
    scanf("%d", &shift);
    
    caesar_encrypt(plaintext, shift, encrypted);
    printf("Encrypted text: %s\n", encrypted);
    
    caesar_decrypt(encrypted, shift, decrypted);
    printf("Decrypted text: %s\n", decrypted);
    
    return 0;
}