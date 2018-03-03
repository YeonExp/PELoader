#pragma once
#include "common.h"

class PELoaderImpl
{
public:
	PELoaderImpl(std::string fileName);
	virtual ~PELoaderImpl();
	const BOOL loadExeFile();
	const void loadSections();
	const void setSections(unsigned long sections);
	const unsigned long generateImportDirectory();
	const void printImportDirectory();
private:
	struct PEInfo
	{
		PIMAGE_DOS_HEADER DosHeader;
		PIMAGE_NT_HEADERS32 NtHeader;
		PIMAGE_SECTION_HEADER *SectionArray;
	};
	unsigned long sections, modules;
	std::ifstream inFile;
	const std::string peFileName;
	struct PEInfo *peData;
	PIMAGE_IMPORT_DESCRIPTOR *idp;
	PELoaderImpl(const PELoaderImpl& rhs);
	const PELoaderImpl& operator=(const PELoaderImpl& rhs);
};