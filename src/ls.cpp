#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <vector>
#include <errno.h>
#include <string.h>
#include <list>
using namespace std;

bool noArg = false, flagA = false, flagL = false, flagR = false;
void findArg(int argc, char* argv[]) {
    if(!(noArg)) {
        int ArrInd = 1, ArrPos = 0;
        while(ArrInd < argc) {
            if(*(argv[ArrInd]) == '-') {
                ArrPos = 0;
                while(*(argv[ArrInd] + ArrPos) != '\0') {
                    if(*(argv[ArrInd] + ArrPos) == 'a') {
                        flagA = true;
                    }
                    else if(*(argv[ArrInd] + ArrPos) == 'l') {
                        flagL = true;
                    }
                    else if(*(argv[ArrInd] + ArrPos) == 'R') {
                        flagR = true;
                    }
                    ++ArrPos;
                }
                ++ArrInd;
            }

            else {
                ++ArrInd;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    string s;
    list<string> vs;
    int size = PATH_MAX;
    char* buf;
    struct dirent *cDirent;
    DIR *cDir;
    if(argc == 1) {
        noArg = true;

    }
    char* pathname = getcwd(buf, (size_t)size);
    cout << pathname << endl;
    cDir = opendir(pathname);
    //cerr << "happened here " << endl;
    if(cDir == NULL) {
        perror("It didn't work");
        errno = 0;
        return 1;
    }

    while((cDirent = readdir(cDir)) != NULL) {
        s = cDirent->d_name;
        vs.push_back(s);
        cout << cDirent->d_name << endl;
    }
    closedir(cDir);
    vs.sort();
    for(auto i = vs.begin(); i != vs.end(); ++i) {
        cout << *i << endl;
    }

    /*struct stat s;
    stat("ls.cpp", &s);*/

    
    //findArg(argc, argv);

    if(flagA) {cout << "1" << endl;}
    if(flagL) {cout << "2" << endl;}
    if(flagR) {cout << "3" << endl;}


    return 0;
}

