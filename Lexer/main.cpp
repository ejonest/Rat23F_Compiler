#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "Lexer.h"

int main(){
	std::ofstream outputFile;
    outputFile.open("output.txt");
    std::string filename = "";

    //create lexer
    Lexer lexer;

	while(filename != "q") {
		std::cout << "Enter q to quit, or enter the source file name: \n";
		std::cin >> filename;

        if (filename == "q")
            return 0;
    
        std::ifstream file;

        file.open("test_cases/" + filename, std::ifstream::in);

        //handle error
        if (!file.is_open())
        {
            std::cerr << "Couldn't open specified file\n";
            exit(1);
        }

        //Print source code to console
        std::string line;
        std::cout << "SOURCE CODE:\n\n";
        while (getline(file, line))
        {
            std::cout << line << std::endl;
        }

        //reset error flags
        file.clear();
        //start at beginning of line 0
        file.seekg(0);

        std::string outputString = "Output for: " + filename + "\n";
        outputString.append(lexer.lexer(file));

        std::cout << outputString << std::endl;

        //close infile
        file.close();

        //write outputString to outputFile
        outputFile << outputString <<  std::endl;     
    }

	outputFile.close();
    return 0;
}
