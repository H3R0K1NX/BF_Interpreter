#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>

#define MEMORY_SIZE 30000
#define U8 unsigned char

class Interpreter {
private:
	U8* memoryBlock = new U8[MEMORY_SIZE]{ 0 };
	U8* memoryPtr = &memoryBlock[0];
	size_t cellIndex = 0;

public:
	void readStream(const char* input)
	{
		size_t loop, i;
		U8 c;
		for (i = 0; input[i] != 0; ++i)
		{
			c = input[i];
			switch (c)
			{
			case '>':
				if (memoryPtr < &memoryBlock[MEMORY_SIZE - 1])
				{
					++memoryPtr;
					++cellIndex;
				}
				else
					std::cout << "Out Of Bounds: Right Bound" << std::endl;
				break;
			case '<':
				if (memoryPtr > &memoryBlock[0])
				{
					--memoryPtr;
					--cellIndex;
				}
				else
					std::cout << "Out Of Bounds: Left Bound" << std::endl;
				break;
			case '+':
				if (*memoryPtr == 255)
					std::cout << "Overflow denied (didn't increment over 255)" << std::endl;
				else
					++* memoryPtr;
				break;
			case '-':
				if (*memoryPtr == 0)
					std::cout << "Underflow denied (didn't decrement under 0" << std::endl;
				else
					--* memoryPtr;
				break;
			case '.':
				putchar(*memoryPtr);
				break;
			case ',':
				std::cin >> *memoryPtr;
				break;
			case '[':
				continue;
			case ']':
				if (*memoryPtr)
				{
					loop = 1;
					while (loop > 0)
					{
						c = input[--i];
						if (c == '[')
							--loop;
						else if (c == ']')
							++loop;
					}
					break;
				}
				break;
			default:
				break;
			}
		}
		delete[] memoryBlock; //Clears the heap allocation
	}

};

char* readFromFile(const char* FileName)
{
	size_t length = 0;
	std::ifstream file(FileName, std::ios::in); //Opening the file for 
	if (!file)
	{
		std::cout << "Error, the file couldn't be opened!" << std::endl;
		exit(-2);
	}
	std::string line, totalCode;
	while (std::getline(file, line))
		length += line.length();
	std::cout << "The length of the code is: " << length << std::endl;
	file.clear();
	file.seekg(0); //Rewinds the file's crusor position.
	while (std::getline(file, line))
		totalCode += line;
	std::cout << "Code to interpret: " << totalCode << std::endl;
	file.close(); //Closes the file
	char* toInterpret = new char[length];
	strcpy(toInterpret, totalCode.c_str());
	return toInterpret;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Running the program failed!" << std::endl;
		std::cout << "Too few or too less given arguments [Requires 1]" << std::endl;
		std::cout << "Format: bf.exe filename.bf" << std::endl;
		return -1;
	}
	Interpreter interpreter; //Creates an instance of the interpreter
	char* toInterpret = readFromFile(argv[1]);
	std::cout << "The output is: " << std::endl << std::endl;
	interpreter.readStream(toInterpret);
	return 0;
}
