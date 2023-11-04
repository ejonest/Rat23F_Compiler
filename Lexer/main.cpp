#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
// #include "Lexer.h"
#include "SyntaxChecker.cpp"

int main(){
	// std::ofstream outputFile;
    // outputFile.open("output.txt");
    std::string filename = "";

    bool syntaxOn = true; //true for syntax checker and false for just lexer

    //create lexer
    Lexer lexer;

	while(filename != "q") {
        std::ofstream outputFile;
        outputFile.open("output.txt");

		std::cout << "Enter q to quit, or enter the source file name: \n";
		std::cin >> filename;

        if (filename == "q")
            return 0;
    
        std::ifstream file;
        // std::ofstream tempFile;

        file.open("test_cases/" + filename, std::ifstream::in);

        //handle error
        if (!file.is_open())
        {
            std::cerr << "Couldn't open specified file\n";
            exit(1);
        }

        //Print source code to console
        std::string line;
        std::cout << "============================Source Code===========================\n\n";
        while (getline(file, line))
        {
            std::cout << line << std::endl;
        }

        //reset error flags
        file.clear();
        //start at beginning of line 0
        file.seekg(0);

        std::cout << "\n=========================Source Code Done=========================\n";
        std::string outputString = "Output for: " + filename + "\n";
        // while (getline(file, line))
        // {
        //     tempFile.open("test_cases/tempFile.txt");
        //     tempFile << line << std::endl;
        //     SyntaxChecker("test_cases/tempFile.txt");
        //     // outputString.append(lexer.lexer("test_cases/tempFile.txt")); //Lexer only
        //     tempFile.close();
        // }

        // std::string outputString = "Output for: " + filename + "\n";
        // here we should call the syntax checker instead which will call the lexer in steps
        // in addtion we should not create the lexer here we create it later
        outputString.append(lexer.lexer(file));
        if (!syntaxOn) {
            std::cout << outputString << std::endl;
        }

        //close infile
        file.close();

        //write outputString to outputFile
        outputFile << outputString <<  std::endl;
        if (syntaxOn) {
            SyntaxChecker("output.txt");
        }
        exit(0);
        outputFile.close();
    }

	// outputFile.close();

    return 0;
}
