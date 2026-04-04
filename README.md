# Guess-the-nummber

Guess the Number
A terminal-based number guessing game written in modern C++17/20, designed as a
learning example for complete beginners.

Overview
The computer picks a random number between 1 and 100. You guess it. After every
guess the game tells you whether your guess was too high or too low. When you
finally get it right, the game shows how many attempts you took and a full
history of your guesses, then asks whether you want to play again.

Requirements
RequirementMinimum versionC++ standardC++17 (C++20 recommended)CompilerGCC 9+, Clang 10+, or MSVC 19.14+Build systemAny (plain g++ / clang++ works fine)Operating systemLinux, macOS, or Windows (terminal required)
No external libraries or package manager is needed. The code uses only the
C++ Standard Library.

Building
Linux / macOS
bashg++ -std=c++20 -Wall -Wextra -o guess_number guess_number.cpp

Windows (MinGW / MSYS2)
bashg++ -std=c++20 -Wall -Wextra -o guess_number.exe guess_number.cpp

Note: -std=c++17 works as well if your toolchain does not yet support
C++20.



