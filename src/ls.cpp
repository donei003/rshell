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
#include <pwd.h>
#include <grp.h>
using namespace std;

bool noArg = false, flagA = false, flagL = false, flagR = false;
list<string> vs;

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

void getCwdFiles() {
    int size = PATH_MAX;
    struct dirent *cDirent;
    DIR *cDir;
    char* buf = NULL;
    char* pathname = getcwd(buf, (size_t)size);
    cout << pathname << endl;
    cDir = opendir(pathname);
    if(cDir == NULL) {
        perror("failed");
        errno = 0;
    }
    
    else {
        while((cDirent = readdir(cDir)) != NULL) {
            string s;
            s = cDirent->d_name;
            vs.push_back(s);
        }

        closedir(cDir);
        vs.sort();
    }
    free(pathname);
    free(buf);
    free(cDirent);

    if(noArg || (flagA && !(flagL) && !(flagR))) {
        int n = 0;
        for(auto i = vs.begin(); i != vs.end(); ++i) {
            if((*i).at(0) == '.' && noArg) {
                continue;
            }
            else if(n == 5) {
                cout << *i << endl;
                n = 0;
            }
            else {
                cout << *i << ' ';
                ++n;
            }
        }
        cout << endl;
    }
    else if(flagL) {
        struct stat s;
        for(auto i = vs.begin(); i != vs.end(); ++i) {
            if((*i).at(0) == '.' && !(flagA)) {
                continue;
            }
            if(stat((*i).c_str(),&s) == -1) {
                perror("stat error");
                break; // This may need to be changed, not sure if continue or not
            }

            if((s.st_mode & S_IFMT) == S_IFDIR) {
                cout << 'd';
            }
            else if((s.st_mode & S_IFMT) == S_IFREG) {
                cout << '-';
            }
            else if((s.st_mode & S_IFMT) == S_IFLNK) {
                cout << 'l';
            }
            else if((s.st_mode & S_IFMT) == S_IFCHR) {
                cout << 'c';
            }
            else if((s.st_mode & S_IFMT) == S_IFBLK) {
                cout << 'b';
            }

            (s.st_mode & S_IRUSR) ? (cout << 'r') : (cout << '-');
            (s.st_mode & S_IWUSR) ? (cout << 'w') : (cout << '-');
            (s.st_mode & S_IXUSR) ? (cout << 'x') : (cout << '-');
            (s.st_mode & S_IRGRP) ? (cout << 'r') : (cout << '-');
            (s.st_mode & S_IWGRP) ? (cout << 'w') : (cout << '-');
            (s.st_mode & S_IXGRP) ? (cout << 'x') : (cout << '-');
            (s.st_mode & S_IROTH) ? (cout << 'r') : (cout << '-');
            (s.st_mode & S_IWOTH) ? (cout << 'w') : (cout << '-');
            (s.st_mode & S_IXOTH) ? (cout << 'x') : (cout << '-');
            cout << ' ' << s.st_nlink << ' ';

            passwd *usrid = getpwuid(s.st_uid);
            group *grpid = getgrgid(s.st_gid);

            cout << usrid->pw_name << ' ' << grpid->gr_name << ' ';
            cout << s.st_size << ' ';

            char date[80];
            struct tm* timing = localtime(&s.st_mtime);
            strftime(date,80,"%b",timing);
            cout << date << ' ';
            strftime(date,80,"%-d",timing);
            cout << date << ' ';
            strftime(date,80,"%R",timing);
            cout << date << ' ';

            cout << "  " << *i << endl;
        }
        cout << endl;
    }
}



int main(int argc, char* argv[]) {
    struct stat s;
    if(argc == 1) {
        noArg = true;

    }
    findArg(argc, argv);
    getCwdFiles();
    /*for(auto i = vs.begin(); i != vs.end(); ++i) {
        stat((*i).c_str(),&s);
        //(S_IWUSR & s.st_mode) ? "w" : "-";
        if((S_IWUSR & s.st_mode)) {
            cout << "-";
        }
        else {
            cout << "-";
        }
        cout << *i << endl;
    }*/

    /*struct stat s;
    stat("ls.cpp", &s);*/

    
    //findArg(argc, argv);

    if(flagA) {cout << "1" << endl;}
    if(flagL) {cout << "2" << endl;}
    if(flagR) {cout << "3" << endl;}


    return 0;
}

