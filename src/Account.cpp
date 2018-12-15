/**
 * Account.cpp
 * Banking Console
 *
 * Created by Srijan Nayak on 19/10/2018.
 * Copyright © 2018 Srijan Nayak, Shyle Shaju. All rights reserved.
 */

#include "Account.hpp"

/**
 * Stops stdout so password can be entered
 * without the input being shown on the screen.
 */
void setStdinEcho(bool enable = true) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

/**
 * Splits long strings into 8 equal parts so
 * that they can stored in a binary file without
 * exceeding the 80 char width limit.
 */
void splitString(const std::string &longString, std::string stringPart[]) {
    int n = static_cast<int>(longString.length());
    int pre;
    int at;
    for (int i = pre = 0; i < 8; i++) {
        at = (n * i + n) / 8;
        stringPart[i] = longString.substr(static_cast<unsigned long>(pre),
                                          static_cast<unsigned long>(at - pre));
        pre = at;
    }
}

/**
 * Combines 8 strings into one single string
 * so that the data can be used as normal string
 * in the program.
 */
void combineString(std::string &longString, std::string stringPart[]) {
    for (int i = 0; i < 8; i++) {
        longString += stringPart[i];
    }
}

/**
 * Returns the account number stored in
 * the current object.
 */
const std::string &Account::getAccountNumber() const { return accountNumber; }

/**
 * Returns the user's phone number stored in
 * the current object.
 */
const std::string &Account::getPhoneNumber() const { return phoneNumber; }

/**
 * Returns the user's email address stored in
 * the current object.
 */
const std::string &Account::getEmail() const { return email; }

/**
 * Returns the users date of birth stored in
 * the current object.
 */
const char *Account::getDateOfBirth() const { return dateOfBirth; }

/**
 * Takes in a password input from the user
 * and then generate a hash with the sha256
 * hash function.
 */
void Account::generateHash() {
    std::string passwordHash;
    char pass[17];
    setStdinEcho(false);
    std::cin.getline(pass, 17);
    setStdinEcho(true);
    std::cout << '\n';
    passwordHash = sha256(pass);
    splitString(passwordHash, hashPart);
}

/**
 * Converts a date string of the format
 * "dd/mm/yyyy" to a string of the format
 * "ddmmyyyy" which can be used to generate
 * the account number.
 */
void Account::convertDate() {
    int i = 0;
    int j = 0;
    while (i < 11) {
        while (j < 9) {
            if (isdigit(dateOfBirth[i])) {
                dateOfBirthString[j] = dateOfBirth[i];
                j++;
                break;
            } else {
                break;
            }
        }
        i++;
        dateOfBirthString[8] = char(0);
    }
    generateAccountNumber();
}

/**
 * Generates the account number by prefixing
 * the converted date of birth string with
 * "34" and suffixing it with the current
 * ACCOUNT_CODE that is stored in accCode.dat
 */
void Account::generateAccountNumber() {
    int ACCOUNT_CODE;
    std::fstream file;
    file.open("data/accCode.dat", std::ios::binary | std::ios::in);
    file.read((char *)&ACCOUNT_CODE, sizeof(ACCOUNT_CODE));
    file.close();
    accountNumber = "34";
    accountNumber += std::string(dateOfBirthString);
    accountNumber += std::to_string(ACCOUNT_CODE);
    ACCOUNT_CODE++;
    file.open("data/accCode.dat", std::ios::binary | std::ios::out);
    file.write((char *)&ACCOUNT_CODE, sizeof(ACCOUNT_CODE));
    file.close();
}

/**
 * Takes in a password input from the user
 * and then generates a hash with the sha256
 * hash function and compares with the hash
 * stored in the current object.
 */
bool Account::compareHash() {
    bool confirmation;
    char pass[17];
    std::string passwordHash;
    std::string hash;
    combineString(passwordHash, hashPart);
    setStdinEcho(false);
    std::cin.getline(pass, 17);
    setStdinEcho(true);
    std::cout << '\n';
    hash = sha256(pass);
    confirmation = hash == passwordHash;
    return confirmation;
}

/**
 * Takes in the input of all necessary
 *  details for creating an account
 *  from the user and validates them.
 */
void Account::inputAccountDetails() {
    std::cout << "Enter name or enter 'e' to exit to menu: ";
    std::cin.ignore();
getName:
    std::getline(std::cin, name);
    if (name == "e") {
        return;
    }
    if (notValidName()) {
        std::cout << "Please enter a valid name or enter 'e' to exit to menu: ";
        goto getName;
    }

    std::cout << "Enter date of birth in dd/mm/yyyy format: ";
getDate:
    std::cin.getline(dateOfBirth, 11);
    if (notValidDateOfBirth()) {
        std::cout << "Please enter date in dd/mm/yyyy format: ";
        goto getDate;
    }
    convertDate();

    std::cout << "Enter phone number without any prefix: ";
getPhoneNumber:
    std::getline(std::cin, phoneNumber);
    if (notValidPhoneNumber()) {
        std::cout << "Please enter valid phone number without any prefix: ";
        goto getPhoneNumber;
    }

    std::cout << "Enter email address: ";
getEmail:
    std::getline(std::cin, email);
    if (notValidEmail()) {
        std::cout << "Please enter a valid email address: ";
        goto getEmail;
    }

    std::cout << "Enter address: ";
    std::string address;
getAddress:
    std::getline(std::cin, address);
    if (address.empty()) {
        std::cout << "Please enter a valid address: ";
        goto getAddress;
    }
    splitString(address, addressPart);

    std::cout << "Enter password (maximum 16 characters): ";
getPassword:
    generateHash();
    std::cout << "Confirm password: ";
    if (!compareHash()) {
        std::cout << "Passwords don't match!" << '\n';
        std::cout << "Please re-enter password: ";
        goto getPassword;
    }

    std::cout << "Enter account type (Current/Savings): ";
getAccountType:
    std::getline(std::cin, accountType);
    if (!(accountType == "Current" || accountType == "Savings")) {
        std::cout << "Invalid input! Please enter valid account type: ";
        goto getAccountType;
    }

    std::cout << "Enter starting balance: ";
getAmount:
    std::cin >> balance;
    if (balance < 0) {
        std::cout << "Please enter a valid balance: ";
        goto getAmount;
    }

    clearScreen;
    std::cout << "Account created successfully" << '\n' << '\n';
    std::cin.ignore();
    displayAccountDetails();
}

/**
 * Provides validation for name.
 * Returns true if name is not
 * valid.
 */
const bool Account::notValidName() const {
    bool notValid = false;
    for (char ch : name) {
        if (!(isalpha(ch) || isspace(ch))) {
            notValid = true;
        }
    }
    return notValid;
}

/**
 * Provides validation for date of birth.
 * Returns true if date of birth is not
 * valid.
 */
const bool Account::notValidDateOfBirth() const {
    return (strlen(dateOfBirth) != 10) || (dateOfBirth[2] != '/') ||
           (dateOfBirth[5] != '/') || (!isdigit(dateOfBirth[0])) ||
           (!isdigit(dateOfBirth[1])) || (!isdigit(dateOfBirth[3])) ||
           (!isdigit(dateOfBirth[4])) || (!isdigit(dateOfBirth[6])) ||
           (!isdigit(dateOfBirth[7])) || (!isdigit(dateOfBirth[8])) ||
           (!isdigit(dateOfBirth[9]));
}

/**
 * Provides validation for phone number.
 * Returns true if phone number is not
 * valid.
 */
const bool Account::notValidPhoneNumber() const {
    bool notValid = false;
    for (char ch : phoneNumber) {
        if (!isdigit(ch)) {
            notValid = true;
        }
    }
    if (phoneNumber.length() != 10) {
        notValid = true;
    }
    return notValid;
}

/**
 * Provides validation for email address.
 * Returns true if email address is not
 * valid.
 */
const bool Account::notValidEmail() const {
    bool noAtTheRate = true;
    bool noDot = true;
    bool notValid = true;
    for (char ch : email) {
        if (ch == '@') {
            noAtTheRate = false;
        } else if (ch == '.') {
            noDot = false;
        }
    }
    if (!noAtTheRate && !noDot) {
        notValid = false;
    }
    return notValid;
}

/**
 * Displays all account details of the user.
 * Does not display sensitive info like the
 * password.
 */
void Account::displayAccountDetails() {
    std::string address;
    combineString(address, addressPart);
    std::cout << "         Account Details" << '\n';
    std::cout << "         ———————————————" << '\n';
    std::cout << " Account number: " << accountNumber << '\n';
    std::cout << "           Name: " << name << '\n';
    std::cout << "  Date of birth: " << dateOfBirth << '\n';
    std::cout << "   Phone number: " << phoneNumber << '\n';
    std::cout << "  Email address: " << email << '\n';
    std::cout << "        Address: " << address << '\n';
    std::cout << "   Account type: " << accountType << '\n';
    std::cout << "Current balance: Rs. " << balance << '\n';
    std::cout << '\n';
    std::cout << "Press ENTER after noting down the details." << '\n';
    std::cin.get();
}

/**
 * Allows the user to deposit funds in his/her
 * account.
 */
void Account::depositAmount() {
    double amount;
    std::cout << "Enter amount to be deposited: ";
getAmount:
    std::cin >> amount;
    if (amount < 0) {
        std::cout << "Please re-enter a valid amount to be deposited: ";
        goto getAmount;
    }
    std::cin.ignore();
    std::cout << "Enter password: ";
getPassword:
    bool confirmation = compareHash();
    if (!confirmation) {
        std::cout << "Incorrect password!" << '\n';
        std::cout << "Please re-enter password: ";
        goto getPassword;
    }
    std::cout << "Rs. " << amount << " deposited successfully." << '\n';
    balance += amount;
    std::cout << "Current balance: " << balance << '\n';
    sleep;
}

/**
 * Allows the user to withdraw funds from his/her
 * account. If the account type is 'Current', then
 * withdrawal request is rejected if final minimum
 * balance is less than or equal to Rs. 1000.
 */
void Account::withdrawAmount() {
    double amount;
    std::cout << "Enter amount to be withdrawn: ";
getAmount:
    std::cin >> amount;
    std::cin.ignore();
    if (accountType == "Savings" && (balance - amount) <= 1000) {
        std::cout << "Savings account must have at least a minimum balance of "
                     "Rs. 1000!"
                  << '\n';
        std::cout << "Please re-enter amount to be withdrawn: ";
        goto getAmount;
    } else if (amount > balance) {
        std::cout << "Insufficient credits!" << '\n';
        std::cout << "Please re-enter amount to be withdrawn: ";
        goto getAmount;
    } else if (amount < 0) {
        std::cout << "Please re-enter a valid amount to be withdrawn: ";
        goto getAmount;
    }
    std::cout << "Enter password: ";
getPassword:
    bool confirmation = compareHash();
    if (!confirmation) {
        std::cout << "Incorrect password!" << '\n';
        std::cout << "Please re-enter password: ";
        goto getPassword;
    }
    std::cout << "Rs. " << amount << " withdrawn successfully." << '\n';
    balance -= amount;
    std::cout << "Current balance: " << balance << '\n';
    sleep;
}

/**
 * Used to withdraw any remaining balance
 * when account is being deleted.
 */
void Account::withdrawAmount(double lastBalance) {
    balance -= lastBalance;
    std::cout << "Remaining balance Rs. " << lastBalance
              << " withdrawn successfully." << '\n';
    sleep;
}

/**
 * Used to change the password for the
 * current account object.
 */
void Account::changePassword() {
    std::cout << "Enter new password: ";
getPassword:
    generateHash();
    std::cout << "Confirm password: ";
    if (!compareHash()) {
        std::cout << "Passwords don't match!" << '\n';
        std::cout << "Please re-enter password: ";
        goto getPassword;
    }
}
