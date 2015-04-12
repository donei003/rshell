#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
using namespace std;

void lsCom() {

}

int main(int argc, char *argv[]) {
if(argc < 1) {
    cout << "No arguements passed in" << endl;
   // exit(1);
}
else {
    //cout << get_current_dir_name() << endl;
    DIR *dirp;
    if( NULL == (dirp = opendir(get_current_dir_name()))) {
      perror("The directory could not be opened or does not exist. ");
      exit(1);
    }
    struct dirent *file;
    errno = 0;
    while(NULL != (file = readdir(dirp))) {
      cout << file->d_name << " ";
    }
    cout << endl;
  }
return 0;
}
