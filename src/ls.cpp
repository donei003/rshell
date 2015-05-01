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

list <string> getPaths(int argc, char *argv[]) {
   list <string> paths;
   bool hasPath = false; // This means that the user included a path when executing ls
   
   for(int i = 1; i < argc; ++i) {
       if(*(argv[i]) == '-') {
           continue;
       }
       else if(*(argv[i]) == '~') {
           int size = PATH_MAX;
           int FWSLeft = 4;
           char *buf = NULL;
           string absPath;
           char *temp_cwd = getcwd(buf, (size_t) size);
           hasPath = true; // Will likely need to move this 
           for(int j = 0; *(argv[i] + j) != '\0'; ++j) {
                if(*(argv[i]+j) == '/') {
                    --FWSLeft;
                    if(FWSLeft == 0) {
                        break;
                    }
                    absPath += *(argv[i] + j);
                }
                else {
                    absPath += *(argv[i] + j);
                }
           }
           cout << absPath << endl;
           paths.push_back(absPath);
           free(buf);
       }
       else if(*(argv[i]) == '/') {
            hasPath = true;
            string temp_path = argv[i];
            paths.push_back(temp_path);
       }
       else {
           hasPath = true;
           int size = PATH_MAX;
           char *buf = NULL;
           char *temp_cwd = getcwd(buf, (size_t) size);
           string relPath = argv[i];
           string fullPath = temp_cwd;
           //cout << fullPath << endl;
           fullPath += ("/" + relPath);
           cout << fullPath << endl;
           paths.push_back(fullPath);
       }
   }
   if(!(hasPath)) {
       int size = PATH_MAX;
       char *buf = NULL;
       char *temp_cwd = getcwd(buf, (size_t) size);
       string cwd = temp_cwd;
       paths.push_back(cwd);
   }
   return paths;
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
            struct fileEntry f;
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
    int largestSize = 0, largestLink = 0,largestDay = 0, totalSize = 0;

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

        errno = 0;
        passwd *usrid = getpwuid(s.st_uid);
        if(usrid == NULL) {
            perror("Cannot retrieve user information");
            errno = 0;
        }
        else {
            f.user = usrid->pw_name;
        }
        group *grpid = getgrgid(s.st_gid);
        if(grpid == NULL) {
            perror("Cannot retrieve group information");
            errno = 0;
        }
        else {
            f.group = grpid->gr_name;
        }


        f.size = s.st_size;
        totalSize += s.st_blocks;
        //totalSize += s.st_size;
        for(int i = s.st_size; i > 0; i /= 10) {
            numDigits++;
        }
        if(numDigits > largestSize) {
            largestSize = numDigits;
        }
        numDigits = 0;
        //cout << usrid->pw_name << ' ' << grpid->gr_name << ' ';
        //cout << s.st_size << ' ';

        char date[80];
        struct tm* timing = localtime(&s.st_mtime);
        
        if(timing == NULL) {
            perror("Could not retrieve time information");
            errno = 0;
        }

        strftime(date,80,"%b",timing);
        //cout << date << ' ';
        f.month = date;
        strftime(date,80,"%-d",timing);
        //cout << date << ' ';
        f.day = date;
        for(int k = timing->tm_mday; k > 0; k /= 10) {
            ++numDigits;
        }
        if(numDigits > largestDay) {
            largestDay = numDigits;
        }
        strftime(date,80,"%R",timing);
        //cout << date << ' ';
        f.time = date;
        f.name = *i;
        //cout << "  " << *i << endl;
        lf.push_back(f);
        //free(f);
    }
    int width = 0;
    if(flagR) {
        cout << spathname << ": " << endl;
    }
    if(flagL) {
        cout << "Total " << totalSize/2 << endl;
    }
    for(auto i = lf.begin(); i != lf.end(); ++i) {
        if(flagL) {
            cout << (*i).perm << ' ' << setw(largestLink)
            << right << (*i).nlink << ' ' << (*i).user
            << ' ' << (*i).group << ' ' << setw(largestSize)
            << right << (*i).size << ' ' << (*i).month 
            << ' ' << setw(largestDay) << right << (*i).day 
            << ' ' << (*i).time << ' ' << (*i).name << endl;;
        }
        else {
            width += (*i).name.size();
            if(width < 80) {
                cout << (*i).name << "  ";
                width += 2;
            }
            else {
                cout << endl << (*i).name << "  ";
                width = (*i).name.size() + 2;
            }
        }
    }
    if(flagR) {
        //cout << endl;
        SubDirs.sort();
        for(auto i = SubDirs.begin(); i != SubDirs.end(); ++i) {
            if((*i) == "." || (*i) == "..") {
                continue;
            }
            cout << endl;

            string temp_path = (spathname + "/" + (*i));
            getCwdFilesRec(temp_path.c_str());
        }
    }
    return;

}

int main(int argc, char* argv[]) {
    struct stat s;
    list <string> IncPaths;
    if(argc == 1) {
        noArg = true;

    }
    findArg(argc, argv);
    IncPaths = getPaths(argc,argv);
    for(auto i = IncPaths.begin(); i != IncPaths.end(); ++i) {
        //cout << *i << endl;
        getCwdFilesRec((*i).c_str());
    }

    return 0;
}

