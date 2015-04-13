#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <string>
using namespace std;

void lsCom() {

}
bool isDependant = 0;
int main(int argc, char *argv[]) {
char *arg[argc];
if(argc < 1) {
    cout << "No arguements passed in" << endl;
    exit(1);
}
else {
    int currArg = 1;
    int SAP = 0; // Sub-Array Position
    string ArgStr;
    char* test = argv[1];
    while(currArg+1 <= argc) {
        SAP = 0;
        arg[SAP] = argv[currArg];
        ++SAP;
        int n = 0;
        for(int i = 0; i < strlen(argv[currArg]); ++i) {
            if(*(argv[currArg]+i) == ';' && i == (strlen(argv[currArg])-1)){
                *(argv[currArg]+i) = '\0';
                isDependant = 0;
            }
        }
        ++currArg;
        if((currArg) < argc && (argv[currArg]) != "||" && (argv[currArg]) != "&&") {
            arg[SAP] = argv[currArg];
            ++SAP;
        }
        else {
            arg[SAP] = NULL;
            ++SAP;
        }
        arg[SAP] = NULL;
        char *testa[3] = { NULL};
        if(execvp(arg[0], arg) == -1) {
            perror("The command could not be executed!");
            errno = 0;
        }
        /*else {
            execvp(arg[0], arg);
        }*/
        ++currArg;
    }
  }
return 0;
}
