#include "CFile.h"

bool CFile::ReadFile(const char* a_pFileName, std::string& a_strContext)
{
	FILE *pFile;
	pFile = fopen(a_pFileName, "r");
	if (pFile == NULL)
	{
		a_strContext = "Open file failed";
		return false;
	}

	fseek(pFile, 0L, SEEK_END);
	int nFileSize = ftell(pFile);
	char *pContext = (char *)malloc(nFileSize + 1);
	memset(pContext, 0, nFileSize + 1);
	if (pContext == nullptr)
	{
		a_strContext = "malloc failed";
		return false;
	}
	fseek(pFile, 0L, SEEK_SET);
	fread(pContext, nFileSize, 1, pFile);
	pContext[nFileSize] = 0;

	a_strContext = pContext;
	free(pContext);
	fclose(pFile);

	return true;
}