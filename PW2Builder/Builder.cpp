#include <cstring>
#include <cstdio>

#include "vector"
#include "string"

#include "filesystem"

#include "AssetManager.h"

using namespace std;

#define ARRAY_COUNT(arr) sizeof(arr) / sizeof(arr[0])

#define SEPARATOR '\\'
#define NOT_SEPARATOR '/'

#define MAIN_DIR ".."

#define BUILD_DIR PathConcat(MAIN_DIR, "build")

#define GLOBAL_DIR PathConcat(MAIN_DIR, "Global")
#define HEADER_DIR PathConcat(MAIN_DIR, "Headers")
#define PATCH_DIR PathConcat(MAIN_DIR, "Patches")
#define LIB_DIR PathConcat(MAIN_DIR, "Libraries")
#define DATA_PATH PathConcat(MAIN_DIR, "Data")

#define EXTERNAL_DIR PathConcat(MAIN_DIR, "ExternalDependencies")

#define ESDB_DIR PathConcat(MAIN_DIR, "ESDB.yml")

#define CTRMAP_DIR "%CTRMAP_DIR%"
#define ARM_NONE_EABI_DIR "%ARM_NONE_EABI_DIR%"
#define JAVA_DIR "%JAVA_DIR%"

#define SWAN_DIR "swan"
#define EXTLIB_DIR "ExtLib"
#define NK_DIR "NitroKernel\\include"
#define LIBRPM_DIR "libRPM\\include"

#define PROJECT_DATA_PATH(projectDir) string(projectDir) + "vfs\\data"
#define PROJECT_PATCH_PATH(projectDir) PathConcat(PROJECT_DATA_PATH(projectDir), "patches")
#define PROJECT_LIB_PATH(projectDir) PathConcat(PROJECT_DATA_PATH(projectDir), "lib")

#define SCRIPT_PATH PathConcat(BUILD_DIR, "build.bat")

#define ECHO_TITLE(str) string("echo ==============================\necho ") + str + "\necho ==============================\n"
#define ECHO_SUBTITLE(str) string("echo ---\necho ") + str + "\necho ---\n"
#define ECHO(str) string("echo ") + str + '\n'
#define ECHO_TAB(str) ""// string("echo \t") + str + '\n'

#define COMPILER string(ARM_NONE_EABI_DIR) + "arm-none-eabi-g++ "
#define COMPILE_INCLUDE(path) string("-I ") + path + ' '
#define COMPILE_INCLUDE_DIRS COMPILE_INCLUDE(PathConcat(EXTERNAL_DIR, SWAN_DIR)) + COMPILE_INCLUDE(PathConcat(EXTERNAL_DIR, EXTLIB_DIR)) + COMPILE_INCLUDE(PathConcat(EXTERNAL_DIR, NK_DIR)) + COMPILE_INCLUDE(PathConcat(EXTERNAL_DIR, LIBRPM_DIR)) + COMPILE_INCLUDE(HEADER_DIR) + COMPILE_INCLUDE(GLOBAL_DIR)
#define COMPILE_OUTPUT(path) string("-o ") + path + ' '
#define COMPILE_FLAGS "-r -mthumb -march=armv5t -Os -mlong-calls"
#define COMPILE_EXTENSION(compileType) ((compileType == CPP) ? ".o" : "ARM.o")

#define MERGER string(ARM_NONE_EABI_DIR) + "arm-none-eabi-ld "
#define MERGE_FLAGS "-r "
#define MERGE_OUTPUT(path) string("-o ") + path + ' '

#define LINKER string(JAVA_DIR) + "java -cp " + CTRMAP_DIR + "CTRMap.jar rpm.cli.RPMTool "
#define LINK_INPUT(path) string("-i ") + path + ' '
#define LINK_EXTENSION ".dll"
#define LINK_OUTPUT(path) string("-o ") + path + ' '
#define LINK_ESDB string("--esdb ") + ESDB_DIR + ' '
#define LINK_FLAGS "--fourcc DLXF --generate-relocations"

#define BUILD_SETTINGS_PATH "buildSettings.txt"
#define BUILD_SETTINGS_MAX_SIZE 8192
#define BUILD_SETTINGS_PROJECT_DIR "SET PROJECT_DIR="
#define BUILD_INSTALL_LOG "install.log"

enum CompileType
{
	DONT_COMPILE = 0,
	CPP = 1,
	ARM = 2,
};
struct CompileObject
{
	string name = string();
	CompileType type = DONT_COMPILE;

	string input = string();
	string folder = string();
};
struct CompileStructure
{
	string name = string();
	vector<CompileObject> objects;
};

struct OutputObject
{
	string name = string();
	string path = string();

	bool upToDate = false;

	string folder = string();
};
#define OUTPUT_INVALID(obj) obj.path.empty()

string LowerCase(const string& input)
{
	string output;
	for (size_t idx = 0; idx < input.size(); ++idx)
		output.push_back(tolower(input[idx]));
	return output;
}

void PathCorrectSeparator(string& path)
{
	for (size_t idx = 0; idx < path.length(); ++idx)
		if (path[idx] == NOT_SEPARATOR)
			path[idx] = SEPARATOR;
}
string PathGetExtension(const string& path)
{
	size_t start = path.find_last_of('.');
	if (start == string::npos)
		return string();

	++start;
	return path.substr(start, path.length() - start);
}
string PathRemoveExtension(const string& path)
{
	string extension = PathGetExtension(path);
	if (extension.empty())
		return path;

	return path.substr(0, path.length() - extension.length() - 1);
}
string PathGetLast(const string& path)
{
	size_t lastSlash = path.find_last_of(SEPARATOR) + 1;
	if (lastSlash == string::npos)
		return path;
	return path.substr(lastSlash);
}
string PathRemoveLast(const string& path, bool removeSeparator = false)
{
	size_t lastSlash = path.find_last_of(SEPARATOR) + 1;
	if (lastSlash == string::npos)
		return string();
	if (removeSeparator)
		lastSlash -= 1;
	return path.substr(0, lastSlash);
}
string PathGetLastName(const string& path)
{
	string file = PathGetLast(path);
	if (file.empty())
		return string();

	return PathRemoveExtension(file);
}
string PathConcat(const string& path, const string& concat) 
{ 
	if (path.empty())
		return concat;
	if (concat.empty())
		return path;
	return path + SEPARATOR + concat; 
}
bool PathRemove(const string& path)
{
	if (!filesystem::exists(path))
		return true;

	std::error_code ec;
	if (!filesystem::remove_all(path, ec))
	{
		printf_s("Error removing path %s (%s)\n", path.c_str(), ec.message().c_str());
		return false;
	}
	return true;
}

bool FileIsUptoDate(const string& input, const string& output)
{
	if (!filesystem::exists(input) ||
		!filesystem::exists(output))
		return false;

	filesystem::file_time_type inTime = filesystem::last_write_time(input);
	filesystem::file_time_type outTime = filesystem::last_write_time(output);
	if (outTime > inTime)
		return true;
	return false;
}

vector<string> FolderGetList(const string& path)
{
	vector<string> folderList;
	if (!filesystem::is_directory(path))
		return folderList;

	for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
	{
		string path = entry.path().string();
		if (!path.empty())
		{
			PathCorrectSeparator(path);
			folderList.push_back(path);
		}
	}

	return folderList;
}
bool FolderCreate(const string& path)
{
	if (filesystem::exists(path))
		return true;

	std::error_code ec;
	if (!filesystem::create_directories(path, ec))
	{
		printf_s("Error creating directory %s (%s)\n", path.c_str(), ec.message().c_str());
		return false;
	}
	return true;
}

bool FindAsset(const string& assetName, const char* assets[], size_t assetsSize)
{
	for (size_t asset = 1; asset < assetsSize; ++asset)
		if (assetName == assets[asset])
			return true;
	return false;
}
#define FIND_ASSET(assetName, assets) FindAsset(assetName, assets, ARRAY_COUNT(assets))

vector<string> installLog;
void LoadInstallLog(const string& path)
{
	FILE* file = nullptr;
	fopen_s(&file, path.c_str(), "r");
	if (!file)
	{
		printf_s("Couldn't load install log %s\n", path.c_str());
		printf_s("	Error opening file\n");
		return;
	}

	char line[BUILD_SETTINGS_MAX_SIZE];
	fgets(line, BUILD_SETTINGS_MAX_SIZE, file);
	while (!feof(file))
	{
		installLog.push_back(line);

		string& log = installLog.back();
		size_t lastEntry = log.length() - 1;
		if (log[lastEntry] == '\n')
			log.erase(log.begin() + lastEntry);

		fgets(line, BUILD_SETTINGS_MAX_SIZE, file);
	}

	fclose(file);
}
void SaveInstallLog(const string& path)
{
	FILE* file = nullptr;
	fopen_s(&file, path.c_str(), "w");
	if (!file)
	{
		printf_s("Couldn't save install log %s\n", path.c_str());
		printf_s("	Error opening file\n");
		return;
	}

	for (size_t log = 0; log < installLog.size(); ++log)
	{
		fwrite(installLog[log].c_str(), sizeof(char), installLog[log].length(), file);
		fputc('\n', file);
	}
	fclose(file);
}
void AddInstallLog(const string& installPath)
{
	for (size_t log = 0; log < installLog.size(); ++log)
		if (installLog[log] == installPath)
			return;

	installLog.push_back(installPath);
}

CompileType GetCompileType(const string& path)
{
	string extension = LowerCase(PathGetExtension(path));
	if (extension == "cpp")
		return CPP;
	if (extension == "s")
		return ARM;
	return DONT_COMPILE;
}
bool GetFolderCompilationData(const string& path, CompileStructure& data)
{
	if (!filesystem::is_directory(path))
	{
		printf_s("Couldn't Compile %s\n", path.c_str());
		printf_s("	Path is not a folder\n");
		return false;
	}

	vector<string> directoryEntries = FolderGetList(path);
	if (!directoryEntries.size())
	{
		printf_s("Couldn't Compile %s\n", path.c_str());
		printf_s("	No files in folder\n");
		return false;
	}
	
	for (size_t entryIdx = 0; entryIdx < directoryEntries.size(); ++entryIdx)
	{
		const string& entry = directoryEntries[entryIdx];
		if (filesystem::is_directory(entry))
			continue;

		CompileType compileType = GetCompileType(entry);
		if (compileType == DONT_COMPILE)
			continue;

		string fileName = PathGetLastName(entry);
		if (fileName.empty())
			continue;

		data.objects.push_back({ fileName , compileType, entry });
	}

	if (data.objects.empty())
	{
		printf_s("Couldn't Compile %s\n", path.c_str());
		printf_s("	No compilable files in folder\n");

		data = CompileStructure();
		return false;
	}

	data.name = PathGetLast(path);
	if (data.name.empty())
	{
		printf_s("Couldn't Compile %s\n", path.c_str());
		printf_s("	Could not resolve the output name\n");

		data = CompileStructure();
		return false;
	}

	return true;
}
bool GetFileCompilationData(const string& path, CompileObject& object)
{
	if (filesystem::is_directory(path))
	{
		printf_s("Couldn't Compile %s\n", path.c_str());
		printf_s("	Path is not a file\n");
		return false;
	}

	CompileType compileType = GetCompileType(path);
	if (compileType == DONT_COMPILE)
		return false;
	object.type = compileType;
	object.input = path;

	object.name = PathGetLastName(path);
	if (object.name.empty())
	{
		printf_s("Couldn't Compile %s\n", path.c_str());
		printf_s("	Couldn't resolve name\n");

		object = CompileObject();
		return false;
	}

	return true;
}

bool GetPatchesCompilationData(const string& path, vector<CompileStructure>& patches)
{
	vector<string> patchNames = FolderGetList(path);
	if (patchNames.empty())
	{
		printf_s("No patches to compile\n");
		return false;
	}

	patches.reserve(patchNames.size());
	for (size_t patchIdx = 0; patchIdx < patchNames.size(); ++patchIdx)
	{
		CompileStructure patch;
		if (!GetFolderCompilationData(patchNames[patchIdx], patch))
			continue;

		patches.push_back(patch);
	}
	if (patches.empty())
	{
		printf_s("No compilable data was found in any patch\n");
		return false;
	}

	return true;
}
void GetLibrariesCompilationData(const string& directory, vector<CompileObject>& libraries, const string& folder = string())
{
	string path = PathConcat(directory, folder);

	vector<string> entries = FolderGetList(path);
	if (!entries.empty())
	{
		for (size_t entryIdx = 0; entryIdx < entries.size(); ++entryIdx)
		{
			const string& entry = entries[entryIdx];
			if (filesystem::is_directory(entry))
			{
				string assetFolder = entry.substr(directory.length() + 1);
				if (!FIND_ASSET(assetFolder, libraryAssets))
					continue;

				GetLibrariesCompilationData(directory, libraries, assetFolder);
				continue;
			}

			CompileObject lib;
			if (!GetFileCompilationData(entry, lib))
				continue;

			lib.folder = folder;
			libraries.push_back(lib);
		}
	}
}

OutputObject CompileCompObject(const string& path, const CompileObject& object, string& buildScript)
{
	string output = PathConcat(path, object.name) +
		COMPILE_EXTENSION(object.type);

	bool upToDate = FileIsUptoDate(object.input, output);
	if (!upToDate)
	{
		buildScript += ECHO(object.name + " is compiling...");

		string command = COMPILER;
		command += object.input + ' ';

		switch (object.type)
		{
		case CPP:
			command += COMPILE_INCLUDE_DIRS;
			break;
		case ARM:
			break;
		}
		command += COMPILE_INCLUDE(MAIN_DIR);
		command += COMPILE_OUTPUT(output);
		command += COMPILE_FLAGS;

		buildScript += command + '\n';
	}
	else
		buildScript += ECHO_TAB(object.name + " is up to date");

	return { object.name, output, upToDate, object.folder };
}
OutputObject CompileCompStruct(const string& path, const CompileStructure& structure, string& buildScript, OutputObject* externMerge = nullptr)
{
	buildScript += ECHO_SUBTITLE(structure.name + " is compiling...");

	vector<OutputObject> compiled;
	unsigned int upToDate = 0;
	for (size_t objIdx = 0; objIdx < structure.objects.size(); ++objIdx)
	{
		OutputObject output = CompileCompObject(path, structure.objects[objIdx], buildScript);
		if (OUTPUT_INVALID(output))
			continue;
		if (output.upToDate)
			++upToDate;
		compiled.push_back(output);
	}
	if (compiled.empty())
	{
		printf_s("%s has no compilable objects\n", structure.name.c_str());
		return OutputObject();
	}

	// If there is only 1 compilable object return the only output
	if (compiled.size() == 1 && !externMerge)
		return compiled[0]; 

	// If all the compilable object to be merged are up to date return an up to date output
	string output = PathConcat(path, structure.name) + COMPILE_EXTENSION(CPP);
	if (compiled.size() == upToDate)
		if (!externMerge || externMerge->upToDate)
			return { structure.name, output, true };

	string command = MERGER;
	command += MERGE_FLAGS;
	command += MERGE_OUTPUT(output);
	for (size_t compIdx = 0; compIdx < compiled.size(); ++compIdx)
		command += compiled[compIdx].path + ' ';
	if (externMerge)
		command += externMerge->path;

	buildScript += command + '\n';
	return { structure.name, output, false };
}

void LinkCompOutputs(const string& path, const vector<OutputObject>& outputs, string& buildScript)
{
	for (size_t outIdx = 0; outIdx < outputs.size(); ++outIdx)
	{
		string fullPath = path;
		if (!outputs[outIdx].folder.empty())
		{
			fullPath = PathConcat(path, outputs[outIdx].folder);
			FolderCreate(fullPath);
		}

		string output = PathConcat(fullPath, outputs[outIdx].name) + LINK_EXTENSION;
		bool upToDate = FileIsUptoDate(outputs[outIdx].path, output) && outputs[outIdx].upToDate;
		if (!upToDate)
		{
			buildScript += ECHO_SUBTITLE(outputs[outIdx].name + " is linking...");
			string command = LINKER;
			command += LINK_INPUT(outputs[outIdx].path);
			command += LINK_OUTPUT(output);
			command += LINK_ESDB;
			command += LINK_FLAGS;

			buildScript += command + '\n';

			AddInstallLog(output);
		}
	}
}

bool BuildPatches(const string& path, const vector<CompileStructure>& patches, const CompileStructure& global, string& buildScript)
{
	buildScript += ECHO_TITLE("Building Patches...");

	OutputObject globalOut = CompileCompStruct(BUILD_DIR, global, buildScript, nullptr);
	if (globalOut.path.empty())
		return false;

	vector<OutputObject> patchesOut;
	unsigned int upToDate = 0;
	for (size_t patchIdx = 0; patchIdx < patches.size(); ++patchIdx)
	{
		OutputObject output = CompileCompStruct(BUILD_DIR, patches[patchIdx], buildScript, &globalOut);
		if (OUTPUT_INVALID(output))
			continue;
		if (output.upToDate)
			++upToDate;
		patchesOut.push_back(output);
	}
	if (patchesOut.empty())
	{
		printf_s("No compilable data found in any patch!\n");
		return true;
	}
	if (patchesOut.size() == upToDate)
		buildScript += ECHO_SUBTITLE("All patches are up to date!");

	LinkCompOutputs(path, patchesOut, buildScript);

	return true;
}
bool BuildLibraries(const string& path, const vector<CompileObject>& libraries, string& buildScript)
{
	if (!libraries.empty())
		buildScript += ECHO_TITLE("Building Libraries...");

	vector<OutputObject> libsOut;
	unsigned int upToDate = 0;
	for (size_t libIdx = 0; libIdx < libraries.size(); ++libIdx)
	{
		OutputObject output = CompileCompObject(BUILD_DIR, libraries[libIdx], buildScript);
		if (OUTPUT_INVALID(output))
			continue;
		if (output.upToDate)
			++upToDate;

		libsOut.push_back(output);
	}
	if (libsOut.empty())
	{
		printf_s("No compilable data found in any library!\n");
		return true;
	}
	if (libsOut.size() == upToDate)
		buildScript += ECHO_SUBTITLE("All libraries are up to date!");

	LinkCompOutputs(path, libsOut, buildScript);

	return true;
}

string AddScriptSettings(const string& path, string& buildScript)
{
	FILE* file = nullptr;
	fopen_s(&file, path.c_str(), "r");
	if (!file)
	{
		printf_s("Couldn't open %s\n", path.c_str());
		printf_s("	Error opening build settings file\n");
		return string();
	}

	char buffer[BUILD_SETTINGS_MAX_SIZE];
	memset(buffer, 0, BUILD_SETTINGS_MAX_SIZE);

	size_t read = fread(buffer, sizeof(char), BUILD_SETTINGS_MAX_SIZE, file);
	if (!read)
	{
		printf_s("Couldn't read %s\n", path.c_str());
		printf_s("	Error reading build settings file\n");
		fclose(file);
		return string();
	}

	for (size_t i = 0; i < read; ++i)
		buildScript += buffer[i];
	buildScript += '\n';
	fclose(file);

	size_t start = buildScript.find(BUILD_SETTINGS_PROJECT_DIR);
	if (start == string::npos)
	{
		printf_s("Couldn't read %s\n", path.c_str());
		printf_s("	Project Directory variable missing!\n");
		printf_s("	%sAdd the path to your CTRMap project HERE\n", BUILD_SETTINGS_PROJECT_DIR);
		return string();
	}
	start += sizeof(BUILD_SETTINGS_PROJECT_DIR) - 1;
	size_t end = buildScript.find('\n', start);

	return buildScript.substr(start, end - start);
}
bool CreateScript(const string& path, const string& buildScript)
{
	FILE* file = nullptr;
	fopen_s(&file, path.c_str(), "w");
	if (!file)
	{
		printf_s("Couldn't create script %s\n", path.c_str());
		printf_s("	Error opening file\n");
		return false;
	}

	if (fwrite(buildScript.c_str(), sizeof(char), buildScript.size(), file) != buildScript.size())
	{
		printf_s("Couldn't create script %s\n", path.c_str());
		printf_s("	Error writing file\n");
		fclose(file);
		return false;
	}

	fclose(file);
	return true;
}

void MoveData(const string& directory, const string& output, const string& folder = string())
{
	string path = PathConcat(directory, folder);

	vector<string> entries = FolderGetList(path);
	if (!entries.empty())
	{
		for (size_t entryIdx = 0; entryIdx < entries.size(); ++entryIdx)
		{
			const string& entry = entries[entryIdx];
			string assetFolder = entry.substr(directory.length() + 1);

			if (filesystem::is_directory(entry))
			{
				MoveData(directory, output, assetFolder);
				continue;
			}

			if (!FIND_ASSET(assetFolder, dataAssets))
				continue;

			string outputPath = PathConcat(output, folder);
			if (!FolderCreate(outputPath))
				continue;
			
			string outputFile = PathConcat(output, assetFolder);
			if (FileIsUptoDate(entry, outputFile))
				continue;

			std::error_code ec;
			if (!filesystem::copy_file(entry, outputFile, filesystem::copy_options::overwrite_existing, ec))
			{
				printf_s("Error copying file %s to %s (%s)\n", entry.c_str(), outputFile.c_str(), ec.message().c_str());
				continue;
			}

			printf("Added file: %s \n", outputFile.c_str());
			AddInstallLog(outputFile);
		}
	}
}

bool Build() 
{
	if (!FolderCreate(BUILD_DIR))
		return false;

	CompileStructure global;
	if (!GetFolderCompilationData(GLOBAL_DIR, global))
		return false;

	vector<CompileStructure> patches;
	if (!GetPatchesCompilationData(PATCH_DIR, patches))
		return false;

	vector<CompileObject> libraries;
	GetLibrariesCompilationData(LIB_DIR, libraries);

	string buildScript = "@ECHO OFF\n";
	string projectDir = AddScriptSettings(BUILD_SETTINGS_PATH, buildScript);
	if (projectDir.empty())
		return false;

	if (!BuildPatches(PROJECT_PATCH_PATH(projectDir), patches, global, buildScript))
		return false;
	if (!BuildLibraries(PROJECT_LIB_PATH(projectDir), libraries, buildScript))
		return false;

	if (!CreateScript(SCRIPT_PATH, buildScript))
		return false;

	MoveData(DATA_PATH, PROJECT_DATA_PATH(projectDir));

	system(SCRIPT_PATH.c_str());
	return true;
}
bool Clear()
{
	if (!PathRemove(BUILD_DIR))
		return false;

	return true;
}
bool Rebuild()
{
	if (!Build())
		return false;
	if (!Clear())
		return false;
	return true;
}
bool Uninstall()
{
	for (size_t log = 0; log < installLog.size(); ++log)
		if (PathRemove(installLog[log]))
			printf("Removed file: %s\n", installLog[log].c_str());

	installLog.clear();

	if (!Clear())
		return false;
	return true;
}

#define BUILD_COMMAND "build"
#define REBUILD_COMMAND "rebuild"
#define CLEAR_COMMAND "clear"
#define UNINSTALL_COMMAND "clear"
int main(int argc, char* argv[])
{
	LoadInstallLog(BUILD_INSTALL_LOG);

	if (argc <= 1)
		Build();
	else
	{
		if (strcmp(argv[1], BUILD_COMMAND))
			Build();
		else if (strcmp(argv[1], REBUILD_COMMAND))
			Rebuild();
		else if (strcmp(argv[1], CLEAR_COMMAND))
			Clear();
		else if (strcmp(argv[1], UNINSTALL_COMMAND))
			Uninstall();
	}

	SaveInstallLog(BUILD_INSTALL_LOG);
}