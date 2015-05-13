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
#include <fcntl.h>
#include <sys/types.h>
#include <vector>


using namespace std;

int skipWhiteSpace(int pos, int strSize,string str) {
    while(pos < strSize && str.at(pos) == ' ') {
        ++pos;
    }
    return pos;
}

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
            bool bPipe = false, iRedir = false, oRedir = false, oRedir2 = false;
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
                else if(str.at(pos) == '<') {
                    iRedir = true;
                    //++pos;
                    break;
                }
                else if(str.at(pos) == '>') {
                    //++pos;
                    if(pos+1 < strSize && str.at(pos+1) == '>') {
                        oRedir2 = true;
                        //++pos;
                    }
                    else {
                        oRedir = true;
                    }
                    break;
                }
                else if(str.at(pos) == '|') {
                    ++pos;
                    if(pos < strSize && str.at(pos) == '|') {
                        logOR = true;
                        ++pos;
                        break;
                    }
                    else {
                        bPipe = true;
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
            //cout << pos << endl;

            if(comment) { // Disregard everything following the '#' character
                break;
            }

            strcpy(comm,scomm.c_str()); // Copies the string into a char pointer
            //++pos; // This is to ommit the whitespace character following the command

            if(pos < strSize && str.at(pos) == ' ' && !(term) && !(oRedir || oRedir2 || iRedir || bPipe)) { // Skips whitespace
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
               else if(str.at(pos) == ' ' && oRedir) {
                   ++pos;
                   continue;
               }
               else if(str.at(pos) == '-' && str.at(pos+1) == ' ') {
                   sarg += str.at(pos);
                   ++pos;
                   while(str.at(pos) == ' ') {
                       ++pos;
                   }
               }
               else if(str.at(pos) == '<') {
                    iRedir = true;
                    //++pos;
                    break;
                }
                else if(str.at(pos) == '>') {
                    //++pos;
                    if(pos+1 < strSize && str.at(pos + 1) == '>') {
                        oRedir2 = true;
                        //++pos;
                    }
                    else {
                        oRedir = true;
                    }
                    break;
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
                    else {
                        bPipe = true;
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
            }// Add a loop to catch the name of the file for io redir

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
            //cout << sarg << endl;
            arr[arrPos] = NULL;
            
            /*for(int i = 0; i < 1; ++i) {
                cout << arr[i] << endl;
            }*/

            /*if((oRedir || oRedir2 || iRedir) && pos < strSize && str.at(pos) == ' ' && !(term)) { // Skips whitespace
                while(pos < strSize && str.at(pos) == ' ') {
                    ++pos;
                }
            }*/
            vector <string> infiles;
            vector <string> outfiles;
            vector <string> outfilesApp;
            //cout << pos << endl;
            //cout << strSize << endl;
            while((oRedir || oRedir2 || iRedir) && pos < strSize) { // Will have to add stopping for connectors and such
                string file;
                if(str.at(pos) == ' ') {
                    ++pos; 
                    continue;
                }
                else if(str.at(pos) == ';') {
                    ++pos;
                    break;
                }
                else if(str.at(pos) == '>') {
                    if(pos+1 < strSize && str.at(pos+1) == '>') {
                        pos += 2;
                        pos = skipWhiteSpace(pos,strSize,str); 
                        while(pos < strSize) {
                            if(str.at(pos) == ';' || str.at(pos) == ' ') {
                                break;
                            }
                            else {
                                file += str.at(pos);
                            }
                            ++pos;
                        }
                        oRedir2 = true;
                        outfilesApp.push_back(file);
                    }
                    else {
                        ++pos;
                        pos = skipWhiteSpace(pos,strSize,str);
                        while(pos < strSize) {
                            if(str.at(pos) == ';' || str.at(pos) == ' ') {
                                break;
                            }
                            else {
                                file += str.at(pos);
                            }
                            ++pos;
                        }
                        oRedir = true;
                        outfiles.push_back(file);
                    }
                }
                else if(str.at(pos) == '<') {
                    ++pos;
                    pos = skipWhiteSpace(pos,strSize,str);
                    while(pos < strSize) {
                        if(str.at(pos) == ';' || str.at(pos) == ' ') {
                            break;
                        }
                        else {
                            file += str.at(pos);
                        }
                        ++pos;
                    }
                    iRedir = true;
                    infiles.push_back(file);
                }
                //cout << pos << endl;
            }

            /*for(unsigned int e = 0; e < outfiles.size(); ++e) {
                cout << outfiles.at(e) << endl;
            }
            for(unsigned int r = 0; r < infiles.size(); ++r) {
                cout << infiles.at(r) << endl;
            }*/
            int ifd, ofd;
            int x = 0;
            if(scomm != "exit") {
                pid = fork(); // Creating child process

                //int x = 0;
                if(pid == 0) { // Child process
                    if((!(lastOR) && !(lastAND)) || (lastSuccess == true && lastOR == true) || 
                        (lastSuccess == true && lastAND == true)) {
                        if(bPipe) {}
                        if(oRedir2) {
                            ofd = dup(1);         
                            if(close(1) == -1) {
                                perror("close: ");
                            }
                            for(unsigned int o = 0; o < outfilesApp.size(); ++o) {
                                if(open(outfilesApp.at(o).c_str(), O_CREAT | O_RDWR | O_APPEND, S_IWUSR | S_IRUSR) == -1) {
                                    perror("open: ");
                                }
                            }
                        }
                        if(oRedir) {
                            ofd = dup(1);         
                            if(close(1) == -1) {
                                perror("close: ");
                            }
                            for(unsigned int p = 0; p < outfiles.size(); ++p) {
                                if(open(outfiles.at(p).c_str(), O_CREAT | O_RDWR | O_TRUNC, S_IWUSR | S_IRUSR) == -1) {
                                    perror("open: ");
                                }
                            }
                        }
                        if(iRedir) {
                            ifd = dup(0);
                            if(close(0) == -1) {
                                perror("close: ");
                            }
                            for(unsigned int q = 0; q < infiles.size(); ++q) {
                                if(open(infiles.at(q).c_str(), O_RDONLY) == -1) {
                                    perror("open: ");
                                }
                            }
                        }

                        x = execvp(arr[0], arr); // Command execution
                        if(x == -1) {
                            perror("The command could not be executed!");
                        }
                        if(oRedir || oRedir2) {
                            dup2(ofd, 1);
                        }
                        if(iRedir) {
                            dup2(ifd, 0);
                        }
                        errno = 0;
                        _exit(0);
                    }
                }

                else { // Parent process
                    if(pid == -1) {
                        perror("fork: ");
                    }
                    if(wait(0) == -1) {
                        perror("wait: ");
                    }
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
