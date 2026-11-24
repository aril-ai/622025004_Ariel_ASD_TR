#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "utils.h"

static int isValidCredential(const char *text, int maxLength) {
    int length = (int)strlen(text);
    return length > 0 && length < maxLength && strchr(text, ' ') == NULL;
}

static int usernameExists(const char *username) {
    char storedUser[MAX_USERNAME];
    char storedPass[MAX_PASSWORD];
    FILE *file = fopen(USERS_FILE, "r");

    if (file == NULL) {
        return 0;
    }
    while (fscanf(file, "%29s %29s", storedUser, storedPass) == 2) {
        if (strcmp(storedUser, username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int registerUser(void) {
    char username[MAX_INPUT];
    char password[MAX_INPUT];
    FILE *file;

    readLine("Username: ", username, sizeof(username));
    if (!isValidCredential(username, MAX_USERNAME)) {
        printf("Username tidak valid (kosong, mengandung spasi, atau terlalu panjang).\n");
        return 0;
    }
    if (usernameExists(username)) {
        printf("Username sudah digunakan.\n");
        return 0;
    }
    readLine("Password: ", password, sizeof(password));
    if (!isValidCredential(password, MAX_PASSWORD)) {
        printf("Password tidak valid (kosong, mengandung spasi, atau terlalu panjang).\n");
        return 0;
    }

    file = fopen(USERS_FILE, "a");
    if (file == NULL) {
        printf("Gagal membuka database akun.\n");
        return 0;
    }
    fprintf(file, "%s %s\n", username, password);
    fclose(file);
    printf("Register berhasil. Silakan login.\n");
    return 1;
}

int loginUser(char *loggedInUser, int size) {
    char username[MAX_INPUT];
    char password[MAX_INPUT];
    char storedUser[MAX_USERNAME];
    char storedPass[MAX_PASSWORD];
    FILE *file;

    readLine("Username: ", username, sizeof(username));
    readLine("Password: ", password, sizeof(password));
    if (strlen(username) == 0 || strlen(password) == 0) {
        printf("Username dan password tidak boleh kosong.\n");
        return 0;
    }

    file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        printf("Login gagal, belum ada akun terdaftar.\n");
        return 0;
    }
    while (fscanf(file, "%29s %29s", storedUser, storedPass) == 2) {
        if (strcmp(storedUser, username) == 0 && strcmp(storedPass, password) == 0) {
            fclose(file);
            strncpy(loggedInUser, username, size - 1);
            loggedInUser[size - 1] = '\0';
            printf("Login berhasil. Selamat datang, %s!\n", loggedInUser);
            return 1;
        }
    }
    fclose(file);
    printf("Login gagal, username atau password salah.\n");
    return 0;
}
