// TODO: Instead of having a fixed number of arguments for each build mode, allow parameters to be modified with a set of "commands"

#include "ZFile.h"
#include "ZTexture.h"
#include "Overlays/ZOverlay.h"
#include "Path.h"
#include "File.h"
#include "Globals.h"

#ifndef _MSC_VER
#include <execinfo.h>
#include <unistd.h>
#include <signal.h>
#endif

#include <string>
#include "tinyxml2.h"

extern void ModelTest();

using namespace tinyxml2;
using namespace std;

bool Parse(string xmlFilePath, string basePath, string outPath, ZFileMode fileMode);
void BuildAssetTexture(string pngFilePath, TextureType texType, string outPath);
int OldMain(int argc, char* argv[]);
int NewMain(int argc, char* argv[]);

#ifndef _MSC_VER
void ErrorHandler(int sig)
{
	void* array[4096];
	char** symbols;
	size_t size;
	size = backtrace(array, 4096);
	symbols = backtrace_symbols(array, 4096);

	for (int i = 1; i < size; i++)
	{
		size_t len = strlen(symbols[i]);
		cout << symbols[i] << "\n";
	}

	//cout << "Error: signal " << sig << ":\n";
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}
#endif

int main(int argc, char* argv[])
{
#ifndef _MSC_VER
	signal(SIGSEGV, ErrorHandler);
#endif

	Globals* g = new Globals();

	// TEST TEST
#if _MSC_VER && _DEBUG
	//ModelTest();
	//return 0;
#endif


	//return OldMain(argc, argv);
	return NewMain(argc, argv);
	
	return 0;
}

int NewMain(int argc, char* argv[])
{
	// Syntax: ZAP2.exe [mode (b/btex/bovl/e)] (Arbritrary Number of Arguments)

	// Parse File Mode
	string buildMode = argv[1];
	ZFileMode fileMode = ZFileMode::Invalid;

	if (buildMode == "b")
		fileMode = ZFileMode::Build;
	else if (buildMode == "btex")
		fileMode = ZFileMode::BuildTexture;
	else if (buildMode == "bovl")
		fileMode = ZFileMode::BuildOverlay;
	else
		fileMode = ZFileMode::Extract;

	if (fileMode == ZFileMode::Invalid)
	{
		cout << "Error: Invalid file mode.\n";
		return 1;
	}

	// Parse other "commands"
	for (int i = 2; i < argc; i++)
	{
		string arg = argv[i];

		if (arg == "-o" || arg == "--outputpath") // Set output path
		{
			Globals::Instance->outputPath = argv[i + 1];
			i++;
		}
		else if (arg == "-i" || arg == "--inputpath") // Set input path
		{
			Globals::Instance->inputPath = argv[i + 1];
			i++;
		}
		else if (arg == "-b" || arg == "--baserompath") // Set baserom path
		{
			Globals::Instance->baseRomPath = argv[i + 1];
			i++;
		}
		else if (arg == "-gsf") // Generate source file
		{
			Globals::Instance->genSourceFile = string(argv[i + 1]) == "1";
			i++;
		}
		else if (arg == "-tt") // Set texture type
		{
			Globals::Instance->texType = ZTexture::GetTextureTypeFromString(argv[i + 1]);
			i++;
		}
		else if (arg == "-cfg") // Set cfg path
		{
			Globals::Instance->cfgPath = argv[i + 1];
			i++;
		}
		else if (arg == "-sm") // Set symbol map path
		{
			Globals::Instance->GenSymbolMap(argv[i + 1]);
			i++;
		}
	}

	if (fileMode == ZFileMode::Build || fileMode == ZFileMode::Extract)
	{
		Parse(Globals::Instance->inputPath, Globals::Instance->baseRomPath, Globals::Instance->outputPath, fileMode);
	}
	else if (fileMode == ZFileMode::BuildTexture)
	{
		TextureType texType = Globals::Instance->texType;
		string pngFilePath = Globals::Instance->inputPath;
		string outFilePath = Globals::Instance->outputPath;

		BuildAssetTexture(pngFilePath, texType, outFilePath);
	}
	else if (fileMode == ZFileMode::BuildOverlay)
	{
		ZOverlay* overlay = ZOverlay::FromBuild(Path::GetDirectoryName(Globals::Instance->inputPath), Path::GetDirectoryName(Globals::Instance->cfgPath));
		if (overlay)
		{
			string source = overlay->GetSourceOutputCode("");
			File::WriteAllText(Globals::Instance->outputPath, source);
		}
	}

	return 0;
}

int OldMain(int argc, char* argv[])
{
	if (argc != 3 && argc != 4 && argc != 5 && argc != 6)
	{
		cout << "ZAP2: Zelda Asset Processor (2)" << "\n";
		cout << "Usage: ZAP2.exe [mode (b/btex/bovl/e)] [Input XML/Texture File/Bin File] [outputPath]" << "\n";
		return 1;
	}

	// Parse File Mode
	string buildMode = argv[1];
	ZFileMode fileMode = ZFileMode::Invalid;

	if (buildMode == "b")
		fileMode = ZFileMode::Build;
	else if (buildMode == "btex")
		fileMode = ZFileMode::BuildTexture;
	else if (buildMode == "bovl")
		fileMode = ZFileMode::BuildOverlay;
	else
		fileMode = ZFileMode::Extract;

	if (fileMode == ZFileMode::Invalid)
	{
		cout << "Error: Invalid file mode.\n";
		return 1;
	}

	if (fileMode == ZFileMode::Build || fileMode == ZFileMode::Extract)
	{
		// Syntax: ZAP2.exe e [input xml file] [baseromPath] [outputPath] [(OPTIONAL) shouldGenerateSourceFile (0/1)]
		if (fileMode == ZFileMode::Extract && argc == 6)
			Globals::Instance->genSourceFile = argv[5][0] == '1';

		Parse(argv[2], argv[3], argv[4], fileMode);
	}
	else if (fileMode == ZFileMode::BuildTexture)
	{
		// Syntax: ZAP2.exe btex [texType] [pngFilePath] [outFilePath]
		string texTypeStr = argv[2];
		string pngFilePath = argv[3];
		string outFilePath = argv[4];

		TextureType texType = ZTexture::GetTextureTypeFromString(texTypeStr);

		BuildAssetTexture(pngFilePath, texType, outFilePath);
	}
	else if (fileMode == ZFileMode::BuildOverlay)
	{
		// Syntax: ZAP2.exe bovl [elfFilePath] [cfgFolderPath] [outputFilePath]
		ZOverlay* overlay = ZOverlay::FromBuild(Path::GetDirectoryName(argv[2]), Path::GetDirectoryName(argv[3]));
		if (overlay)
		{
			string source = overlay->GetSourceOutputCode("");
			File::WriteAllText(argv[4], source);
		}
	}

	return 0;
}

bool Parse(string xmlFilePath, string basePath, string outPath, ZFileMode fileMode)
{
	XMLDocument doc;

	XMLError eResult = doc.LoadFile(xmlFilePath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
		return false;

	XMLNode* root = doc.FirstChild();

	if (root == nullptr)
		return false;

	vector<ZFile*> files = vector<ZFile*>();

	for (XMLElement* child = root->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
	{
		if (string(child->Name()) == "File")
		{
			//ZFile* file = new ZFile(fileMode, child, Path::GetDirectoryName(xmlFilePath));
			ZFile* file = new ZFile(fileMode, child, basePath, outPath);

			files.push_back(file);
		}
	}

	for (ZFile* file : files)
	{
		if (fileMode == ZFileMode::Build)
			file->BuildResources();
		else
			file->ExtractResources(outPath);
	}

	XMLElement* element = root->FirstChildElement("File");

	if (element == nullptr)
		return false;

	return true;
}

void BuildAssetTexture(string pngFilePath, TextureType texType, string outPath)
{
	ZTexture* tex = ZTexture::FromPNG(pngFilePath, texType);
	File::WriteAllBytes(outPath, tex->GetRawData());
	
	delete tex;
}