#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <boost/tokenizer.hpp>
using namespace std;
using namespace boost;
// Method for getting user and hostname then prompting it
void userprompt() {
	char hname[128] = "";
	auto c = cuserid( getlogin() );
	gethostname(hname,sizeof(hname));
	cout << c << '@' << hname << "$ " << flush;
}



int main(int argc, char *argv[])
{
	while(1) {
		//initializing variables
		string amp  = "&&";
		string bar = "||";
		string scolon = ";";
		char *cmdline = NULL;
		userprompt();
		size_t n = 0;
		size_t nchar = 0;
		int pid;
		int status;
		if((nchar = getline (&cmdline, &n, stdin)) ) {
			cmdline[--nchar] = 0; //take off the newline from getline
		}
		// exit immediately if user enter exit
		if(strcmp(cmdline, "exit") == 0) {
			exit(0);
		}
		char *token = strtok( cmdline, " "); //separate into tokens by whitespace
		char *arg[999];
		char **next = arg;
		int num_arg = 1;
		vector<int> index= {0};
		// This loop places tokens into char **arg and null terminates each command or connector
		while( token != NULL ) {
			*next++ = token;
			++num_arg;
			token = strtok(NULL, " ");
			if(token != NULL) {
				if( (strcmp( token, amp.c_str()) == 0) ||
				strcmp( token, bar.c_str())== 0|| strcmp( token, scolon.c_str())== 0) {
					*next++ = NULL;
					*next++ = token;
					index.push_back(num_arg);
					++num_arg;
					token = strtok(NULL, " ");
					if(token != NULL) {
						*next++ = token;
						index.push_back(num_arg);
						token = strtok(NULL, " ");
					}
				}
			}
		}
		*next = NULL;
		--num_arg;
		// Loop for executing commands
		for(unsigned i = 0; i < index.size(); ++i ) {
			pid = fork();
			int childpid;
			if(pid == 0) {
					childpid = execvp(arg[index.at(i)], arg+index.at(i));
					if(childpid == -1) {
						perror("Error with exec");
						exit(1);
					}
			}
			else if(pid < 0) {
				perror("Error with fork().");
				exit(1);
			}
			else {
				waitpid(pid, &status, 0);
				// Supposed to keep track of connectors ||, &&, ; , but is causes segmentation faults
				// in its current state
				/*if(!WIFEXITED(status) ) {
					if((i+1) < index.size() &&( (strcmp(arg[index.at(i+1)], amp.c_str()) == 0)
					|| strcmp(arg[index.at(i+1)], scolon.c_str() ) == 0)) {
						++i;
					}
					else if((i+2) < index.size() &&
					(strcmp(arg[index.at(i+1)], bar.c_str() ) == 0) ) {
						i+2;
					}
				}
				else{
					if((i+2) < index.size() &&( (strcmp(arg[index.at(i+1)], amp.c_str()) == 0)
					|| strcmp(arg[index.at(i+1)], scolon.c_str() ) == 0)) {
						i+2;
					}
					else if((i+2) < index.size() &&
					(strcmp(arg[index.at(i+1)], bar.c_str() ) == 0) ) {
						++i;
					}
				}*/
			}
		}
	}
}
