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

struct fileEntry { // Struct used for -l but also for the other flags
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

/* 
 * This function finds all flags and omits incorrect ones 
 * Global boolean variable are set if a proper flag is detected
 */  
void findArg(int argc, char* argv[]) {
    bool wrongArg = false;
    if(!(noArg)) {
        int ArrInd = 1, ArrPos = 0;
        while(ArrInd < argc) {
            if(*(argv[ArrInd]) == '-') {
                ArrPos = 1;
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
                    else {
                        wrongArg = true;
                    }
                    ++ArrPos;
                }
                ++ArrInd;
            }

            else {
                ++ArrInd;
            }
        }
        if(wrongArg) {
            cerr << "You entered am incorrect flag, it will be omitted" << endl;
        }
    }
}
// Takes in argv and argc and returns a list of paths to be printed
// This will only return the absolute paths regardless of input
// I figured it would be easier for me and the user to has abs paths
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
           hasPath = true; 
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


void getCwdFilesRec(char* pathname) { // The one size fits all ls function, works for all flags
    string spathname(pathname);
    if(spathname.at(spathname.size()-1) == '/' && spathname.size() != 1) {
        spathname.resize(spathname.size()-1);
    }
    struct dirent *cDirent; 
    DIR *cDir;
    list <string> FilesInDir; // list of file names of current dir
    list <string> SubDirs; // List of SubDirectories in current dir
    list <fileEntry> lf; // List of all files and descriptions of current dir
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
        int fd = stat((absPath).c_str(),&s);
        if(fd == -1) {
            perror("stat error");
            errno = 0;
            continue; 
        }

        if((s.st_mode & S_IFMT) == S_IFDIR) {
            f.perm = 'd';
            hasSubDir = true;
            SubDirs.push_back(*i);
        }
        else if((s.st_mode & S_IFMT) == S_IFREG) {
            f.perm = '-';
        }
        else if((s.st_mode & S_IFMT) == S_IFLNK) {
            f.perm = 'l';
        }
        else if((s.st_mode & S_IFMT) == S_IFCHR) {
            f.perm = 'c';
        }
        else if((s.st_mode & S_IFMT) == S_IFBLK) {
            f.perm = 'b';
        }

        // Adding the perms of the current entry to a string, which is then printed
        // out to stdout
        (s.st_mode & S_IRUSR) ? (f.perm += 'r') : (f.perm += '-');
        (s.st_mode & S_IWUSR) ? (f.perm += 'w') : (f.perm += '-');
        (s.st_mode & S_IXUSR) ? (f.perm += 'x') : (f.perm += '-');
        (s.st_mode & S_IRGRP) ? (f.perm += 'r') : (f.perm += '-');
        (s.st_mode & S_IWGRP) ? (f.perm += 'w') : (f.perm += '-');
        (s.st_mode & S_IXGRP) ? (f.perm += 'x') : (f.perm += '-');
        (s.st_mode & S_IROTH) ? (f.perm += 'r') : (f.perm += '-');
        (s.st_mode & S_IWOTH) ? (f.perm += 'w') : (f.perm += '-');
        (s.st_mode & S_IXOTH) ? (f.perm += 'x') : (f.perm += '-');
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
            exit(0);
        }
        else {
            f.user = usrid->pw_name;
            if(f.user.size() > largestUser) {
                largestUser = f.user.size();
            }
        }
        errno = 0;
        group *grpid = getgrgid(s.st_gid);
        
        if(errno != 0) {
            perror("Cannot retrieve group information");
            errno = 0;
            exit(0);
        }
        else {
            f.group = grpid->gr_name;
            if(f.group.size() > largestGroup) {
                largestGroup = f.group.size();
            }
        }

        
        f.size = s.st_size;
        totalSize += s.st_blocks;
        for(int l = s.st_size; l > 0; l /= 10) {
            numDigits++;
        }
        if(numDigits > largestSize) {
            largestSize = numDigits;
        }
        numDigits = 0;

        char date[80];
        struct tm *timing = localtime(&s.st_mtime);
        strftime(date,80,"%b",timing);
        f.month = date;
        strftime(date,80,"%-d",timing);
        f.day = date;
        for(int k = timing->tm_mday; k > 0; k /= 10) {
            ++numDigits;
        }
        if(numDigits > largestDay) {
            largestDay = numDigits;
        }
        strftime(date,80,"%R",timing);
        f.time = date;
        f.name = *i;
        if((*i).size() > largestWordSize) {
            largestWordSize = (*i).size();
        }
        lf.push_back(f);
    }
    int width = 0;
    cout << spathname << ": " << endl;
    if(flagL) {
        cout << "Total " << totalSize/2 << endl;
    }


    // Prints out the contents of the current directory depending on flags
    for(std::list<fileEntry>::iterator i = lf.begin(); i != lf.end(); ++i) {
        if(flagL) {
            cout << (*i).perm << ' ' << setw(largestLink)
            << right << (*i).nlink << ' ' << setw(largestUser) << left << (*i).user
            << ' ' << setw(largestGroup) << left << (*i).group << ' ' << setw(largestSize)
            << right << (*i).size << ' ' << (*i).month 
            << ' ' << setw(largestDay) << right << (*i).day 
            << ' ' << (*i).time << ' ' << (*i).name << endl << flush;
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

    /* The following if statement is my implementation of the recursive search algorithm
     * Every time a directory is found, it is pushed into a list and is then iterated through
     * in this if statement, following the display of current directory's contents.
     */
    if(flagR && hasSubDir) {
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
            getCwdFilesRec((char*) (temp_path.c_str()));

        }
        //SubDirs.erase(SubDirs.begin(),SubDirs.end());
    }
    else if((flagR && !(flagL))) {
        cout << endl;
    }
    //return;

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
        if(i != IncPaths.begin()) {
            if(flagA && !(flagL) && !(flagR)) {
                cout << endl;
            }
            cout << endl;
        }
        getCwdFilesRec((char*) ((*i).c_str()));
    }
    if(!(flagL)) {
        cout << endl;
    }

    return 0;
}

