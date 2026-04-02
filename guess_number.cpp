// =============================================================================
// GUESS THE NUMBER — Modern C++17/20 example for beginners
// =============================================================================
// This file intentionally demonstrates MODERN approaches and explains why they
// are better than older C++ or C-style ways of writing code.
// =============================================================================

#include <iostream>   // std::cout, std::cin
#include <vector>     // std::vector (dynamic array — replaces C-style int[])
#include <optional>   // std::optional (a value that "might exist")
#include <random>     // std::random_device, std::mt19937, std::uniform_int_distribution
#include <memory>     // std::unique_ptr (smart pointer — replaces new/delete)
#include <string>     // std::string
#include <limits>     // std::numeric_limits (for clearing the input buffer)

// =============================================================================
// enum class: Game states
// =============================================================================
// Why enum class instead of old-style enum?
//   Old: enum State { PLAYING, WON };  → State is in the global namespace
//        → name collisions are possible, e.g. WON might clash with a macro
//   New: enum class GameState { Playing, Won };  → access only via GameState::Playing
//        → safe, readable, no collisions
// =============================================================================
enum class GameState {
    Playing,   // A round is currently in progress
    Won,       // The player guessed the number
    Quit       // The player decided to stop
};

// =============================================================================
// GameConfig — game configuration
// =============================================================================
// constexpr: the value is computed at COMPILE TIME, not at runtime.
// Why? Faster and safer than #define MIN_VALUE 1 (macros have no type!).
// =============================================================================
struct GameConfig {
    static constexpr int MinValue = 1;    // Lowest possible number
    static constexpr int MaxValue = 100;  // Highest possible number
};

// =============================================================================
// NumberGenerator — random number generator
// =============================================================================
class NumberGenerator {
public:
    // Constructor accepts the desired range
    NumberGenerator(int min, int max)
        // std::random_device: a "true" source of randomness (hardware entropy)
        // std::mt19937: Mersenne Twister — a high-quality pseudo-random generator
        // Why not rand()? rand() is deprecated, has poor distribution, and is
        //                  not thread-safe
        : rng_(std::random_device{}()),
          distribution_(min, max)
    {}

    // Generates a random number within the configured range
    int generate() {
        return distribution_(rng_);
    }

private:
    std::mt19937 rng_;                               // Randomness engine
    std::uniform_int_distribution<int> distribution_; // Uniform distribution
};

// =============================================================================
// GuessGame — logic for a single round
// =============================================================================
class GuessGame {
public:
    // explicit: prevents accidental implicit conversion of an int to a GuessGame
    explicit GuessGame(int secretNumber)
        : secretNumber_(secretNumber),
          state_(GameState::Playing)
    {
        // std::vector: dynamic array — grows automatically, no need to know the
        //              size in advance.
        // Why not int guesses[100]? Arrays have a fixed size and don't track
        //                           how many elements are actually used.
        guesses_.reserve(10); // Optimisation: pre-allocate space for ~10 guesses
    }

    // Processes the player's guess
    // Returns the current GameState after the guess
    GameState processGuess(int guess) {
        // Save the guess to history
        guesses_.push_back(guess);

        if (guess < secretNumber_) {
            std::cout << "  ^ Too low! Try a higher number.\n";
        } else if (guess > secretNumber_) {
            std::cout << "  v Too high! Try a lower number.\n";
        } else {
            state_ = GameState::Won;
            std::cout << "  * Correct! You guessed the number " << secretNumber_ << "!\n";
        }

        return state_;
    }

    // Returns the guess history as a const reference
    // const std::vector<int>&: no copy is made (efficient), data cannot be modified
    const std::vector<int>& getGuesses() const {
        return guesses_;
    }

    // Returns the number of attempts made
    int getAttemptCount() const {
        // auto: the compiler infers the type (here size_t → cast to int)
        return static_cast<int>(guesses_.size());
    }

    GameState getState() const {
        return state_;
    }

    int getSecretNumber() const {
        return secretNumber_;
    }

private:
    const int secretNumber_; // Secret number — const: cannot change after initialisation
    GameState state_;
    std::vector<int> guesses_; // Guess history
};

// =============================================================================
// Game — controls the entire program flow (menu, game loop, statistics)
// =============================================================================
class Game {
public:
    Game()
        // std::make_unique: creates an object on the heap and returns a smart pointer.
        // Why not NumberGenerator* gen = new NumberGenerator(...)?
        //   → with new we must manually call delete → risk of memory leaks
        //   → unique_ptr is automatically destroyed when it goes out of scope
        : generator_(std::make_unique<NumberGenerator>(
              GameConfig::MinValue,
              GameConfig::MaxValue))
    {}

    // Main game loop — plays rounds until the player wants to quit
    void run() {
        printWelcome();

        bool playAgain = true;
        while (playAgain) {
            playRound();
            playAgain = askPlayAgain();
        }

        std::cout << "\nThanks for playing! Goodbye.\n";
    }

private:
    // std::unique_ptr: owns the pointer to the generator
    std::unique_ptr<NumberGenerator> generator_;

    // Plays a single round
    void playRound() {
        // Create a new game with a fresh random number
        // auto: compiler infers the type as 'GuessGame'
        auto game = GuessGame(generator_->generate());

        std::cout << "\n--- New Game ---\n";
        std::cout << "Guess a number between "
                  << GameConfig::MinValue << " and " << GameConfig::MaxValue << ".\n\n";

        // Game loop — repeats until the player wins
        while (game.getState() == GameState::Playing) {
            // std::optional<int>: the function can return a number OR "nothing"
            //                     (on invalid input)
            // Why not int with a magic value like -1? optional is explicit and safe.
            auto input = readGuess();

            if (!input.has_value()) {
                std::cout << "  ! Invalid input. Please enter a whole number.\n";
                continue;
            }

            int guess = input.value();

            if (guess < GameConfig::MinValue || guess > GameConfig::MaxValue) {
                std::cout << "  ! Please enter a number between "
                          << GameConfig::MinValue << " and " << GameConfig::MaxValue << ".\n";
                continue;
            }

            game.processGuess(guess);
        }

        // Display round results
        printResults(game);
    }

    // Reads the player's guess — returns optional (a value or "nothing")
    std::optional<int> readGuess() {
        std::cout << "Your guess: ";
        int value{};

        if (std::cin >> value) {
            return value; // Success: return the number wrapped in optional
        }

        // Bad input (e.g. the player typed a letter instead of a number)
        // Reset cin state and flush the buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt; // "Nothing" — no valid number was entered
    }

    // Displays the results of a round
    void printResults(const GuessGame& game) {
        std::cout << "\n=== Round Results ===\n";
        std::cout << "Attempts: " << game.getAttemptCount() << "\n";
        std::cout << "Your guesses: ";

        // Range-based for loop: modern, more readable than a classic index-based for
        // const auto&: we read values without copying them
        for (const auto& guess : game.getGuesses()) {
            std::cout << guess << " ";
        }
        std::cout << "\n";

        // Performance rating
        const int attempts = game.getAttemptCount();
        if (attempts <= 3) {
            std::cout << "Rating: Outstanding! You're a natural!\n";
        } else if (attempts <= 6) {
            std::cout << "Rating: Great job! Well done.\n";
        } else if (attempts <= 10) {
            std::cout << "Rating: Not bad! Keep practising.\n";
        } else {
            std::cout << "Rating: Better luck next time! You'll get it.\n";
        }
    }

    // Asks whether the player wants to play again
    bool askPlayAgain() {
        std::cout << "\nPlay again? (y/n): ";
        std::string answer;

        // Skip any leftover newline from the previous line
        if (std::cin.peek() == '\n') {
            std::cin.ignore();
        }

        std::getline(std::cin, answer);

        // Accept 'y', 'Y', 'yes', 'Yes', etc.
        return !answer.empty() && (answer[0] == 'y' || answer[0] == 'Y');
    }

    // Welcome screen
    void printWelcome() const {
        std::cout << "+----------------------------------+\n";
        std::cout << "|    GUESS THE NUMBER  (C++20)     |\n";
        std::cout << "|  Guess a number from 1 to 100!   |\n";
        std::cout << "+----------------------------------+\n";
    }
};

// =============================================================================
// main() — program entry point
// =============================================================================
// Intentionally minimal: all logic lives inside the Game and GuessGame classes.
// main() just creates a Game object and starts it.
// =============================================================================
int main() {
    // Game lives on the stack — automatically destroyed when main() returns.
    // Internally, Game holds a unique_ptr that manages the generator on the heap.
    Game game;
    game.run();

    return 0; // 0 = program finished without errors
}
