#ifndef AUTH_H
#define AUTH_H

#define MAX_USERNAME 30
#define MAX_PASSWORD 30
#define USERS_FILE "data/users.txt"

int registerUser(void);
int loginUser(char *loggedInUser, int size);

#endif
