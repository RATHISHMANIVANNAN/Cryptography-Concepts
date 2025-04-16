#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void railFenceEncrypt(const char* text, int rails, char* result) {
    int len = strlen(text);
    char rail[rails][len];

    for (int i = 0; i < rails; i++)
        for (int j = 0; j < len; j++)
            rail[i][j] = '\n';

    int row = 0;
    int dir_down = 0;

    for (int i = 0; i < len; i++) {
        if (row == 0 || row == rails - 1)
            dir_down = !dir_down;

        rail[row][i] = text[i];

        row += dir_down ? 1 : -1;
    }

    int idx = 0;
    for (int i = 0; i < rails; i++)
        for (int j = 0; j < len; j++)
            if (rail[i][j] != '\n')
                result[idx++] = rail[i][j];

    result[idx] = '\0';
}

void railFenceDecrypt(const char* cipher, int rails, char* result) {
    int len = strlen(cipher);
    char rail[rails][len];

    for (int i = 0; i < rails; i++)
        for (int j = 0; j < len; j++)
            rail[i][j] = '\n';

    int row = 0;
    int dir_down = 0;
    for (int i = 0; i < len; i++) {
        if (row == 0 || row == rails - 1)
            dir_down = !dir_down;

        rail[row][i] = '*';
        row += dir_down ? 1 : -1;
    }

    int idx = 0;
    for (int i = 0; i < rails; i++) {
        for (int j = 0; j < len; j++) {
            if (rail[i][j] == '*' && idx < len)
                rail[i][j] = cipher[idx++];
        }
    }

    row = 0;
    dir_down = 0;
    for (int i = 0; i < len; i++) {
        if (row == 0 || row == rails - 1)
            dir_down = !dir_down;

        result[i] = rail[row][i];
        row += dir_down ? 1 : -1;
    }
    result[len] = '\0';
}

int main() {
    const char* message = "WEAREDISCOVEREDRUNATONCE";
    int rails = 6;

    char encrypted[100];
    char decrypted[100];

    railFenceEncrypt(message, rails, encrypted);
    printf("Encrypted: %s\n", encrypted);

    railFenceDecrypt(encrypted, rails, decrypted);
    printf("Decrypted: %s\n", decrypted);

    return 0;
}
