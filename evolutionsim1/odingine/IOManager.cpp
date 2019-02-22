#include "IOManager.h"
#include <fstream>

namespace odingine {

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		// seek to the end of the file
		file.seekg(0, std::ios::end);

		// get file size
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		// reduce filesize by any header bytes 
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize); // cast to buffer of chars
		file.close();

		return true;
	}

}
