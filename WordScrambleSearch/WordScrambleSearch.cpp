
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <regex>
#include <iosfwd>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <array>

// Search directions
const std::vector<std::pair<int, int>> directions = {
	{0, 1}, {0, -1},					// R, L				// right, left
	{1, 0}, {1, 1}, {1, -1},			// D, RD, LD		// down, right-down, left-down
	{-1, 0}, {-1, -1}, {-1, 1} 			// U, LU, RU		// up, left-up, right-up
};

bool searchWord(const std::vector<std::string>& matrix, const std::string& word, int row, int col, int dir) {

	// Loop every character of the word
	for (int i = 0; i < word.length(); ++i) {

		// Set position for checking
		int newRow = row + i * directions[dir].first;
		int newCol = col + i * directions[dir].second;

		// Check out of bounds of matrix
		if (newRow < 0 ||
			newRow >= matrix.size() ||
			newCol < 0 ||
			newCol >= matrix[0].size() ||

			// Finally check if not matching current word
			std::toupper(matrix[newRow][newCol]) != std::toupper(word[i])
			) {
			return false;
		}
	}
	return true;
}

bool iterate_every_position_and_every_direction_in_matrix(const std::vector<std::string>& matrix, const std::string& word) {
	for (int row = 0; row < matrix.size(); ++row) {
		for (int col = 0; col < matrix[0].size(); ++col) {
			for (int dir = 0; dir < directions.size(); ++dir) {
				if (searchWord(matrix, word, row, col, dir)) {
					return true;
				}
			}
		}
	}
	return false;
}

std::vector<std::string> read_words_from_HTML(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Can't open file: " << filename << std::endl;
		return {};
	}

	std::string line;
	std::vector<std::string> words;
	std::regex wordRegex(R"(([a-žA-Ž]+))");
	std::regex brRegex(R"(<br\s*/?>)");

	while (std::getline(file, line)) {

		// Remove <br> tags
		line = std::regex_replace(line, brRegex, "");

		// Push cleaned word
		words.push_back(line);
	}

	return words;
}

int main() {

	// Record start time 
	auto start = std::chrono::high_resolution_clock::now();

	// Load 2d matrix
	std::vector<std::string> matrix = {
		"GSETRIBEHKUŽKIH",
		"UQVLOGIKABTENOT",
		"AITAZIMKOEVANJM",
		"RPROGRAMIRANJEL",
		"DTC++HOVGITAPVK",
		"IBRAZVIJALEC/KA",
		"ASZARGRAFIČNITO",
		"RBPOGONOBIJANGB",
		"ITRAINTHEBRAINU",
		"SNOPTIMIZACIJAG",
		"AKJOINISUKODAPV",
		"SBTHEJGRADNJAČO",
		"KHTRIBEABVKEPMT",
		"SVODGOVORNOSTGZ",
		"ER#TANDIRECTXN@"
	};

	// Load words library
	std::vector<std::string> files = {
		/*
			Library source : http://bos.zrc-sazu.si/besede.html
		*/
		"../lib/sbsj/sbsj.html",

		/*
			Library source : https://github.com/dwyl/english-words
		*/
		"../lib/english-words-master/words_alpha.txt"
	};

	// Loop through all words from dictionary and search them in out matrix
	for (const auto& lib : files) {
		std::vector<std::string> words = read_words_from_HTML(lib);
		std::cout << "Library: " << lib << " -> Words found:\n";
		for (const auto& word : words) {
			if (iterate_every_position_and_every_direction_in_matrix(matrix, word)) {
				std::cout << word << std::endl;
			}
		}
	}

	// Record elapsed time
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Elapsed time: " << duration << " ms" << std::endl;

	std::cout << "Press key to exit\n";
	std::cin.get();
	return 0;
}
