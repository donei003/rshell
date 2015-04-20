#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <string>
using namespace std;

bool isDependant = 0;
int main() {
    string str;
    pid_t pid;
    bool hasArg = 0, term = 0;
    int strSize = 0;
    int pos, arrPos;
    bool bexit = false;

    char *arg;
    char *comm;
    while(1) {
        cout << "$ ";
        getline(cin, str);
        string sarg, scomm;
        bool success = false;
        bool lastOR = false, lastAND = false;
        bool lastSuccess = false;

        pos = 0;
        strSize = str.size();
        //cout << strSize << endl;
        while(pos < strSize) { // Kepp looking until the end of line is reached
            arg = (char*) malloc(256);
            comm = (char*) malloc(256);
            char* arr[30]; 
            string sarg, scomm;

            bool logOR = false, logAND = false;
            bool comment = false;
            arrPos = 0;
            hasArg = 0;
            term = false;
            hasArg = false;

            if(str.at(pos) == ' ') { // If there is a white space before command skip
                while(pos < strSize && str.at(pos) == ' ') {
                    ++pos;
                }
                if(pos >= strSize) { // Break out if end is reached
                    break;
                }
            }

            while(pos < strSize && str.at(pos) != ' ') { // Getting our command into a string
                if(str.at(pos) == '#') { // So we don't skip our comment
                    comment = true;
                    break;
                }

                else if(str.at(pos) == ';') { // Looking for connectors adjacent to the command
                    term = true;
                    ++pos;
                    break;
                }
                else if(str.at(pos) == '|') {
                    ++pos;
                    if(pos < strSize && str.at(pos) == '|') {
                        logOR = true;
                        ++pos;
                        break;
                    }
                }
                else if(str.at(pos) == '&') {
                    ++pos;
                    if(pos < strSize && str.at(pos) == '&') {
                        logAND = true;
                        ++pos;
                        break;
                    }
                }
                else {
                    scomm += str.at(pos); //Pushing onto the string
                    ++pos;
                }
            }

            if(comment) { // Disregard everything following the '#' character
                break;
            }

            strcpy(comm,scomm.c_str()); // Copies the string into a char pointer
            //++pos; // This is to ommit the whitespace character following the command

            if(pos < strSize && str.at(pos) == ' ' && !(term)) { // Skips whitespace
                while(pos < strSize && str.at(pos) == ' ') {
                    ++pos;
                }
                if(pos >= strSize) {
                    break;
                }
            }

            arr[arrPos] = comm;
            ++arrPos;
            while(pos < strSize && !(term) && !(logOR) && !(logAND)) { // Looking for our arguments
               if(str.at(pos) == '#') { // This is the same process as for the command
                   break;
               }
               else if(str.at(pos) == '-' && str.at(pos+1) == ' ') {
                   sarg += str.at(pos);
                   ++pos;
                   while(str.at(pos) == ' ') {
                       ++pos;
                   }
               }
               else if(str.at(pos) == ';') {
                   ++pos;
                   break;
               }
               else if(str.at(pos) == '|') {
                    ++pos;
                    if(pos < strSize && str.at(pos) == '|') {
                        logOR = true;
                        ++pos;
                        break;
                    }
                }
                else if(str.at(pos) == '&') {
                    ++pos;
                    if(pos < strSize && str.at(pos) == '&') {
                        logAND = true;
                        ++pos;
                        break;
                    }
                }
               else {
                   sarg += str.at(pos);
                   ++pos;
                   hasArg = true;
               }
            }

            if(hasArg) {
                unsigned int n = 0;
                string s;
                while(n < sarg.size()) { // Seperating each command to be into their own char pointer
                    if(sarg.at(n) == '-' && n != 0) {
                        strcpy(arg, s.c_str());
                        arr[arrPos] = arg;
                        ++arrPos;
                        s = "";
                    }
                    s += sarg.at(n);
                    ++n;
                }
                n = s.size() -1;
                while(s.at(n) == ' ') { // Omitting whitespace characters in argument
                    s.at(n) = '\0';
                    --n;
                }
                strcpy(arg, s.c_str());
                arr[arrPos] = arg;
                ++arrPos;
            }

            arr[arrPos] = NULL;
            
            /*for(int i = 0; i < 1; ++i) {
                cout << arr[i] << endl;
            }*/

            
            int x = 0;
            if(scomm != "exit") {
                pid = fork(); // Creating child process

                //int x = 0;
                if(pid == 0) { // Child process
                    if((!(lastOR) && !(lastAND)) || (lastSuccess == true && lastOR == true) || 
                        (lastSuccess == true && lastAND == true)) {
                        x = execvp(arr[0], arr); // Command execution
                        perror("The command could not be executed!");
                        errno = 0;
                        _exit(0);
                    }
                }

                else { // Parent process
                    wait(0);
                    if(x == 0) { // Handling the cases of connectors with commands failing/succeeding
                        if(logOR == true) {
                            success = true;
                        }
                        else {
                            success = false;
                        }
                    }
                    else {
                        if(logOR) {
                            success = false;
                        }
                        else {
                            success = true;
                        }
                    }
                    /*else if (x == 0 && logOR) {
                        cout << x << endl;
                        success = true;
                    }*/
                }
            }

            else { // If the user typed in "exit"
                if((!lastOR && !lastAND) || (!lastSuccess && lastOR) || (lastSuccess && lastAND)) {
                    cout << "Good-bye!" << endl;
                    exit(0);
                }
            }


            lastOR = logOR;
            lastAND = logAND;
            if(success == true) {
                lastSuccess = false;
            }
            else {
                lastSuccess = true;
            }

            free(comm); // Freeing up our char pointers for the next instruction
            free(arg);
        }
        if(bexit) {
            break;
        }
    }

    return 0;
}
