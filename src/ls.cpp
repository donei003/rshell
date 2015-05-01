#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <vector>
#include <errno.h>
#include <string>
#include <list>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <queue>
#include <stdlib.h>
using namespace std;

struct fileEntry {
    string perm;
    int nlink;
    string user;
    string group;
    int size;
    string month;
    string day;
    string time;
    string name;
};

bool noArg = false, flagA = false, flagL = false, flagR = false;
bool isDir = false;
bool hasPath = false; // This means that the user included a path when executing ls


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
           if(temp_cwd == NULL) {
               perror("Could not obtain a proper path");
               errno = 0;
           }
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
           free(temp_cwd);
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
           if(temp_cwd == NULL) {
               perror("The file/directory does not exist or could not be obtained");
               errno = 0;
           }
           string relPath = argv[i];
           string fullPath = temp_cwd;
           //cout << fullPath << endl;
           fullPath += ("/" + relPath);
           cout << fullPath << endl;
           paths.push_back(fullPath);
           free(buf);
           free(temp_cwd);
       }
   }
   if(!(hasPath)) {
       int size = PATH_MAX;
       char *buf = NULL;
       char *temp_cwd = getcwd(buf, (size_t) size);
       if(temp_cwd == NULL) {
               perror("Could not access current directory");
               errno = 0;
       }
       string cwd = temp_cwd;
       paths.push_back(cwd);
       free(buf);
       free(temp_cwd);
   }
   return paths;
}


void getCwdFilesRec(const char* pathname) {
    string spathname(pathname);
    if(spathname.at(spathname.size()-1) == '/' && spathname.size() != 1) {
        spathname.resize(spathname.size()-1);
    }
    struct dirent *cDirent; 
    DIR *cDir;
    list <string> FilesInDir;
    list <string> SubDirs;
    list <fileEntry> lf;
    unsigned int largestSize = 0, largestLink = 0,largestDay = 0, totalSize = 0;
    unsigned int largestWordSize = 0, largestUser = 0, largestGroup = 0;

    cDir = opendir(spathname.c_str());
    if(cDir == NULL) {
        perror("Cannot open the directory specified");
        errno = 0;
        return;
    }
    
    else {
        errno = 0;
        while((cDirent = readdir(cDir)) != NULL || errno != 0) {
            if(cDirent == NULL && errno != 0) {
                perror("An error occurred while reading a file/folder");
                errno = 0;
                continue;
            }
            string entryName = cDirent->d_name;
            FilesInDir.push_back(entryName);
        }


        if(closedir(cDir) == -1) {
            perror("Could not close directory");
            errno = 0;
            exit(0);
        }
        FilesInDir.sort();
    }

    bool hasSubDir = false;

    for(std::list<string>::iterator i = FilesInDir.begin(); i != FilesInDir.end(); ++i) {
        struct stat s;
        struct fileEntry f;
        if((*i).at(0) == '.' && !(flagA)) {
            continue;
        }
        string absPath = spathname + "/" + (*i);

        if(stat((absPath).c_str(),&s) == -1) {
            perror("stat error");
            errno = 0;
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
        for(int j = s.st_nlink; j > 0; j /= 10) {
            numDigits++;
        }
        if(numDigits > largestLink) {
            largestLink = numDigits;
        }
        numDigits = 0;

        errno = 0;
        passwd *usrid = getpwuid(s.st_uid);
        if(errno != 0) {
            perror("Cannot retrieve user information");
            errno = 0;
        }
        else {
            f.user = usrid->pw_name;
            if(f.user.size() > largestUser) {
                largestUser = f.user.size();
            }
        }
        group *grpid = getgrgid(s.st_gid);
        if(errno != 0) {
            perror("Cannot retrieve group information");
            errno = 0;
        }
        else {
            f.group = grpid->gr_name;
            if(f.group.size() > largestGroup) {
                largestGroup = f.group.size();
            }
        }


        f.size = s.st_size;
        totalSize += s.st_blocks;
        //totalSize += s.st_size;
        for(int l = s.st_size; l > 0; l /= 10) {
            numDigits++;
        }
        if(numDigits > largestSize) {
            largestSize = numDigits;
        }
        numDigits = 0;
        //cout << usrid->pw_name << ' ' << grpid->gr_name << ' ';
        //cout << s.st_size << ' ';

        char date[80];
        struct tm *timing = localtime(&s.st_mtime);
        
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
        if((*i).size() > largestWordSize) {
            largestWordSize = (*i).size();
        }
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
    for(std::list<fileEntry>::iterator i = lf.begin(); i != lf.end(); ++i) {
        if(flagL) {
            cout << (*i).perm << ' ' << setw(largestLink)
            << right << (*i).nlink << ' ' << setw(largestUser) << right << (*i).user
            << ' ' << setw(largestGroup) << right << (*i).group << ' ' << setw(largestSize)
            << right << (*i).size << ' ' << (*i).month 
            << ' ' << setw(largestDay) << right << (*i).day 
            << ' ' << (*i).time << ' ' << (*i).name << endl;;
        }
        else {
            width += largestWordSize;
            if(width <= 80) {
                cout << setw(largestWordSize) << left << (*i).name << " ";
                width += 1;
            }
            else {
                cout << endl << setw(largestWordSize) << left << (*i).name;
                width = largestWordSize + 1;
            }
        }
    }
    if(flagR && hasSubDir) {
        //cout << endl;
        SubDirs.sort();
        for(std::list<string>::iterator i = SubDirs.begin(); i != SubDirs.end(); ++i) {
            if((*i) == "." || (*i) == "..") {
                continue;
            }
            cout << endl;
            string temp_path;
            if(spathname.at(spathname.size()-1) == '/') {
                temp_path = (spathname + (*i));
            }
            else {
                temp_path = (spathname + "/" + (*i));
            }
            getCwdFilesRec(temp_path.c_str());
        }
    }
    else if(!(hasPath) || (flagA && !(flagL))) {
        cout << endl;
    }
    return;

}

int main(int argc, char* argv[]) {
    list <string> IncPaths;
    if(argc == 1) {
        noArg = true;

    }
    findArg(argc, argv);
    IncPaths = getPaths(argc,argv);
    for(std::list<string>::iterator i = IncPaths.begin(); i != IncPaths.end(); ++i) {
        //cout << *i << endl;
        getCwdFilesRec((*i).c_str());
    }

    return 0;
}

