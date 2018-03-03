#include "PELoader.h"

PELoader::PELoader(std::string fileName) : peImpl(new PELoaderImpl(fileName))
{
	std::cout << "[INFO] PELoader::PELoader(std::string fileName)" << std::endl;
}

PELoader::~PELoader()
{
	std::cout << "[INFO] PELoader::~PELoader()" << std::endl;
}

const BOOL PELoader::loadExeFile()
{
	return peImpl->loadExeFile();
}

const void PELoader::loadSections()
{
	return peImpl->loadSections();
}



const unsigned long PELoader::generateImportDirectory()
{
	return peImpl->generateImportDirectory();
}