/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: istroschu
 *
 * Created on 8. November 2017, 12:25
 */


#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <vector>

using namespace std;

/*
 * 
 */
 void handle_SIGCHLD(int signum) {
 	pid_t pid;
 	pid=wait(NULL);
 	cout<<"pid: "<<pid<< " exites"<<endl;
 }
 
char* shell_read_line() {
    char* read= new char[256];

    cin.getline(read,256);
    return read;
}

char** shell_parse_line(char* line) {
    char* tmp;
    vector<char*>tmp2;

    tmp =strtok(line, " \t\r\n\a");
    while(tmp !=NULL){
    	tmp2.push_back(tmp);
    	tmp=strtok(NULL, " \t\r\n\a");
    }
    char** tokens = new char*[tmp2.size()];
    for (int i = 0; i < tmp2.size();i++)
    tokens[i]=tmp2[i];
    return tokens;
}

int launch(char** args) {
    pid_t pid, wpid;
    int status;
    string s(args[0]);

    //Schaut ob ein Hintergrundprozess gestartet werden soll
    bool bk_process = (s.back() == '&');
    if (bk_process) {
        s.erase(s.end() - 1);
        const char* prog = s.c_str();
        args[0] = strdup(prog);
    }

    pid = fork();
    if (pid == 0) {
        //Kind Prozess
        if (execvp(args[0], args) == -1) {
            cerr << "Error on exec." << endl;
            //Beendet den Process falls etwas schief geht
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        //Fehler beim fork-Systemaufruf
        cerr << "Error on fork." << endl;
    } else {
        //Falls es ein Hintergrundprozess ausgeführt werden soll, muss nicht gewartet werden
        if (bk_process) {
            cout << "Started " << args[0] << " in background with PID: " << pid << endl;
        } else {
            //Wartet darauf, dass der Kind Prozess sich beendet
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    return 1;
}

int execute(char** args) {
    string check(args[0]);
    if (args[0] == NULL) {
        cout << "Invalid command." << endl;
        return 1;
    } else if (check =="logout") {
        cout << "Wollen Sie wirklich das Programm schließen? (Y/n)" << endl;
        string input;
        cin >> input;
        if (input == "Y"){
            exit(0);
        }
    }
    return launch(args);
}

void shell_loop() {
    int status = 1;
    char* line;
    char** arguments;

    do {
        cout << "$$$ ";
        line = shell_read_line();
        arguments = shell_parse_line(line);
        status=execute(arguments);
        
        delete line;
        delete[] arguments;
    } while (status);
}

int main(int argc, char** argv) {


    signal(SIGCHLD, handle_SIGCHLD);
    shell_loop();

    return 0;
}