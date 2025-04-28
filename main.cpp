#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include <memory>  // Include for unique_ptr
#include <ctime>   // Include for getting current year

#include "animal.h"

using namespace std;

// Helper function to get the current year
int getCurrentYear() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_year + 1900; // tm_year gives years since 1900
}

// Function to read names from a file and categorize them by species
void readNamesFromFile(const string& filename, vector<string>& hyenaNames, vector<string>& lionNames,
                       vector<string>& bearNames, vector<string>& tigerNames) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open " << filename << " for reading" << endl;
        return;
    }

    string line, currentSpecies;

    while (getline(file, line)) {
        if (line.empty() || line.find("Names:") != string::npos) {
            if (line.find("Hyena Names") != string::npos) currentSpecies = "Hyena";
            else if (line.find("Lion Names") != string::npos) currentSpecies = "Lion";
            else if (line.find("Bear Names") != string::npos) currentSpecies = "Bear";
            else if (line.find("Tiger Names") != string::npos) currentSpecies = "Tiger";
        } else {
            line = line.substr(0, line.find_last_not_of(" \n\r\t") + 1);
            if (currentSpecies == "Hyena") hyenaNames.push_back(line);
            else if (currentSpecies == "Lion") lionNames.push_back(line);
            else if (currentSpecies == "Bear") bearNames.push_back(line);
            else if (currentSpecies == "Tiger") tigerNames.push_back(line);
        }
    }

    // Debug: Print tiger names to verify
    cout << "Tiger Names loaded: ";
    for (const auto& name : tigerNames) {
        cout << name << " ";
    }
    cout << endl;
}

// Function to read and parse data from a file
void readAnimalsFromFile(const string& filename, vector<unique_ptr<Animal>>& animals,
                         vector<string>& hyenaNames, vector<string>& lionNames,
                         vector<string>& bearNames, vector<string>& tigerNames) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Could not open " << filename << " for reading" << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());

    string line;
    regex rgx(R"((\d+) year old (\w+) (hyena|lion|bear|tiger), born in (spring|summer|fall|winter|unknown), ([A-Za-z]+(?: [A-Za-z]+)*) color, (\d+) pounds, from (.+))");

    while (getline(inFile, line)) {
        cout << "Reading line: " << line << endl;
        smatch matches;

        if (regex_search(line, matches, rgx)) {
            string species = matches[3];
            cout << "Extracted species: " << species << endl;

            string name = "Unnamed";
            vector<string>* speciesNames = nullptr;

            if (species == "hyena") speciesNames = &hyenaNames;
            else if (species == "lion") speciesNames = &lionNames;
            else if (species == "bear") speciesNames = &bearNames;
            else if (species == "tiger") speciesNames = &tigerNames;

            if (speciesNames && !speciesNames->empty()) {
                uniform_int_distribution<size_t> dis(0, speciesNames->size() - 1);
                size_t idx = dis(gen);

                name = (*speciesNames)[idx];
                (*speciesNames)[idx] = speciesNames->back();
                speciesNames->pop_back();
            }

            int age = stoi(matches[1]);
            string gender = matches[2];
            string birthSeason = matches[4];
            string color = matches[5];
            double weight = stod(matches[6]);
            string origin = matches[7];

            if (species == "hyena") animals.push_back(make_unique<Hyena>(name, age, birthSeason, color, gender, weight, origin));
            else if (species == "lion") animals.push_back(make_unique<Lion>(name, age, birthSeason, color, gender, weight, origin));
            else if (species == "tiger") animals.push_back(make_unique<Tiger>(name, age, birthSeason, color, gender, weight, origin));
            else if (species == "bear") animals.push_back(make_unique<Bear>(name, age, birthSeason, color, gender, weight, origin));
        }
    }
}

// Function to generate the report for all animals and write it to a file
void generateReportToFile(const vector<unique_ptr<Animal>>& animals, const string& outputFilename) {
    ofstream outFile(outputFilename);
    if (!outFile) {
        cerr << "Error: Could not open " << outputFilename << " for writing" << endl;
        return;
    }

    int currentYear = getCurrentYear();

    auto calculateBirthDate = [&](const Animal& animal) {
        int birthYear = currentYear - animal.getAge();
        string season = animal.getBirthSeason();
        string monthDay;

        if (season == "spring") monthDay = "04-01";
        else if (season == "summer") monthDay = "07-01";
        else if (season == "fall") monthDay = "10-01";
        else if (season == "winter") monthDay = "01-01";
        else monthDay = "06-01"; // unknown

        return to_string(birthYear) + "-" + monthDay;
    };

    int hyenaCounter = 1;
    int lionCounter = 1;
    int tigerCounter = 1;
    int bearCounter = 1;

    outFile << "Hyena Habitat:" << endl;
    for (const auto& animal : animals) {
        if (animal->getSpecies() == "Hyena") {
            string id = "Hy" + to_string(hyenaCounter++);
            string birthDate = calculateBirthDate(*animal);

            outFile << id << "; " << animal->getName() << "; age "
                    << animal->getAge() << "; birth date " << birthDate << "; "
                    << animal->getColor() << " color; "
                    << animal->getGender() << "; " << animal->getWeight() << " pounds; from "
                    << animal->getOrigin() << endl;
        }
    }

    outFile << "Lion Habitat:" << endl;
    for (const auto& animal : animals) {
        if (animal->getSpecies() == "Lion") {
            string id = "Li" + to_string(lionCounter++);
            string birthDate = calculateBirthDate(*animal);

            outFile << id << "; " << animal->getName() << "; age "
                    << animal->getAge() << "; birth date " << birthDate << "; "
                    << animal->getColor() << " color; "
                    << animal->getGender() << "; " << animal->getWeight() << " pounds; from "
                    << animal->getOrigin() << endl;
        }
    }

    outFile << "Tiger Habitat:" << endl;
    for (const auto& animal : animals) {
        if (animal->getSpecies() == "Tiger") {
            string id = "Ti" + to_string(tigerCounter++);
            string birthDate = calculateBirthDate(*animal);

            outFile << id << "; " << animal->getName() << "; age "
                    << animal->getAge() << "; birth date " << birthDate << "; "
                    << animal->getColor() << " color; "
                    << animal->getGender() << "; " << animal->getWeight() << " pounds; from "
                    << animal->getOrigin() << endl;
        }
    }

    outFile << "Bear Habitat:" << endl;
    for (const auto& animal : animals) {
        if (animal->getSpecies() == "Bear") {
            string id = "Be" + to_string(bearCounter++);
            string birthDate = calculateBirthDate(*animal);

            outFile << id << "; " << animal->getName() << "; age "
                    << animal->getAge() << "; birth date " << birthDate << "; "
                    << animal->getColor() << " color; "
                    << animal->getGender() << "; " << animal->getWeight() << " pounds; from "
                    << animal->getOrigin() << endl;
        }
    }

    outFile << "-----------------------------" << endl;
}

// Main function
int main() {
    vector<unique_ptr<Animal>> animals;
    vector<string> hyenaNames, lionNames, bearNames, tigerNames;

    // Reading names and assigning them to the species
    readNamesFromFile("C:/Users/NCG/CLionProjects/zoo/cmake-build-debug/animalNames.txt", hyenaNames, lionNames, bearNames, tigerNames);

    // Reading animal data and assigning names
    readAnimalsFromFile("C:/Users/NCG/CLionProjects/zoo/cmake-build-debug/arrivingAnimals.txt", animals, hyenaNames, lionNames, bearNames, tigerNames);

    // Generating report
    generateReportToFile(animals, "C:/Users/NCG/CLionProjects/zoo/cmake-build-debug/zooPopulation.txt");

    return 0;
}
