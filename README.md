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
Or with Clang:
bashclang++ -std=c++20 -Wall -Wextra -o guess_number guess_number.cpp
Windows (MSVC — Developer Command Prompt)
cmdcl /std:c++20 /W4 /EHsc guess_number.cpp /Fe:guess_number.exe
Windows (MinGW / MSYS2)
bashg++ -std=c++20 -Wall -Wextra -o guess_number.exe guess_number.cpp

Note: -std=c++17 works as well if your toolchain does not yet support
C++20.


Running
bash./guess_number          # Linux / macOS
guess_number.exe        # Windows
The game runs entirely in the terminal. No graphical interface, no
installation step — just compile and run.



Modern C++ Features Used
The source code is heavily commented to explain why each modern feature is
used instead of the older approach.
FeatureWhereWhy it is betterstd::vector<int>Guess historyGrows automatically; no fixed-size arraysstd::optional<int>readGuess() return typeExplicit "no value" without magic numbersstd::mt19937 + std::uniform_int_distributionNumberGeneratorBetter randomness than rand()std::unique_ptrGenerator ownership in GameAutomatic memory management; no new/deleteenum class GameStateRound state trackingScoped, type-safe; no name collisionsconstexprGameConfig::MinValue/MaxValueCompile-time constants with proper typesRange-based forPrinting guess historyCleaner and less error-prone than index loopsautoLocal variable type deductionLess repetition, easier to refactor

Project Structure
guess_number.cpp   ← entire source, one self-contained file
README.md          ← this file

