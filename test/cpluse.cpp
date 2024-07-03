#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // Include for remove_if function
#include <cctype>    // Include for ispunct function

// Function to count word frequency in a given text
std::unordered_map<std::string, int> countWordFrequency(const std::string& text) {
    std::unordered_map<std::string, int> frequencyMap;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        // Remove punctuation from the word (if any)
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end()); // Updated to ::ispunct
        // Increment the frequency of the word in the map
        frequencyMap[word]++;
    }

    return frequencyMap;
}

int main() {
    std::string text = "This is a simple example. We are counting word frequency in a given text. "
                       "This example demonstrates the use of hash table.";

    // Count word frequency
    std::unordered_map<std::string, int> wordFrequency = countWordFrequency(text);

    // Display word frequency
    std::cout << "Word Frequency:\n";
    for (const auto& pair : wordFrequency) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}

