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
    string ArgStr;
    cout << argv[1] << endl;
    cout << *(argv[1] + 1) << endl;
    char* test = argv[1];
    while(currArg+1 < argc) {
        arg[0] = argv[currArg];
        int n = 0;
        for(int i = 0; i < strlen(argv[currArg]); ++i) {
            if(*(argv[currArg+1]+i) == ';' && i == (strlen(argv[currArg])-1)){
                *(argv[currArg+1]+i) = '\0';
                isDependant = 0;
            }
        }
        arg[1] = argv[currArg+1];
        ++currArg;
    }
    arg[2] = NULL;
    /*
    while(currArg < argc){
        if(*(argv[currArg]).at(0) = '-') {
            if(*(argv[currArg]).at(*(argv[currArg]).size() - 1) == ';') {
                *argv[currArg].pop_back();
                endCommand = 1;
            }
            ArgStr += argv[currArg];
        }
        ++currArg;



    }*/
    execvp(arg[0], arg);
    //cout << get_current_dir_name() << endl;
    /*DIR *dirp;
    if( NULL == (dirp = opendir(get_current_dir_name()))) {
      perror("The directory could not be opened or does not exist. ");
      exit(1);
    }
    struct dirent *file;
    errno = 0;
    while(NULL != (file = readdir(dirp))) {
      cout << file->d_name << " ";
    }
    cout << endl;*/
  }
return 0;
}
