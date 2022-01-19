#include <iostream>
#include <fstream>
#include <regex>
#include <stdio.h>
#include <string.h>
#include <cstdio>

using namespace std;

string longgedUser;

string getNameWithLineInput(string input, char symbol){
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
        if(name == getNameWithLineInput(line, ':')){
            return false;
        }
    }
    return true;
}

string getHashedPassword(string name){
    fstream dataBase;
    dataBase.open("user.txt", fstream::in | fstream::app);
    string line;

    string correctLine;
    while(getline(dataBase, line)){
        if(name == getNameWithLineInput(line, ':')){
            correctLine = line;
        }
    }
    int i = 0;
    for (auto &ch : correctLine) {
        if(ch == ':'){
            break;
        }
        i++;
    }
    return correctLine.substr(i + 1, correctLine.length());
}
bool checkIfPasswordIsCorrect(string name ,string passwordGuess){
    hash <string> hash;
    long long passwordGuessHashNumber = hash(passwordGuess);
    string passwordGuessHashString = to_string(passwordGuessHashNumber);

    string passwordRealHash = getHashedPassword(name);
    return passwordRealHash == passwordGuessHashString;
}

bool removeAUserFromTheDB(){
    fstream dataBase;
    dataBase.open("user.txt", fstream::in | fstream::app);
    vector<string> dbSave;
    string line;

    while(getline(dataBase, line)){
            cout << "Name: " <<getNameWithLineInput(line, ':') << " ";
        if(longgedUser == getNameWithLineInput(line, ':')){
            cout << "removed" << line << endl;
            continue;
        }else{
            dbSave.push_back(line);
            cout << line << endl;
        }
    }
    dataBase.close();
    dataBase.open("user.txt", fstream::out | fstream::trunc);
    for(int i = 0;i < dbSave.size(); i++){
        dataBase << dbSave[i] << "\n";
    }
    dataBase.close();
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
    longgedUser = name;

    if(regex_match(name, nameRegex)
       && regex_match(password, passwordRegex)
       && checkIfTheNameAlreadyExist(name)){

        dataBase.open("user.txt", fstream::out | fstream::app);
        dataBase << name << ":" << hash(password) << "\n";
        dataBase.close();

        string fileName = name + ".txt";
        dataBase.open(fileName, fstream::app);
        dataBase.close();
        cout << "Register successfully!" << endl;
        return true;
    }else if(!checkIfTheNameAlreadyExist(name)){
        cout << "User already exist!" << endl;
        return false;
    }else if(!regex_match(name, nameRegex)){
        cout << "Invalid name!" << endl;
        return false;
    }else if(!regex_match(password, passwordRegex)){
        cout << "Invalid password!" << endl;
        return false;
    }else{
        cout << "Incorrect action!" << endl;
        return false;
    }
}


bool login(){
    fstream dataBase;
    string name, password;

    cout << "Enter username: ";
    getline(cin, name);
    cout << "Enter password: ";
    getline(cin, password);
    longgedUser = name;

    dataBase.open("user.txt", fstream::in | fstream::app);
    if(!checkIfTheNameAlreadyExist(name)
        && checkIfPasswordIsCorrect(name ,password)){
        cout << "Login successfully!" << endl;
        return true;

    }else if(checkIfTheNameAlreadyExist(name)){
        cout << "This user doesnt exist!" << endl;
        return false;
    }else if(!checkIfPasswordIsCorrect(name ,password)){
        cout << "Wrong password!" << endl;
        return false;
    }else{
        cout << "Incorrect action!" << endl;
        return false;
    }
}

bool closeAccount(){
    fstream dataBase;
    string password;
    cout << "Enter password to confirm: ";
    getline(cin, password);

    if(checkIfPasswordIsCorrect(longgedUser ,password)){
        if(removeAUserFromTheDB()){
            string fileName = longgedUser + ".txt";
            std::remove(fileName.c_str());
            cout << "Account deleted successfully!" << endl;
            return true;
        }else{
            cout << "Error in the DataBase!" << endl;
            return false;
        }

    }else if(!checkIfPasswordIsCorrect(longgedUser ,password)){
        cout << "Wrong password!" << endl;
        return false;
    }else{
        cout << "Incorrect action!" << endl;
        return false;
    }
}

int main()
{
    fstream dataBase;
    dataBase.open("user.txt", fstream::in | fstream::app);
    dataBase.close();

    bool areUOnTheHomePage = true;
    bool stay = true;
    //!areUOnTheHomePage
    while(true){
        if(areUOnTheHomePage){
            if(stay){
                cout << "-------------------------" << endl;
                cout << "Welcome to Home Screen :)" << endl;
            }
            cout << "Enter a command: ";
            string command;
            getline(cin, command);

            if(command == "r" || command == "R"){
                areUOnTheHomePage = !registeer();
            }else if(command == "l" || command == "L"){
                areUOnTheHomePage = !login();
            }else if(command == "q" || command == "Q"){
                return 0;
            }else{
                //cout << "Do u have hands?";
                cout << "Invalid command!" << endl;
            }
            stay = false;
        }else{
            if(!stay){
                cout << "-------------------------" << endl;
                cout << "Welcome to Work Screen 8]" << endl;
                cout << "Hello " << longgedUser << endl;
                cout << "You have X mails. Choose one of the following options:" << endl;
                cout << "C - close account" << endl;
                cout << "I - inbox" << endl;
                cout << "L - logout" << endl;
                cout << "O - open" << endl;
                cout << "S - send" << endl;
            }
            cout << "Enter a command: ";
            string command;
            getline(cin, command);

            if(command == "c" || command == "C"){
                areUOnTheHomePage = closeAccount();
            }

            stay = true;
        }

    }


    return 0;
}
