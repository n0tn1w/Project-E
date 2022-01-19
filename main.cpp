#include <iostream>
#include <fstream>
#include <regex>
#include <stdio.h>
#include <string.h>

using namespace std;

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
        cout << "Incorrect username or password!!!" << endl;
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
        cout << "Incorrect username or password!" << endl;
        return false;
    }
}

int main()
{
    fstream dataBase;
    dataBase.open("user.txt", fstream::in | fstream::app);
    dataBase.close();

    bool areUOnTheHomePage = false;
    //!areUOnTheHomePage
    cout << "-------------------------" << endl;
    cout << "Welcome to Home Scree :)" << endl;
    while(!false){
        cout << "Enter a command: ";
        string command;
        getline(cin, command);

        if(command == "r" || command == "R"){
            areUOnTheHomePage = registeer();
        }else if(command == "l" || command == "L"){
            areUOnTheHomePage = login();
        }else if(command == "q" || command == "Q"){
            return 0;
        }else{
            cout << "Do u have hands?";

        }
    }


    return 0;
}
