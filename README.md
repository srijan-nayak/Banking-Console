# Banking Console
A simple banking console simulator developed for a school project.
![Console Screenshot](https://github.com/srijan-nayak/Banking-Console/blob/master/screenshots/Screenshot%202018-11-11%20at%205.04.42%20PM.png?raw=true)
###### For macOS and Linux only.

## Features
* Open account
* Display account details
* Password reset
* Deposit amount
* Withdraw amount
* Transfer amount
* Delete account
* sha256 hash generator for secure storage of passwords
* Hidden password input to protect from password peeking

## How to Compile
Once in the "Banking Console" directory, the project can be compiled via clang or gcc compiler from the terminal.

```bash
clang++ -std=c++14 src/main.cpp src/Account.hpp src/Account.cpp src/sha256.hpp src/sha256.cpp
```
**OR**
```bash
g++ -std=c++14 src/main.cpp src/Account.hpp src/Account.cpp src/sha256.hpp src/sha256.cpp
```

## How to Run
After compiling, from the same directory, run the following command.
```bash
./a.out
```
After the application shows the main menu, enter 'c' to generate or reset the accCode.dat file.

**NOTE:** The above step **must be** followed on the first run of the application before doing anything else.

## How to Exit
The console can be force-quitted by closing the terminal, but for simulation purposes, the application be terminated by entering 0 in the main menu, and then entering '30092015' as the password.

To change the master key, simply replace the masterKeyHash in src/main.cpp with your own sha256 hash.
```c++
std::string masterKeyHash = // Hash of master key for exiting the console.
    "491203c64506797dc07ce337d86f0355b0660df01f12c6b5de0304aebbc01fbd";
```
You can generate your own sha256 hash from [here](https://passwordsgenerator.net/sha256-hash-generator/).
