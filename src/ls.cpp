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
#include <iomanip>
#include <queue>
using namespace std;

struct fileEntry {
    string perm = "";
    int nlink = 0;
    string user = "";
    string group = "";
    int size = 0;
    string month = "";
    string day = "";
    string time = "";
    string name = "";
};

bool noArg = false, flagA = false, flagL = false, flagR = false;
bool isDir = false;


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

/*void getCwdFiles() {
    int size = PATH_MAX;
    struct dirent *cDirent;
    DIR *cDir = NULL;
    char* buf = NULL;
    list<string> FilesInDir;

    char* pathname = getcwd(buf, (size_t)size);
    //string pathname = getcwd(buf, (size_t)size);

    //cout << pathname << endl;
    cDir = opendir(pathname);
    if(cDir == NULL) {
        perror("failed");
        errno = 0;
    }
    
    else {
        while((cDirent = readdir(cDir)) != NULL) {
            string s;
            s = cDirent->d_name;
            FilesInDir.push_back(s);
        }

        closedir(cDir);
        FilesInDir.sort();
    }
    free(pathname);
    free(buf);
    free(cDirent);

    if(noArg || (flagA && !(flagL) && !(flagR))) {
        int n = 0;
        for(auto i = FilesInDir.begin(); i != FilesInDir.end(); ++i) {
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
        list <fileEntry> lf;
        int largestSize = 0, largestLink = 0, totalSize = 0;
        for(auto i = FilesInDir.begin(); i != FilesInDir.end(); ++i) {
            struct fileEntry f = {0};
            if((*i).at(0) == '.' && !(flagA)) {
                continue;
            }
            if(stat((*i).c_str(),&s) == -1) {
                perror("stat error");
                break; // This may need to be changed, not sure if continue or not
            }

            if((s.st_mode & S_IFMT) == S_IFDIR) {
                //cout << 'd';
                f.perm += 'd';
                isDir = true;
            }
            else if((s.st_mode & S_IFMT) == S_IFREG) {
                //cout << '-';
                f.perm += '-';
            }
            else if((s.st_mode & S_IFMT) == S_IFLNK) {
                //cout << 'l';
                f.perm += 'l';
            }
            else if((s.st_mode & S_IFMT) == S_IFCHR) {
                //cout << 'c';
                f.perm += 'c';
            }
            else if((s.st_mode & S_IFMT) == S_IFBLK) {
                //cout << 'b';
                f.perm += 'b';
            }

            (s.st_mode & S_IRUSR) ? (f.perm += 'r') : (f.perm += '-');
            (s.st_mode & S_IWUSR) ? (f.perm += 'w') : (f.perm += '-');
            (s.st_mode & S_IXUSR) ? (f.perm += 'x') : (f.perm += '-');
            (s.st_mode & S_IRGRP) ? (f.perm += 'r') : (f.perm += '-');
            (s.st_mode & S_IWGRP) ? (f.perm += 'w') : (f.perm += '-');
            (s.st_mode & S_IXGRP) ? (f.perm += 'x') : (f.perm += '-');
            (s.st_mode & S_IROTH) ? (f.perm += 'r') : (f.perm += '-');
            (s.st_mode & S_IWOTH) ? (f.perm += 'w') : (f.perm += '-');
            (s.st_mode & S_IXOTH) ? (f.perm += 'x') : (f.perm += '-');
            //cout << ' ' << s.st_nlink << ' ';
            f.nlink = s.st_nlink;
            
            unsigned int numDigits = 0;
            for(int i = s.st_nlink; i > 0; i /= 10) {
                numDigits++;
            }
            if(numDigits > largestLink) {
                largestLink = numDigits;
            }
            numDigits = 0;


            passwd *usrid = getpwuid(s.st_uid);
            group *grpid = getgrgid(s.st_gid);
            f.user = usrid->pw_name;
            f.group = grpid->gr_name;
            f.size = s.st_size;
            totalSize += s.st_blocks;
            //totalSize += s.st_size;
            for(int i = s.st_size; i > 0; i /= 10) {
                numDigits++;
            }
            if(numDigits > largestSize) {
                largestSize = numDigits;
            }
            //cout << usrid->pw_name << ' ' << grpid->gr_name << ' ';
            //cout << s.st_size << ' ';

            char date[80];
            struct tm* timing = localtime(&s.st_mtime);
            strftime(date,80,"%b",timing);
            //cout << date << ' ';
            f.month = date;
            strftime(date,80,"%-d",timing);
            //cout << date << ' ';
            f.day = date;
            strftime(date,80,"%R",timing);
            //cout << date << ' ';
            f.time = date;
            f.name = *i;
            //cout << "  " << *i << endl;
            lf.push_back(f);
            isDir = false;
        }
        cout << "Total " << totalSize/2 << endl;
        for(auto i = lf.begin(); i != lf.end(); ++i) {
            cout << (*i).perm << ' ' << setw(largestLink)
            << right << (*i).nlink << ' ' << (*i).user
            << ' ' << (*i).group << ' ' << setw(largestSize)
            << right << (*i).size << ' ' << (*i).month 
            << ' ' << (*i).day << ' ' << (*i).time
            << ' ' << (*i).name << endl;
        }
    }
}*/

void getCwdFilesRec(const char* pathname) {
    string spathname = pathname;
    struct dirent *cDirent; 
    DIR *cDir;
    bool hasSubDir = false;
    list <string> FilesInDir;
    list <string> SubDirs;
    list <fileEntry> lf;
    int largestSize = 0, largestLink = 0, totalSize = 0;

    cDir = opendir(spathname.c_str());
    if(cDir == NULL) {
        perror("Cannot open the directory specified");
        errno = 0;
        exit(0);
    }
    
    else {
        while((cDirent = readdir(cDir)) != NULL) {
            string entryName = cDirent->d_name;
            FilesInDir.push_back(entryName);
        }

        closedir(cDir);
        FilesInDir.sort();
    }

    /*for(auto i = FilesInDir.begin(); i != FilesInDir.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl << spathname << endl;*/
    //delete(cDirent);
    //delete(cDir);
    

    for(auto i = FilesInDir.begin(); i != FilesInDir.end(); ++i) {
        struct stat s;
        struct fileEntry f;
        //f = (fileEntry*) malloc(sizeof(fileEntry));
        if((*i).at(0) == '.' && !(flagA)) {
            continue;
        }
        string absPath = spathname + "/" + (*i);

        if(stat((absPath).c_str(),&s) == -1) {
            perror("stat error");
            break; // This may need to be changed, not sure if continue or not
        }

        if((s.st_mode & S_IFMT) == S_IFDIR) {
            //cout << 'd';
            f.perm += 'd';
            hasSubDir = true;
            SubDirs.push_back(*i);
        }
        else if((s.st_mode & S_IFMT) == S_IFREG) {
            //cout << '-';
            f.perm += '-';
        }
        else if((s.st_mode & S_IFMT) == S_IFLNK) {
            //cout << 'l';
            f.perm += 'l';
        }
        else if((s.st_mode & S_IFMT) == S_IFCHR) {
            //cout << 'c';
            f.perm += 'c';
        }
        else if((s.st_mode & S_IFMT) == S_IFBLK) {
            //cout << 'b';
            f.perm += 'b';
        }

        (s.st_mode & S_IRUSR) ? (f.perm += 'r') : (f.perm += '-');
        (s.st_mode & S_IWUSR) ? (f.perm += 'w') : (f.perm += '-');
        (s.st_mode & S_IXUSR) ? (f.perm += 'x') : (f.perm += '-');
        (s.st_mode & S_IRGRP) ? (f.perm += 'r') : (f.perm += '-');
        (s.st_mode & S_IWGRP) ? (f.perm += 'w') : (f.perm += '-');
        (s.st_mode & S_IXGRP) ? (f.perm += 'x') : (f.perm += '-');
        (s.st_mode & S_IROTH) ? (f.perm += 'r') : (f.perm += '-');
        (s.st_mode & S_IWOTH) ? (f.perm += 'w') : (f.perm += '-');
        (s.st_mode & S_IXOTH) ? (f.perm += 'x') : (f.perm += '-');
        //cout << ' ' << s.st_nlink << ' ';
        f.nlink = s.st_nlink;
        
        unsigned int numDigits = 0;
        for(int i = s.st_nlink; i > 0; i /= 10) {
            numDigits++;
        }
        if(numDigits > largestLink) {
            largestLink = numDigits;
        }
        numDigits = 0;


        passwd *usrid = getpwuid(s.st_uid);
        group *grpid = getgrgid(s.st_gid);
        f.user = usrid->pw_name;
        f.group = grpid->gr_name;


        f.size = s.st_size;
        totalSize += s.st_blocks;
        //totalSize += s.st_size;
        for(int i = s.st_size; i > 0; i /= 10) {
            numDigits++;
        }
        if(numDigits > largestSize) {
            largestSize = numDigits;
        }
        //cout << usrid->pw_name << ' ' << grpid->gr_name << ' ';
        //cout << s.st_size << ' ';

        char date[80];
        struct tm* timing = localtime(&s.st_mtime);
        strftime(date,80,"%b",timing);
        //cout << date << ' ';
        f.month = date;
        strftime(date,80,"%-d",timing);
        //cout << date << ' ';
        f.day = date;
        strftime(date,80,"%R",timing);
        //cout << date << ' ';
        f.time = date;
        f.name = *i;
        //cout << "  " << *i << endl;
        lf.push_back(f);
        //free(f);
    }
    cout << "Total " << totalSize/2 << endl;
    for(auto i = lf.begin(); i != lf.end(); ++i) {
        cout << (*i).perm << ' ' << setw(largestLink)
        << right << (*i).nlink << ' ' << (*i).user
        << ' ' << (*i).group << ' ' << setw(largestSize)
        << right << (*i).size << ' ' << (*i).month 
        << ' ' << (*i).day << ' ' << (*i).time
        << ' ' << (*i).name << endl;
    }
    SubDirs.sort();
    for(auto i = SubDirs.begin(); i != SubDirs.end(); ++i) {
        if((*i) == "." || (*i) == "..") {
            continue;
        }
        string temp_path = (spathname + "/" + (*i));
        getCwdFilesRec(temp_path.c_str());
    }
    return;

}

int main(int argc, char* argv[]) {
    struct stat s;
    if(argc == 1) {
        noArg = true;

    }
    findArg(argc, argv);
    //getCwdFiles();
    getCwdFilesRec(argv[2]);


    return 0;
}

