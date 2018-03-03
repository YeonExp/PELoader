#include "PELoader.h"

int main()
{
	std::auto_ptr<PELoader> peLoader(new PELoader("C:\\Users\\dlawo\\Downloads\\Easy_UnpackMe\\Easy_UnpackMe.exe"));
	peLoader->loadExeFile();
	peLoader->loadSections();

	peLoader->generateImportDirectory();
	peLoader->printImportDirectory();
	return 0;
}