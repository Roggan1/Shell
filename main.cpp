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
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

/*
 * 
 */
string shell_read_line() {
    string read;

    getline(cin,read);
    return read;
}

vector<string> shell_parse_line(string line) {
    stringstream ss;
    string tmp;
    vector<string>tmp2;

    ss.str(line);
    while (!ss.eof()) {
        ss>>tmp;
        tmp2.push_back(tmp);
    }
    return tmp2;
}

int fork(){
    
}

int shell_launch(vector<string> arguments) {
    int pid;
    if (arguments.at(0)=="logout"){
        return 0;
    }
    
    return 1;
}

void shell_loop() {
    int status = 1;
    string line;
    vector<string> arguments;

    do {
        cout << "$$$ ";
        line = shell_read_line();
        arguments = shell_parse_line(line);
        status=shell_launch(arguments);
        
    } while (status);
}

int main(int argc, char** argv) {

    shell_loop();

    return 0;
}