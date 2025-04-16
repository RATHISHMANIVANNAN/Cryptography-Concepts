#include <stdio.h>
#include <string.h>

void atbash_cipher(char *text, char *result) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (ch >= 'A' && ch <= 'Z')
            result[i] = 'Z' - (ch - 'A');
        else if (ch >= 'a' && ch <= 'z')
            result[i] = 'z' - (ch - 'a');
        else
            result[i] = ch;
    }
    result[strlen(text)] = '\0';
}

int main() {
    char input[100];
    char output[100];
    
    printf("Enter text to encrypt using Atbash cipher: ");
    fgets(input, sizeof(input), stdin);
    
    if (input[strlen(input) - 1] == '\n')
        input[strlen(input) - 1] = '\0';
    
    atbash_cipher(input, output);
    
    printf("Original text: %s\n", input);
    printf("Encrypted text: %s\n", output);
    
    return 0;
}