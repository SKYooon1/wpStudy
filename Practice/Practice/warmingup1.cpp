#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

void inputSentence(std::vector<std::string>& wordsInput);
void printSentence(std::vector<std::string>& wordsInput);
void getCommand(std::vector<std::string>& wordsInput);
void convertEwordToUpper(std::vector<std::string>& wordsInput);
void convertCase(std::vector<std::string>& wordsInput);
void rotateSentence(std::vector<std::string>& wordsInput);

bool blankFlag{ true };		// Convert by command f

int main()
{
	std::string a;
	std::cin >> a;

	std::cout << "Input sentence: ";
	std::vector<std::string> wordsInput;
	inputSentence(wordsInput);

	getCommand(wordsInput);
}

void inputSentence(std::vector<std::string>& wordsInput)
{
	std::string tempWord;
	while (true)
	{
		std::cin >> tempWord;
		wordsInput.push_back(tempWord);
		if (tempWord.back() == '.')
			break;
	}
}

void printSentence(std::vector<std::string>& wordsInput)
{
	if (blankFlag)
	{
		for (std::string s : wordsInput)
			std::cout << s << " ";
	}
	else
		for (std::string s : wordsInput)
			std::cout << s << "@";
}

void getCommand(std::vector<std::string>& wordsInput)
{
	char commandInput{};
	while (true)
	{
		std::cout << "Input command" << std::endl;
		commandInput = _getch();

		std::cout << "Command ";
		switch (commandInput)
		{
		case 'q':
			return;
		case 'e':
			std::cout << "e: ";
			convertEwordToUpper(wordsInput);
			break;
		case 'f':		// Convert blank to at, at to blank
			std::cout << "f: ";
			blankFlag = !blankFlag;
			break;
		case 'l':		// Count the words
			std::cout << "l: " << wordsInput.size() << " words";
			break;
		case 'c':
			std::cout << "c: ";
			convertCase(wordsInput);
			break;
		case 'r':
			rotateSentence(wordsInput);
			break;
		}
		printSentence(wordsInput);
		std::cout << std::endl;
	}
}

void convertEwordToUpper(std::vector<std::string>& wordsInput)
{
	bool eFlag{};
	for (std::string s : wordsInput)
	{
		for (char c : s)		// Is there e or E?
		{
			if (c == 'e' || c == 'E')
				eFlag = true;
		}
		if (eFlag)				// If it is, convert to upper case
		{
			for (char c : s)
				c = toupper(c);
			eFlag = false;		// Clear the flag
		}
	}
}

void convertCase(std::vector<std::string>& wordsInput)
{
	for (std::string s : wordsInput)
	{
		for (char c : s)
		{
			if (isupper(c))
				c = tolower(c);
			else
				c = toupper(c);
		}
	}
}

void rotateSentence(std::vector<std::string>& wordsInput)
{
	std::string& s = wordsInput.front();
	wordsInput.back().push_back(s.front());		// Push back the first letter
	if (s.size() == 1)		// If first word has just a letter, erase the word from the vector
		wordsInput.erase(wordsInput.begin());
	else					// Else, erase just a letter
		s.erase(s.begin());
}