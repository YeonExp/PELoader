#pragma once
#include "common.h"

class PELoaderImpl
{
public:
	PELoaderImpl(std::string fileName);
	~PELoaderImpl();
	const BOOL loadExeFile();
	const void loadSections();
	const void setSections(unsigned long sections);
private:
	struct PEInfo
	{
		PIMAGE_DOS_HEADER DosHeader;
		PIMAGE_NT_HEADERS32 NtHeader;
		PIMAGE_SECTION_HEADER *SectionArray;
	};
	unsigned int sections;
	std::ifstream inFile;
	const std::string peFileName;
	struct PEInfo *peData;
	PELoaderImpl(const PELoaderImpl& rhs);
	const PELoaderImpl& operator=(const PELoaderImpl& rhs);
};