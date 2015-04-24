#include <iostream>
using namespace std;

bool noArg = false, flagA = false, flagL = false, flagR = false;
int argn = 0;
char* arga[];
void findArg() {
    if(!(noArg)) {
        int ArrInd = 1, ArrPos = 0;
        while(ArrInd < argc && *(argv[ArrInd]) == '-') {
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
    }
}

            


int main(int argc, char* argv[]) {
    argn = argc;
    arga[] = argv[];
    
    if(argc == 1) {
        noArg = true;
    }

    /*int n = 0;
    cout << argc << endl;
    while(*(argv[2]+n) != '\0') {
        if(*(argv[2] + n) == ' ') {
            cout << "-";
        }
        else {
            cout << *(argv[2] + n);
        }
        ++n; 
    }
    cout << endl;*/

    
    findArg();

    if(flagA) {cout << "1" << endl;}
    if(flagL) {cout << "2" << endl;}
    if(flagR) {cout << "3" << endl;}


    return 0;
}
