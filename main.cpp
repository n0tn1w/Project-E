#include <iostream>
#include <fstream>
#include <regex>
#include <stdio.h>
#include <string.h>

using namespace std;

string splitBy(string input, char symbol){
    int i = 0;
    for (auto &ch : input) {
        if(ch == symbol){
            break;
        }
        i++;
    }
    return input.substr(0,i);
}

bool checkIfTheNameAlreadyExist(string name){
    fstream dataBase;
    dataBase.open("user.txt", fstream::in | fstream::app);
    string line;
    while(getline(dataBase, line)){
        if(name == splitBy(line, ':')){
            return false;

        }
    }

    return true;
}

bool registeer(){
    hash <string> hash;
    fstream dataBase;

    regex nameRegex("^[a-z,A-Z,0-9]+$");
    regex passwordRegex("^(?=.*[a-z])(?=.*[0-9])(?=.*[A-Z])(?=.*[&*<>?.+-])[a-z,0-9,A-Z,&*<>?.+-]{6,}$");
    string name, password;

    cout << "Enter username: ";
    getline(cin, name);
    cout << "Enter password: ";
    getline(cin, password);

    if(regex_match(name, nameRegex)
       && regex_match(password, passwordRegex)
       && checkIfTheNameAlreadyExist(name)){

        dataBase.open("user.txt", fstream::out | fstream::app);
        dataBase << name << ":" << hash(password) << "\n";
        dataBase.close();

        string fileName = name + ".txt";
        dataBase.open(fileName, fstream::app);
        dataBase.close();
        return true;
    }else if(!checkIfTheNameAlreadyExist(name)){
        cout << "User already exist!"<< endl;
        return false;
    }else if(!regex_match(name, nameRegex)){
        cout << "Invalid name!"<< endl;
        return false;
    }else if(!regex_match(password, passwordRegex)){
        cout << "Invalid password!"<< endl;
        return false;
    }else{
        cout << "Incorrect username or password!!!"<< endl;
        return false;
    }



}

int main()
{
    fstream dataBase;
    dataBase.open("user.txt", fstream::in | fstream::app);
    dataBase.close();

    cout << "Enter a command: ";
    string command;
    getline(cin, command);

    while(true){
        cout << "Time to register" << endl;
        registeer();
    }

    return 0;
}
