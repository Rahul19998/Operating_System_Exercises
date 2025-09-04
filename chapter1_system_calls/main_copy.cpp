#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <type_traits>

#define BUF_SIZE 8192

enum Status
{
	SUCCESS,
	SOURCE_NOT_FOUND,
	DESTINATION_NOT_FOUND,
	WRITE_ERROR,
	WRONGUSAGE
};

int main(int argc, char *argv[])
{
	std::string sourceFileName{""};
	std::string destinationFileName{""};
	int inputFd{0};
	int outputFd{0};
	ssize_t retIn{0};
	ssize_t retOut{0};
	char buffer[BUF_SIZE];
        if(argc !=3)
	{
		std::cerr << "Usage: cp <source> <detination>" << std::endl;
		return Status::WRONGUSAGE;
	}
	sourceFileName = argv[1];
	destinationFileName = argv[2];

	std::cout << "\nChecking if the source file exist" << std::endl;
	inputFd = open(sourceFileName.c_str(), O_RDONLY);
	if(inputFd == -1)
	{
		std::cerr << "Source File not found.." << std::endl;
		return Status::SOURCE_NOT_FOUND;
	}

	std::cout << "Checking if destination is present or not!" << std::endl;
	outputFd = open(destinationFileName.c_str(), O_WRONLY);
	if(outputFd == -1)
	{
		std::cerr << "Dstination is not present.." << std::endl;
		return Status::DESTINATION_NOT_FOUND;
	}

	/* Copy process */
    while((retIn = read (inputFd, &buffer, BUF_SIZE)) > 0)
	{
		retOut = write (outputFd, &buffer, static_cast<ssize_t>(retIn));
		if(retOut != retIn)
		{
                /* Write error */
			std::cerr << "Write error!";
			return Status::WRITE_ERROR;
		}
    }

	/*Close file descriptors*/
	close(inputFd);
	close(outputFd);

	return 0;
}
