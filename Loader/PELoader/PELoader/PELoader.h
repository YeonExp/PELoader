#pragma once
#include "PELoader_impl.h"

class PELoader
{
public:
	PELoader(std::string fileName);
	~PELoader();
	const BOOL loadExeFile();
	const void loadSections();
	const void setSections(unsigned long sections);
private:
	std::tr1::shared_ptr<PELoaderImpl> peImpl;
	/*
	struct PEInfo
	{
		PIMAGE_DOS_HEADER DosHeader;
		PIMAGE_NT_HEADERS32 NtHeader;
		PIMAGE_SECTION_HEADER *SectionArray;
	};
	std::fstream inFile;
	std::string peFileName;
	*/
	PELoader(const PELoader& rhs);
	const PELoader& operator=(const PELoader& rhs);
};