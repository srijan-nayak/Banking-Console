/**
 * main.cpp
 * Banking Console
 *
 * Created by Srijan Nayak on 16/04/2018.
 * Copyright © 2018 Srijan Nayak, Shyle Shaju. All rights reserved.
 */

#include "Account.hpp"

std::string masterKeyHash = // Hash of master key for exiting the console.
    "491203c64506797dc07ce337d86f0355b0660df01f12c6b5de0304aebbc01fbd";

/**
 * Displays the splash screen briefly when
 * the console is launched.
 */
void splashScreen() {
    std::cout << '\n';
    std::cout << '\n';
    std::cout << "                       Banking Console®" << '\n';
    std::cout << "                         ver. 1.0.1" << '\n';
    std::cout << '\n';
    std::cout << "                   Developers:- Srijan Nayak" << '\n';
    std::cout << "                                Shyle Shaju" << '\n';
    sleep;
    std::cout.flush();
}

/**
 * Displays the banking console main menu
 * and prompts the user to enter the option
 * number.
 */
void menuDisplay() {
    std::cout << '\n';
    std::cout << "  1. Open Account     5. Display All Account Information"
              << '\n';
    std::cout << "  2. Deposit Amount   6. Transfer Amount" << '\n';
    std::cout << "  3. Withdraw Amount  7. Delete Account" << '\n';
    std::cout << "  4. Forgot Password" << '\n';
    std::cout << '\n';
    std::cout << "Enter preferred option number: ";
    std::cout.flush();
}

int main() {
    Account a;
    Account a1;
    std::fstream file;
    std::fstream file1;
    char choice;
    char temp[25];
    std::string temp1;
    long pos = 0;
    long pos1 = 0;
    double amount;
    bool foundFlag = false;
    bool foundFlag1 = false;
    bool deletionConfirmation = false;
    const int ACCOUNT_CODE = 1000;

    clearScreen;
    splashScreen();
    while (true) {
        clearScreen;
        menuDisplay();
        std::cin >> choice;
        clearScreen;

        switch (choice) {
        case 'c': // Case for hidden menu option 'Reset Account Code Counter'.
            file.open("data/accCode.dat", std::ios::binary | std::ios::out);
            file.write((char *)&ACCOUNT_CODE, sizeof(ACCOUNT_CODE));
            file.close();
            break;

        case '0': // Case for hidden menu option 'Exit Banking Console'.
            std::cout << "Enter master key to exit the console: ";
            setStdinEcho(false);
            std::cin.ignore();
            std::cin.getline(temp, 25);
            setStdinEcho(true);
            std::cout << '\n';
            if (sha256(temp) == masterKeyHash) {
                clearScreen;
                return 0;
            } else {
                std::cout << "Incorrect password!" << '\n';
            }
            break;

        case '1': // Case for menu option 'Open Account'.
            file.open("../data/records.dat",
                      std::ios::binary | std::ios::out | std::ios::app);
            a.inputAccountDetails();
            file.write((char *)&a, sizeof(a));
            file.close();
            break;

        case '2': // Case for menu option 'Deposit Amount'.
            file.open("../data/records.dat",
                      std::ios::binary | std::ios::in | std::ios::out);
            std::cout << "Enter account number: ";
            std::cin.ignore();
            std::getline(std::cin, temp1);
            while (file) {
                pos = file.tellg();
                file.read((char *)&a, sizeof(a));
                if (temp1 == a.getAccountNumber()) {
                    a.depositAmount();
                    file.seekp(pos);
                    file.write((char *)&a, sizeof(a));
                    foundFlag = true;
                    break;
                }
            }
            if (!foundFlag) {
                std::cout << "No account with that account number!" << '\n';
                sleep;
            }
            file.close();
            break;

        case '3': // Case for menu option 'Withdraw Amount'.
            file.open("../data/records.dat",
                      std::ios::binary | std::ios::in | std::ios::out);
            std::cout << "Enter account number: ";
            std::cin.ignore();
            std::getline(std::cin, temp1);
            while (file) {
                pos = file.tellg();
                file.read((char *)&a, sizeof(a));
                if (temp1 == a.getAccountNumber()) {
                    a.withdrawAmount();
                    file.seekp(pos);
                    file.write((char *)&a, sizeof(a));
                    foundFlag = true;
                    break;
                }
            }
            if (!foundFlag) {
                std::cout << "No account with that account number!" << '\n';
                sleep;
            }
            file.close();
            break;

        case '4': // Case for menu option "Forgot Password".
            file.open("../data/records.dat",
                      std::ios::binary | std::ios::in | std::ios::out);
            std::cout << "Enter account number: ";
            std::cin.ignore();
            std::getline(std::cin, temp1);
            while (file) {
                pos = file.tellg();
                file.read((char *)&a, sizeof(a));
                if (temp1 == a.getAccountNumber()) {
                    foundFlag = true;
                    std::cout
                        << "Enter phone number associated with the account: ";
                    std::getline(std::cin, temp1);
                    if (temp1 == a.getPhoneNumber()) {
                        std::cout << "Enter email address associated with the "
                                     "account: ";
                        std::getline(std::cin, temp1);
                        if (temp1 == a.getEmail()) {
                            std::cout << "Enter date of birth: ";
                            std::cin.getline(temp, 11);
                            if (strcmp(temp, a.getDateOfBirth()) == 0) {
                                a.changePassword();
                                file.seekp(pos);
                                file.write((char *)&a, sizeof(a));
                                foundFlag1 = true;
                            }
                        }
                    }
                    if (!foundFlag1) {
                        std::cout << "Incorrect detail entered!" << '\n';
                        sleep;
                    }
                    break;
                }
            }
            if (!foundFlag) {
                std::cout << "No account with that account number!" << '\n';
                sleep;
            }
            file.close();
            break;

        case '5': // Case for menu option 'Display All Account Information'.
            file.open("../data/records.dat", std::ios::binary | std::ios::in);
            std::cout << "Enter account number: ";
            std::cin.ignore();
            std::getline(std::cin, temp1);
            while (file.read((char *)&a, sizeof(a))) {
                if (temp1 == a.getAccountNumber()) {
                    foundFlag = true;
                    std::cout << "Enter password: ";
                    if (a.compareHash()) {
                        clearScreen;
                        a.displayAccountDetails();
                        break;
                    } else {
                        std::cout << "Incorrect password!" << '\n';
                        sleep;
                        break;
                    }
                }
            }
            if (!foundFlag) {
                std::cout << "No account with that account number!" << '\n';
                sleep;
            }
            file.close();
            break;

        case '6': // Case for menu option 'Transfer Account'.
            file.open("../data/records.dat",
                      std::ios::binary | std::ios::in | std::ios::out);
            std::cout << "Enter your account number: ";
            std::cin.ignore();
            std::getline(std::cin, temp1);
            while (file) {
                pos = file.tellg();
                file.read((char *)&a, sizeof(a));
                if (temp1 == a.getAccountNumber()) {
                    foundFlag = true;
                    std::cout << "Enter password: ";
                    if (a.compareHash()) {
                        break;
                    } else {
                        std::cout << "Incorrect password!" << '\n';
                        sleep;
                        break;
                    }
                }
            }
            if (!foundFlag) {
                std::cout << "No account with that account number!" << '\n';
                sleep;
            } else {
                std::cout << "Enter recipient's account number: ";
                std::getline(std::cin, temp1);
                file.seekg(file.beg);
                while (file) {
                    pos1 = file.tellg();
                    file.read((char *)&a1, sizeof(a1));
                    if (temp1 == a1.getAccountNumber()) {
                        foundFlag1 = true;
                        std::cout << "Enter amount to be transferred: ";
                    getAmount:
                        std::cin >> amount;
                        if (amount >= a.balance) {
                            std::cout << "Insufficient credits!" << '\n';
                            std::cout << "Enter amount again: " << '\n';
                            goto getAmount;
                        } else {
                            a1.balance += amount;
                            a.balance -= amount;
                            std::cout << "Rs." << amount
                                      << " transferred successfully to "
                                      << temp1 << '\n';
                            file.seekp(pos);
                            file.write((char *)&a, sizeof(a));
                            file.seekp(pos1);
                            file.write((char *)&a1, sizeof(a1));
                            sleep;
                            break;
                        }
                    }
                }
                if (!foundFlag1) {
                    std::cout << "No account with that account number!" << '\n';
                    sleep;
                }
            }
            file.close();
            break;

        case '7': // Case for menu option 'Delete Account'.
            file.open("../data/records.dat", std::ios::binary | std::ios::in);
            file1.open("data/temp.dat", std::ios::binary | std::ios::out);
            std::cout << "Enter account number: ";
            std::cin.ignore();
            std::getline(std::cin, temp1);
            while (file.read((char *)&a, sizeof(a))) {
                if (temp1 == a.getAccountNumber()) {
                    foundFlag = true;
                    std::cout << "Enter password to confirm deletion of your "
                                 "account: ";
                    if (a.compareHash()) {
                        a.withdrawAmount(a.balance);
                        file.seekg(file.beg);
                        while (file.read((char *)&a, sizeof(a))) {
                            if (temp1 == a.getAccountNumber()) {
                                std::cout << "Account successfully deleted!"
                                          << '\n';
                                sleep;
                            } else {
                                file1.write((char *)&a, sizeof(a));
                            }
                            deletionConfirmation = true;
                        }
                        break;
                    } else {
                        std::cout << "Incorrect password!" << '\n';
                        sleep;
                        break;
                    }
                }
            }
            if (!foundFlag) {
                std::cout << "No account with that account number!" << '\n';
                sleep;
            }
            if (deletionConfirmation) {
                remove("../data/records.dat");
                rename("data/temp.dat", "../data/records.dat");
            } else {
                remove("data/temp.dat");
            }
            file.close();
            file1.close();
            break;

        default:
            std::cout << "Invalid choice!" << '\n';
            sleep;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
        foundFlag = false;  // Resets the state of all boolean check flags to
        foundFlag1 = false; // false.
        deletionConfirmation = false;
    }
}
