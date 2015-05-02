#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <iomanip>
#include <ftw.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
using namespace std;
bool a_FLAG = false;
bool l_FLAG = false;
bool R_FLAG = false;
void AlphaSort(vector<dirent *> files,string path) {
	auto temp = files.at(0);
	unsigned i = 0;
	unsigned index = 0;
	nlink_t total;
	struct stat x;
	for(i = 0; i < files.size();  ++i) {
		temp = files.at(i);
		index = i;
		for(unsigned j = i; j < files.size(); ++j) {
			if(strcasecmp(temp->d_name, files.at(j)->d_name) >=  0 ) {
				if(strcasecmp(temp->d_name, files.at(j)->d_name) == 0) {
					if(strcmp(temp->d_name,files.at(j)->d_name) < 0) {
						temp = files.at(j);
						index = j;
					}
				}
				else {	
					temp = files.at(j);
					index = j;
				}
			}
		}
		if(temp != files.at(i)) {
			swap(files.at(i), files.at(index));
		}
		string pathh = path;
			pathh += '/';
			pathh.append(files.at(i)->d_name);
			if(stat((pathh.c_str()), &x) < 0) {
				perror("Error with stat()");
				exit(1);
			}
			total += (x.st_blocks);
	}
	if(l_FLAG) {
		total = total / x.st_blksize;
		cout << "Total: " << total<< endl;
		
		for(unsigned a = 0; a < files.size(); ++a)
		{
			errno = 0;
			struct stat s;
			string patth = path;
			patth += '/';
			patth.append(files.at(a)->d_name);
			if(stat((patth.c_str()), &s) < 0) {
				perror("Error with stat()");
				exit(1);
			}
			auto user = getpwuid(s.st_uid);
			if( user == 0) {
				perror("Error with getpwuid(). ");
				exit(1);
			}
			auto group = getgrgid(s.st_gid);
			if(group == 0) {
				perror("Error with getgrgid(). ");
				exit(1);
			}
			time_t t = s.st_mtime;
			struct tm lt;
			localtime_r(&t, &lt);
			char datestring[80];
			strftime(datestring, sizeof(datestring), "%b %d %H:%M", &lt);
			cout << ((S_IFDIR & s.st_mode)?"d":"-")
				 << ((S_IRUSR & s.st_mode)?"r":"-")
				 << ((S_IWUSR & s.st_mode)?"w":"-")
				 << ((S_IXUSR & s.st_mode)?"x":"-")
				 << ((S_IRGRP & s.st_mode)?"r":"-")
				 << ((S_IWGRP & s.st_mode)?"w":"-")
				 << ((S_IXGRP & s.st_mode)?"x":"-")
				 << ((S_IFDIR & s.st_mode)?"r":"-")
				 << ((S_IFDIR & s.st_mode)?"w":"-")
				 << ((S_IFDIR & s.st_mode)?"x":"-")
			 	 << " " << setw(3) << setfill(' ') << s.st_nlink
				 << " " << setw(8) << setfill(' ') << user->pw_name
				 << " " << setw(8) << setfill(' ') << group->gr_name
				 << " " << setw(6) << setfill(' ') << s.st_size
				 << " " << datestring
				 << " " << files.at(a)->d_name
				 << endl;
		}
	}
	else {
		for(i =0; i < files.size(); ++i)
		{
			cout << files.at(i)->d_name << " ";
		}
		cout << endl;
	}
}
/*
void Recursive(vector<dirent *> files, string path) {
	//AlphaSort(files,path);
	DIR *dirp;
	if(NULL== (dirp = opendir(path))) {
		perror("There was an error with opendir(). ");
	}
	struct dirent* filespec;
	char dot = '.';
	vector<dirent*> file;
	while(NULL != (filespec = readdir(dirp)) {
		if(!a_FLAG) {
			if(filespec->d_name[0] != dot) {
				files.push_back(filespec);
			}
		}
		else {
			files.push_back(filespec);
		}
	}
}
*/
	
void ReadDir(int argc, char **argv) {
	DIR *dirp;
	char CurrentPath[FILENAME_MAX];
	if(!getcwd(CurrentPath, sizeof(CurrentPath))) {
		perror("There was an error with getcwd");
		exit(1);
	}
	string _path;
	_path.append(CurrentPath);
	_path += '/';

	CurrentPath[sizeof(CurrentPath)-1] = '\0';
	if(NULL == (dirp = opendir(CurrentPath))) {
		perror("There was an error with opendir(). ");
	}
	struct dirent *filespecs;
	errno = 0;
	char dot = '.';
	vector<dirent *> files;
	while(NULL != (filespecs = readdir(dirp))) {
		if(!a_FLAG) {
			if(filespecs->d_name[0] != dot) {
				files.push_back(filespecs);
			}
		}
		else {
			files.push_back(filespecs);
		}
	}
	if(errno != 0) {
		perror("There was an error with readdir(). ");
		exit(1);
	}
	if(R_FLAG) {
		cout << ".:" << endl;
		AlphaSort(files,_path);
		//Recursive(files,_path);
	}
	else {
		AlphaSort(files, _path);
	}
	if(-1 == closedir(dirp)) {
		perror("There was an error with closedir(). ");
		exit(1);
	}
}
string combine( char* argv, char *cwd) {
	string file = argv;
	string path = cwd;
	path += '/';
	path += file;
	return path;
}
void Read_Dir(int argc, char **argv) {
	DIR *dirp;
	char CurrentPath[FILENAME_MAX];
	if(!getcwd(CurrentPath, sizeof(CurrentPath))) {
		perror("There was an error with getcwd");
		exit(1);
	}
	string path = combine(argv[0], CurrentPath);
	CurrentPath[sizeof(CurrentPath)-1] = '\0';
	
	if(NULL == (dirp = opendir(path.c_str() ))) {
		perror("There was an error with opendir(). ");
		return;
	}
	struct dirent *filespecs;
	errno = 0;
	char dot = '.';
	vector<dirent *> files;
	while(NULL != (filespecs = readdir(dirp))) {
		if(!a_FLAG) {
			if(filespecs->d_name[0] != dot) {
				files.push_back(filespecs);
			}
		}
		else {
			files.push_back(filespecs);
		}
	}
	if(errno != 0) {
		perror("There was an error with readdir(). ");
		return;
	}
	AlphaSort(files, path);
	if(-1 == closedir(dirp)) {
		perror("There was an error with closedir(). ");
		return;
	}
}
int main(int argc, char **argv)
{
	int ch = 0;
	opterr = 0;
	while(( ch = getopt(argc, argv, "alR")) != -1) 
	{
		switch(ch)
		{
			case 'a':
				a_FLAG = true;
				break;
			case 'l':
				l_FLAG = true;
				break;
			case 'R':
				R_FLAG = true;
				break;
			case '?':
				cout << "Parameter not supported"  << endl;
		}		
	}
	argc -= optind;
	if(argc == 0)
	{
		ReadDir(argc, argv);
		exit(1);
	}
	argv += optind;
	while( argc > 0 )
	{
		Read_Dir(argc, argv);
		++argv;
		--argc;
	}
	return 0;
}
