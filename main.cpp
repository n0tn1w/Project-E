/**
*
* Solution to course project # 3
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2021/2022
*
* @author Daniel Manchevski
* @idnumber 2MI0600094
* @compiler GCC
*
*
*
*/



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
            return true;
        }
    }
    return false;
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
    longgedUser = "";
    return true;
}
bool isNumber(const string& str){
    for(char const &c : str){
        if(std::isdigit(c) == 0){
            return false;
        }
    }
    return true;
}
int numbersOfEmailOfAPerson(){
    string titleOfTheUserFile = longgedUser + ".txt";
    fstream userRead;
    userRead.open(titleOfTheUserFile, fstream::in | fstream::app);
    int counter = 1;
    string line;
    while(getline(userRead, line)){
        counter++;
    }
    userRead.close();

    return counter;
}

bool checkIfTheUserHasAnEmailWithNumber(string input){
    if(!isNumber(input)){
        return false;
    }
    int number = stoi(input);

    int numberOfEmails = numbersOfEmailOfAPerson();

    if(number < numberOfEmails && number > 0 ){
        return true;
    }else{
        return false;
    }
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
       && !checkIfTheNameAlreadyExist(name)){

        dataBase.open("user.txt", fstream::out | fstream::app);
        dataBase << name << ":" << hash(password) << "\n";
        dataBase.close();

        string fileName = name + ".txt";
        dataBase.open(fileName, fstream::app);
        dataBase.close();
        cout << "Register successfully!" << endl;
        fstream authentication;
        authentication.open("authentication.txt", fstream::out | fstream::app);
        authentication << longgedUser << "\n";
        authentication.close();
        return true;
    }else if(checkIfTheNameAlreadyExist(name)){
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
    if(checkIfTheNameAlreadyExist(name)
        && checkIfPasswordIsCorrect(name ,password)){
        cout << "Login successfully!" << endl;
        fstream authentication;
        authentication.open("authentication.txt", fstream::out | fstream::app);
        authentication << longgedUser << "\n";
        authentication.close();
        return true;

    }else if(!checkIfTheNameAlreadyExist(name)){
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
void myInbox(){
    int emailsTotal = numbersOfEmailOfAPerson();
    if(emailsTotal <= 1){
        cout << "You have no emails :(" << endl;
    }else{
        fstream user;
        string title = longgedUser + ".txt";
        user.open(title, fstream::in | fstream::app);

        string line;
        while(getline(user, line)){
            cout << line << endl;
        }
    }
}

bool logOut(){
    longgedUser = "";
    cout << "You logged out successfully!" << endl;
    fstream authentication;
    authentication.open("authentication.txt", fstream::out | fstream::trunc);
    authentication.close();
    return true;
}
void readAnEmail(){
    fstream dataBase;

    string number;
    cout << "Enter email number: ";
    getline(cin, number);

    string emailTitle = longgedUser + "-" + number + ".txt";

    if(checkIfTheUserHasAnEmailWithNumber(number)){
        dataBase.open(emailTitle, fstream::in | fstream::app);

        string from, subject, content;
        getline(dataBase, from);
        getline(dataBase, subject);
        getline(dataBase, content);

        cout << "From: " << from << endl;
        cout << "Subject: " << subject << endl ;
        cout << "Content: " << content << endl;
    }else{
        cout << "Invalid email number!" << endl;
    }
}


bool sendAnEmail(){
    string receiverName, subject, content;

    cout << "Enter the receiver's username: ";
    getline(cin, receiverName);
    cout << "Enter the subject: ";
    getline(cin, subject);
    cout << "Enter the content: ";
    getline(cin, content);

    if(checkIfTheNameAlreadyExist(receiverName)){
        if(subject == ""){
            cout << "This mail has no subject!" << endl;
            return false;
        }
        if(content == ""){
            cout << "This mail has no content!" << endl;
            return false;
        }
        string titleOfTheUserFile = receiverName + ".txt";
        fstream userRead;
        userRead.open(titleOfTheUserFile, fstream::in | fstream::app);
        //Counter must start from zero D: not from 1;
        int counter = 1;
        string line;
        while(getline(userRead, line)){
            counter++;
        }
        userRead.close();


        string titleOfTheMail = receiverName + "-" + to_string(counter) + ".txt";
        fstream mail;
        mail.open(titleOfTheMail, fstream::in | fstream::app);
        mail.close();
        mail.open(titleOfTheMail, fstream::out | fstream::app);
        mail << longgedUser << "\n";
        mail << subject << "\n";
        mail << content << "\n";
        mail.close();

        fstream userChange;
        userChange.open(titleOfTheUserFile, fstream::out | fstream::app);
        userChange << counter << " " << subject << "\n";
        userChange.close();

        cout << "Mail send to " << receiverName << endl;
        return true;
    }else{
        cout << "This person doesnt exist!" << endl;
        return false;
    }
}

int main()
{
    fstream dataBase;
    dataBase.open("user.txt", fstream::in | fstream::app);
    dataBase.close();

    fstream authentication;
    authentication.open("authentication.txt", fstream::in | fstream::app);
    string line;
    bool areUOnTheHomePage = true;
    bool firstLoginWithAuthentication;
    if(getline(authentication, line)){
        longgedUser = line;
        firstLoginWithAuthentication = true;
        areUOnTheHomePage = false;
    }else{
        areUOnTheHomePage = true;
    }
    authentication.close();

    bool stay = true;
    while(true){
        if(areUOnTheHomePage){
            if(stay){
                cout << "-------------------------" << endl;
                cout << "Welcome to Home Screen :)" << endl;
                cout << "R - register" << endl;
                cout << "L - login" << endl;
                cout << "Q - quit" << endl;
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
            if(!stay || firstLoginWithAuthentication){
                cout << "-------------------------" << endl;
                cout << "Welcome to Work Screen 8]" << endl;
                cout << "Hello " << longgedUser << endl;
                cout << "You have X mails. Choose one of the following options:" << endl;
                cout << "C - close account" << endl;
                cout << "I - inbox" << endl;
                cout << "L - logout" << endl;
                cout << "O - open" << endl;
                cout << "S - send" << endl;
                firstLoginWithAuthentication = false;
            }
            cout << "Enter a command: ";
            string command;
            getline(cin, command);

            if(command == "c" || command == "C"){
                areUOnTheHomePage = closeAccount();
            }else if(command == "i" || command == "I"){
                myInbox();
            }else if(command == "l" || command == "L"){
                areUOnTheHomePage = logOut();
            }else if(command == "o" || command == "O"){
                readAnEmail();
            }else if(command == "s" || command == "S"){
                sendAnEmail();
            }else{
                cout << "Invalid command!" << endl;
            }

            stay = true;
        }

    }


    return 0;
}
