#pragma once

#include <map>
#include <string>
#include <vector>

class OutputFormatter
{
private:
	const int tabSize;
	const int defaultIndent;
	const int lineLimit;

	int col;
	int nest;
	int nestIndent[8];
	int currentIndent;
	int wordNests;

	char word[128];
	char space[128];
	char* wordP;
	char* spaceP;

	std::string str;

	void Flush();

	static OutputFormatter* Instance;
	static int WriteStatic(const char* buf, int count);

public:
	OutputFormatter(int tabSize = 4, int defaultIndent = 4, int lineLimit = 120);

	int (*StaticWriter())(const char* buf, int count);

	int Write(const char* buf, int count);

	std::string GetOutput();
};