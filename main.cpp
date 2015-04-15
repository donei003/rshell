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
    int pos;
    while(1) {
        getline(cin, str);
        pos = 0;
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
            while(str.at(pos) != ' ') {

            }
            execvp();
        }
    }

return 0;
}
