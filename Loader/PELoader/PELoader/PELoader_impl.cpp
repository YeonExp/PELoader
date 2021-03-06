#include "PELoader_impl.h"

PELoaderImpl::PELoaderImpl(std::string fileName) : peFileName(fileName)
{
	std::cout << "[INFO] PE File Setting Complete" << std::endl;
}

PELoaderImpl::~PELoaderImpl() 
{
	try {
		if (inFile.is_open()) inFile.close();
		if (peData != NULL)
		{
			for (int i = 0; i < sections; i++)
			{
				delete peData->SectionArray[i];
			}
			delete peData->SectionArray;
			delete peData->NtHeader;
			delete peData->DosHeader;
			delete peData;
		}
	}
	catch(DWORD dwError)
	{
		std::cerr << "[ERROR] Error Code : " << dwError << std::endl;
	}
	std::cout << "[INFO] PELoaderImpl::~PELoaderImpl()" << std::endl;
}

const BOOL PELoaderImpl::loadExeFile()
{
	inFile = std::ifstream(peFileName.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!inFile.is_open())
	{
		std::cerr << "[ERROR] PELoaderImpl::loadExeFile()" << std::endl;
		return false;
	}
	std::cout << "[INFO] inFile Setting Complete" << std::endl;
	return true;
}

const void PELoaderImpl::loadSections()
{
	peData = new PEInfo();

	// Initialization
	peData->DosHeader = new IMAGE_DOS_HEADER();
	peData->NtHeader = new IMAGE_NT_HEADERS32();

	inFile.seekg(0);
	inFile.read((char *)(peData->DosHeader), sizeof(IMAGE_DOS_HEADER));
	
	inFile.seekg(peData->DosHeader->e_lfanew);
	inFile.read((char *)(peData->NtHeader), sizeof(IMAGE_NT_HEADERS32));
	sections = peData->NtHeader->FileHeader.NumberOfSections;

	setSections(sections);
	std::cout << "[INFO] PELoaderImpl::loadSections()" << std::endl;
}

const void PELoaderImpl::setSections(unsigned long sections)
{
	int SectionStart =
		peData->DosHeader->e_lfanew + sizeof(IMAGE_FILE_HEADER) + 
		peData->NtHeader->FileHeader.SizeOfOptionalHeader + 0x4;

	peData->SectionArray = new PIMAGE_SECTION_HEADER[sections];
	inFile.seekg(SectionStart);
	for (int i = 0; i < sections; i++)
	{
		peData->SectionArray[i] = new IMAGE_SECTION_HEADER();
		inFile.read((char *)(peData->SectionArray[i]), sizeof(IMAGE_SECTION_HEADER));
		// std::cout << peData->SectionArray[i]->Name << std::endl;
	}
	std::cout << "[INFO] PELoaderImpl::setSections(unsigned long sections)" << std::endl;
}

const unsigned long PELoaderImpl::generateImportDirectory()
{
	unsigned long rawImport = peData->NtHeader->OptionalHeader.DataDirectory[1].VirtualAddress;
	unsigned long rawSize = peData->NtHeader->OptionalHeader.DataDirectory[1].Size;

	modules = rawSize / 0x14;
	if (modules >= 1)
	{
		try
		{
			idp = new PIMAGE_IMPORT_DESCRIPTOR[modules];
		}
		catch(DWORD dwError)
		{
			std::cerr << "[ERROR] Memory Allocation Failed.. (PELoaderImpl::generateImportDirectory()" << std::endl;
			return -1;
		}
		for (int i = 0; i < modules; i++)
		{
			idp[i] = new IMAGE_IMPORT_DESCRIPTOR();
			inFile.seekg(rawImport);
			inFile.read((char *)idp[i], sizeof(IMAGE_IMPORT_DESCRIPTOR));

			rawImport += sizeof(IMAGE_IMPORT_DESCRIPTOR);
		}
	}
	std::cout << "[SUCCESS] PELoaderImpl::generateImportDirectory()" << std::endl;
	return 1;
}

const void PELoaderImpl::printImportDirectory()
{
	char c;
	char moduleName[256] = { 0 };
	for (int i = 0; i < modules; i++)
	{
		unsigned int count = 0;
		ZeroMemory(moduleName, 256);
		inFile.seekg(idp[i]->NameRVA);
		while (true)
		{
			c = inFile.get();
			if (c == '\0') break;
			moduleName[count] = c;
			count++;
		}
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "[INFO] Module Name : " << moduleName << std::endl;
		std::cout << "[INFO] INT : " << std::hex << idp[i]->OFTs << std::endl;
		std::cout << "[INFO] TimeDateStamp : " << std::hex << idp[i]->TimeDateStamp << std::endl;
		std::cout << "[INFO] ForwarderChain : " << std::hex << idp[i]->ForwarderChain << std::endl;
		std::cout << "[INFO] Name RVA : " << std::hex << idp[i]->NameRVA << std::endl;
		std::cout << "[INFO] IAT : " << std::hex << idp[i]->FTs << std::endl;
		std::cout << "-------------------------------------" << std::endl;
	}
}