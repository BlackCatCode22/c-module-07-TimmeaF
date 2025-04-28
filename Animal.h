#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
#include <iostream>
#include <ctime> // for time operations
#include <random> // for random
#include <utility>

class Animal {
protected:
    static int hyenaCounter;
    static int lionCounter;
    static int tigerCounter;
    static int bearCounter;

    std::string id;  // Unique ID for each animal (formatted as "Hy1", "Li1", etc.)
    std::string name;
    int age;
    std::string species;
    std::string birthSeason;
    std::string color;
    std::string gender;
    double weight;
    std::string origin;

    // Random engine and distribution
    static std::mt19937 rng;  // Mersenne Twister RNG
    static std::uniform_int_distribution<int> dist;

public:
    // Constructor
    Animal(std::string n, int a, std::string s, std::string bSeason,
           std::string c, std::string g, double w, std::string o)
        : name(std::move(n)), age(a), species(std::move(s)), birthSeason(std::move(bSeason)), color(std::move(c)), gender(std::move(g)), weight(w), origin(std::move(o)) {
        // Increment species-specific counter and assign the ID
        id = std::to_string(generateUniqueId(species));  // Assign ID during object construction
    }

    // Getter functions
    [[nodiscard]] std::string getId() const { return id; }
    [[nodiscard]] std::string getName() const { return name; }
    [[nodiscard]] int getAge() const { return age; }
    [[nodiscard]] std::string getSpecies() const { return species; }
    [[nodiscard]] std::string getBirthSeason() const { return birthSeason; }
    [[nodiscard]] std::string getColor() const { return color; }
    [[nodiscard]] std::string getGender() const { return gender; }
    [[nodiscard]] double getWeight() const { return weight; }
    [[nodiscard]] std::string getOrigin() const { return origin; }

    [[nodiscard]] virtual std::string getHabitat() const = 0;

    // Calculate birthdate
    std::string genBirthDay() const {
        // Get current year
        time_t now = time(nullptr);
        tm *ltm = localtime(&now);
        int year = 1900 + ltm->tm_year;

        // Adjust the year based on the animal's birth season and current month
        if (ltm->tm_mon < getSeasonMonth(birthSeason)) {
            year -= age + 1;  // Animal hasn't had its birthday yet this year
        } else {
            year -= age;  // Animal had its birthday earlier this year
        }

        // Get a random day of the month for the birth season
        int day = getRandomDayInSeason(birthSeason);

        // Return the birthdate in ISO 8601 format (YYYY-MM-DD)
        return std::to_string(year) + "-" + std::to_string(getSeasonMonth(birthSeason)) + "-" + std::to_string(day);
    }

    virtual ~Animal() = default;

private:
    // Generate unique ID for each species with a species-specific counter
    static int generateUniqueId(const std::string& species) {
        if (species == "Hyena") {
            return 1000 + ++hyenaCounter;  // Add species-specific offset for uniqueness
        } else if (species == "Lion") {
            return 2000 + ++lionCounter;  // Add species-specific offset for uniqueness
        } else if (species == "Tiger") {
            return 3000 + ++tigerCounter;  // Add species-specific offset for uniqueness
        } else if (species == "Bear") {
            return 4000 + ++bearCounter;  // Add species-specific offset for uniqueness
        }
        return -1; // Default return if species doesn't match
    }

    // Helper function to get the month corresponding to the season
    static int getSeasonMonth(const std::string& season) {
        if (season == "spring") return 3;  // March
        if (season == "summer") return 6;  // June
        if (season == "fall") return 9;    // September
        if (season == "winter") return 12; // December
        return 1;  // Default to January if invalid
    }

    // Helper function to get a random day in the specified season using C++11 <random>
    static int getRandomDayInSeason(const std::string& season) {
        if (season == "spring") return dist(rng) % 30 + 1;  // Random day between 1 and 30
        if (season == "summer") return dist(rng) % 31 + 1;  // Random day between 1 and 31
        if (season == "fall") return dist(rng) % 30 + 1;    // Random day between 1 and 30
        if (season == "winter") return dist(rng) % 31 + 1;   // Random day between 1 and 31
        return 1;  // Default to day 1 if invalid
    }
};

// Initialize the random number generator and distribution
std::mt19937 Animal::rng(std::random_device{}()); // Mersenne Twister engine seeded with random device
std::uniform_int_distribution<int> Animal::dist(1, 31); // Uniform distribution for days (1 to 31)

// Static counters for each species
int Animal::hyenaCounter = 0;
int Animal::lionCounter = 0;
int Animal::tigerCounter = 0;
int Animal::bearCounter = 0;

// Hyena class
class Hyena : public Animal {
public:
    Hyena(const std::string& n, int a, const std::string& bSeason, const std::string& c,
          const std::string& g, double w, const std::string& o)
        : Animal(n, a, "Hyena", bSeason, c, g, w, o) {}

    [[nodiscard]] std::string getHabitat() const override {
        return "Savannas, grasslands, and woodlands in Africa";
    }
};

// Lion class
class Lion : public Animal {
public:
    Lion(const std::string& n, int a, const std::string& bSeason, const std::string& c,
         const std::string& g, double w, const std::string& o)
        : Animal(n, a, "Lion", bSeason, c, g, w, o) {}

    [[nodiscard]] std::string getHabitat() const override {
        return "Grasslands, savannas, and open woodlands in Africa and India";
    }
};

// Bear class
class Bear : public Animal {
public:
    Bear(const std::string& n, int a, const std::string& bSeason, const std::string& c,
         const std::string& g, double w, const std::string& o)
        : Animal(n, a, "Bear", bSeason, c, g, w, o) {}

    [[nodiscard]] std::string getHabitat() const override {
        return "Forests, mountains, and tundras in North America, Europe, and Asia";
    }
};

// Tiger class
class Tiger : public Animal {
public:
    Tiger(const std::string& n, int a, const std::string& bSeason, const std::string& c,
          const std::string& g, double w, const std::string& o)
        : Animal(n, a, "Tiger", bSeason, c, g, w, o) {}

    [[nodiscard]] std::string getHabitat() const override {
        return "Tropical forests, grasslands, and mangrove swamps in Asia";
    }
};

#endif // ANIMAL_H
