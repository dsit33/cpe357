#ifndef OPS_H
#define OPS_H

#define USAGE "mytr \"set1\" \"set2\" || -d \"set1\""

typedef struct {
   char* set1;
   char* set2;
   int dFlag;
} ProgState;

void validateArgc(int argc);
int checkFlag(char* arg, ProgState* prog);
void getSets(char* set1, char* set2, ProgState* prog);
void iterateSet(char* arg, ProgState* prog);
char getCharAt(char* arg, int* i);
int isMatch(int c, ProgState* prog);
void replace(int c, ProgState* prog);

#endif
