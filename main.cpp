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
int main() {
    string str;
    int strSize = 0;
    int pos, arrPos;
    char *arr[3];
    while(1) {
        cout << "$ ";
        getline(cin, str);
        char *arg = (char*) malloc(256);
        char *comm = (char*) malloc(256);
        string sarg, scomm;
        pos = 0;
        arrPos = 0;
        strSize = str.size();
        while(pos < strSize) {
            if(str.at(pos) == ' ') {
                while(pos < strSize && str.at(pos) == ' ') {
                    ++pos;
                }
                if(pos >= strSize) {
                    break;
                }
            }
            while(pos < strSize && str.at(pos) != ' ') {
                if(str.at(pos) == ';') {
                    execvp(arr[0], arr);
                    break;
                }
                /*else if(str.at(pos) == '|') {
                    if(pos+1 < strSize)*/
                else {
                    scomm += str.at(pos);
                }
                ++pos;
            }

            strcpy(comm,scomm.c_str());
            cout << *comm << endl; 
            ++pos; // This is to ommit the whitespace character following the command
            arr[arrPos] = comm;
            ++arrPos;

            while(pos < strSize) {
               if(str.at(pos) == ';') {
                  arr[arrPos + 1] = NULL;
                  execvp(arr[0], arr);
               }
               else {
                   sarg += str.at(pos);
                   ++pos;
               }
            }
            strcpy(arg, sarg.c_str());
            arr[arrPos] = arg;
            arr[arrPos + 1] = NULL;

            if(execvp(arr[0], arr) == -1) {
                perror("The command could not be executed!");
                errno = 0;
            }        
            free(comm);
            free(arg);
        }
    }

return 0;
}
