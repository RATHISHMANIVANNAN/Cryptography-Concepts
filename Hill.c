#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int mod26(int x) {
    return (x % 26 + 26) % 26;
}

char* padText(const char* text, int keySize) {
    int len = strlen(text);
    int paddingNeeded = (keySize - (len % keySize)) % keySize;
    
    char* paddedText = (char*)malloc(len + paddingNeeded + 1);
    strcpy(paddedText, text);
    
    for (int i = 0; i < paddingNeeded; i++) {
        paddedText[len + i] = 'X';
    }
    paddedText[len + paddingNeeded] = '\0';
    
    return paddedText;
}

void hillEncrypt3x3(const char* plaintext, int key[3][3], char* ciphertext) {
    int len = strlen(plaintext);
    
    for (int i = 0; i < len; i += 3) {
        int p1 = toupper(plaintext[i]) - 'A';
        int p2 = toupper(plaintext[i + 1]) - 'A';
        int p3 = toupper(plaintext[i + 2]) - 'A';
        
        ciphertext[i] = 'A' + mod26(key[0][0] * p1 + key[0][1] * p2 + key[0][2] * p3);
        ciphertext[i + 1] = 'A' + mod26(key[1][0] * p1 + key[1][1] * p2 + key[1][2] * p3);
        ciphertext[i + 2] = 'A' + mod26(key[2][0] * p1 + key[2][1] * p2 + key[2][2] * p3);
    }
    
    ciphertext[len] = '\0';
}

int determinant3x3(int matrix[3][3]) {
    return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
           matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
           matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

int modInverse(int a) {
    a = mod26(a);
    for (int i = 1; i < 26; i++) {
        if (mod26(a * i) == 1) {
            return i;
        }
    }
    return -1; 
}

void adjugate3x3(int matrix[3][3], int adj[3][3]) {
    adj[0][0] = mod26(matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
    adj[0][1] = mod26(matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2]);
    adj[0][2] = mod26(matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]);
    
    adj[1][0] = mod26(matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2]);
    adj[1][1] = mod26(matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]);
    adj[1][2] = mod26(matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2]);
    
    adj[2][0] = mod26(matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    adj[2][1] = mod26(matrix[0][1] * matrix[2][0] - matrix[0][0] * matrix[2][1]);
    adj[2][2] = mod26(matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
}

int inverse3x3(int matrix[3][3], int inv[3][3]) {
    int det = mod26(determinant3x3(matrix));
    int detInv = modInverse(det);
    
    if (detInv == -1) {
        return 0; 
    }
    
    int adj[3][3];
    adjugate3x3(matrix, adj);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            inv[i][j] = mod26(adj[i][j] * detInv);
        }
    }
    
    return 1; 
}

int hillDecrypt3x3(const char* ciphertext, int key[3][3], char* plaintext) {
    int invKey[3][3];
    if (!inverse3x3(key, invKey)) {
        return 0;
    }
    
    int len = strlen(ciphertext);
    
    for (int i = 0; i < len; i += 3) {
        int c1 = toupper(ciphertext[i]) - 'A';
        int c2 = toupper(ciphertext[i + 1]) - 'A';
        int c3 = toupper(ciphertext[i + 2]) - 'A';
        
        plaintext[i] = 'A' + mod26(invKey[0][0] * c1 + invKey[0][1] * c2 + invKey[0][2] * c3);
        plaintext[i + 1] = 'A' + mod26(invKey[1][0] * c1 + invKey[1][1] * c2 + invKey[1][2] * c3);
        plaintext[i + 2] = 'A' + mod26(invKey[2][0] * c1 + invKey[2][1] * c2 + invKey[2][2] * c3);
    }
    
    plaintext[len] = '\0';
    return 1; 
}

int main() {
    int key[3][3] = {
        {6, 24, 1},
        {13, 16, 10},
        {20, 17, 15}
    };
    
    char plaintext[100];
    char ciphertext[100];
    char decrypted[100];
    
    printf("Enter plaintext (uppercase letters only): ");
    scanf("%s", plaintext);
    
    for (int i = 0; plaintext[i]; i++) {
        plaintext[i] = toupper(plaintext[i]);
        
        if (plaintext[i] < 'A' || plaintext[i] > 'Z') {
            printf("Error: Please enter uppercase letters (A-Z) only.\n");
            return 1;
        }
    }
    
    char* paddedText = padText(plaintext, 3);
    printf("Padded plaintext: %s\n", paddedText);
    
    hillEncrypt3x3(paddedText, key, ciphertext);
    printf("Encrypted: %s\n", ciphertext);
    
    if (hillDecrypt3x3(ciphertext, key, decrypted)) {
        printf("Decrypted: %s\n", decrypted);
    } else {
        printf("Error: Key matrix is not invertible in mod 26.\n");
    }
    
    free(paddedText);
    return 0;
}