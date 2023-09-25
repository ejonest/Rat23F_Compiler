#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.h"

int main(){
	std::ofstream outputFile("output.txt");
    std::string filename;
	std::string line;

	do {
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

        //Write source code to ouput file
        std::cout << "SOURCE CODE:\n\n";
        while (getline(file, line))
        {
            std::cout << line << std::endl;
        }

        //create lexer
        Lexer lexer;

        //reset error flags
        file.clear();
        //start at beginning of line 0
        file.seekg(0);

        std::string outputString = lexer.lexer(file);

        std::cout << outputString << std::endl;

        //close infile
        file.close();

        //write outputString to outputFile
        outputFile << outputString <<  std::endl;
        
        
    } while (filename != "q");

	outputFile.close();
    return 0;
}
