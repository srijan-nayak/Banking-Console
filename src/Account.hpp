/**
 * Account.hpp
 * Banking Console
 *
 * Created by Srijan Nayak on 19/10/2018.
 * Copyright © 2018 Srijan Nayak, Shyle Shaju. All rights reserved.
 */

#ifndef BANKINGCONSOLE_ACCOUNT_HPP
#define BANKINGCONSOLE_ACCOUNT_HPP

#include "sha256.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <termios.h>
#include <thread>
#include <unistd.h>

using namespace std::this_thread;
using namespace std::chrono;

#define sleep sleep_for(seconds(3))
#define clearScreen std::cout << "\033[2J\033[1;1H"

void setStdinEcho(bool);
void splitString(const std::string &longString, std::string *stringPart);
void combineString(std::string &longString, std::string *stringPart);

class Account {
  private:
    std::string name;
    std::string accountNumber;
    std::string phoneNumber;
    std::string email;
    std::string addressPart[8];
    std::string hashPart[8];
    std::string accountType;
    char dateOfBirth[11];
    char dateOfBirthString[9];

    void convertDate();
    void generateHash();
    void generateAccountNumber();
    const bool notValidName() const;
    const bool notValidDateOfBirth() const;
    const bool notValidPhoneNumber() const;
    const bool notValidEmail() const;

  public:
    double balance;

    const std::string &getAccountNumber() const;
    const std::string &getPhoneNumber() const;
    const std::string &getEmail() const;
    const char *getDateOfBirth() const;
    bool compareHash();
    void inputAccountDetails();
    void displayAccountDetails();
    void depositAmount();
    void withdrawAmount();
    void withdrawAmount(double);
    void changePassword();
};

#endif // BANKINGCONSOLE_ACCOUNT_HPP
