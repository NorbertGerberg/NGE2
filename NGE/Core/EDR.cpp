/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "EDR.hpp"
#include "Console.hpp"
#include <iostream>
#include <sstream>
#include <direct.h>
#include <algorithm>
#include <filesystem>

//! Return the tokens value as a std::string
strg EDR::GetString(strg file, strg token, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
	{
		File.close();
		return "Error: Could not find '" + file + "'";
	}

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (newTok == tok.substr(0, tok.find("="))) {
			File.close();
			strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
			std::replace_if(tuk.begin(), tuk.end(), [](char c) { return c == '|'; }, ' ');
			tok.clear();
			newTok.clear();
			return tuk;
		}
	}
	File.close();
	tok.clear();
	newTok.clear();
	return "";
}

//! Return the tokens value as a real6
real6 EDR::GetNumber(strg file, strg token, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
	{
		File.close();
		return 0;
	}
	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (newTok == tok.substr(0, tok.find("="))) {
			File.close();
			return ::atof(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
		}
	}
	return 0;
}

//! Return the tokens value as an int
int EDR::GetInt(strg file, strg token, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
	{
		File.close();
		return 0;
	}

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (newTok == tok.substr(0, tok.find("="))) {
			File.close();
			return ::atoi(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
		}
	}
	return 0;
}

//! Return the tokens value as a bool
bool EDR::GetBool(strg file, strg token, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
	{
		File.close();
		return false;
	}
	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (newTok == tok.substr(0, tok.find("="))) {
			strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
			if (tuk == "false" || tuk == "null" || tuk == "NULL" || tuk == "Null" || tuk == "0") {
				File.close();
				return false;
			}
			else if (tuk == "true" || tuk == "1") {
				File.close();
				return true;
			}
			else {
				File.close();
				return false;
			}
		}
	}
	return false;
}

//! Set a tokens value
const uint EDR::SetValue(strg file, strg value, const bool binary)
{
	strg tok;
	std::ofstream File;
	File.open(file, binary ? std::ios::binary | std::ios::out : std::ios::out);
	if (!File)
	{
		File.close();
		return 0;
	}
	File.clear();
	File << value;
	File.close();
	return 1;
}

//! Change a tokens value
const uint EDR::ChangeValue(strg file, strg token, strg value, const bool binary)
{
	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');

	strg newValue = value;
	std::replace_if(newValue.begin(), newValue.end(), [](char c) { return c == ' '; }, '|');

	std::ifstream _inpfile;
	_inpfile.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!_inpfile)
	{
		_inpfile.close();
		return 0;
	}
	strg _content = strg((std::istreambuf_iterator<char>(_inpfile)), std::istreambuf_iterator<char>());
	_inpfile.close();

	if (_content.find(newTok) != strg::npos)
		_content.replace(_content.find(newTok), newTok.size() + GetString(file, newTok).size() + 1, newTok + "=" + newValue);

	SetValue(file, _content);
	return 1;
}

//! Return the tokens value from memory as a std::string
strg EDR::GetStringI(strg text, strg token, const bool binary)
{
	std::istringstream iss(text);
	strg word = "";
	while (iss >> word)
	{
		strg ww = word.substr(0, word.find("="));
		std::replace_if(ww.begin(), ww.end(), [](char c) { return c == ' '; }, '|');
		if (token == ww) {
			strg tuk = word.replace(word.find(token), token.length() + 1, "");
			if (tuk.find("'") != strg::npos)
			{
				iss.clear();
				word.clear();
				return tuk.replace(tuk.find("'"), 1, " ");
			}
			else
			{
				iss.clear();
				word.clear();
				return tuk;
			}
		}
	}
	return "";
}

//! Return the tokens value from memory as a real6
real6 EDR::GetNumberI(strg text, strg token, const bool binary)
{
	std::istringstream iss(text);
	strg word = "";
	while (iss >> word)
	{
		strg ww = word.substr(0, word.find("="));
		std::replace_if(ww.begin(), ww.end(), [](char c) { return c == ' '; }, '|');
		if (token == ww) {
			iss.clear();
			return ::atof(word.replace(word.find(token), token.length() + 1, "").c_str());
		}
	}
	iss.clear();
	return 0;
}

//! Return the tokens value from memory as an int
int	EDR::GetIntI(strg text, strg token, const bool binary)
{
	std::istringstream iss(text);
	strg word = "";
	while (iss >> word)
	{
		strg ww = word.substr(0, word.find("="));
		std::replace_if(ww.begin(), ww.end(), [](char c) { return c == ' '; }, '|');
		if (token == ww)
			return ::atoi(word.replace(word.find(token), token.length() + 1, "").c_str());
	}
	return 0;
}

//! Return the tokens value from memory as a bool
bool EDR::GetBoolI(strg text, strg token, const bool binary)
{
	std::istringstream iss(text);
	strg word = "";
	while (iss >> word)
	{
		strg ww = word.substr(0, word.find("="));
		std::replace_if(ww.begin(), ww.end(), [](char c) { return c == ' '; }, '|');
		if (token == ww) {
			strg tuk = word.replace(word.find(token), token.length() + 1, "");
			if (tuk == "false" || tuk == "null" || tuk == "NULL" || tuk == "Null" || tuk == "0")
				return false;
			else if (tuk == "true" || tuk == "1")
				return true;
			else
				return false;
		}
	}
	return true;
}

//! Change a tokens value in memory
strg EDR::ChangeValueI(strg text, strg token, strg value, const bool binary)
{
	strg rtVl = text;
	if (rtVl.find(token) != strg::npos)
		rtVl.replace(rtVl.find(token), token.size() + GetStringI(rtVl, token).size() + 1, token + "=" + value);
	return rtVl;
}

//! Convert text to binary string
void EDR::ConvertToBinary(strg& text)
{
	strg rtBin = "";
	for (uint i = 0; i < text.length(); i++)
	{
		//Convert character to ASCII code
		int ascii = (int)text[i];
		strg _tmpBin = "";
		while (ascii > 0)
		{
			if (ascii % 2 == 0)
				_tmpBin = "0" + _tmpBin;
			else
				_tmpBin = "1" + _tmpBin;
			ascii /= 2;
		}

		//Pad with zeros to ensure each binary code is 8 bits
		while (_tmpBin.length() < 8)
			_tmpBin = "0" + _tmpBin;
		rtBin += _tmpBin;
	}

	//Compress
	strg crtBin;
	uint zeros = 0;
	uint ones = 0;
	bool isCounting0 = false;
	bool firstCount = false;
	for (auto& it : rtBin)
	{
		if (it == '0')
		{
			if (!isCounting0 && firstCount)
			{
				isCounting0 = true;
				crtBin += std::to_string(ones);
				ones = 0;
			}

			if (!firstCount)
			{
				firstCount = true;
				crtBin = "0";
			}
			zeros += 1;
		}
		else if (it == '1')
		{
			if (isCounting0 && firstCount)
			{
				isCounting0 = false;
				crtBin += std::to_string(zeros);
				zeros = 0;
			}

			if (!firstCount)
			{
				firstCount = true;
				crtBin = "1";
			}
			ones += 1;
		}
	}
	text = crtBin;
	return;
}

//! Convert binary string to text
void EDR::ConvertToText(strg& binary)
{
	//De-Compress
	strg ucbinary;
	bool isCounting0 = false;
	if (binary.at(0) == '0')
		isCounting0 = true;

	for (auto& it : binary)
	{
		uint amount = ::atoi(strg(1, it).c_str());
		strg out = isCounting0 ? "0" : "1";
		isCounting0 = !isCounting0;

		for (uint i = 0; i < amount; i++)
			ucbinary += out;
	}

	strg rtTxt = "";
	for (uint i = 0; i < ucbinary.length(); i += 8)
	{
		strg byteString = ucbinary.substr(i, 8);
		int decimal = 0;
		for (int f = 0; f < byteString.length(); f++)
			decimal = decimal * 2 + (byteString[f] - '0');
		char character = (char)decimal;
		rtTxt += character;
	}
	binary = rtTxt;
	return;
}

//! Returns a files content
strg EDR::GetContext(strg filePath, const bool binary)
{
	std::ifstream File(filePath, binary ? std::ios::binary | std::ios::in : std::ios::in);
	strg rtVl = strg((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
	File.close();
	return rtVl;
}

//! Removes a file from the drive
void EDR::RemoveFile(const char* filePath)
{
	remove(filePath);
	return;
}

//! Creates a directory on drive
void EDR::CreateDirectoryExt(const char* dir)
{
	//Creating File
	if (_mkdir(dir) == -1) {
		eConsole::PrintLog(eString("EDR::CreateDirectory"), strerror(errno));
		return;
	}
	else {
		eConsole::PrintLog("EDR::CreateDirectory", "Successfully created new directory.");
	}
	return;
}

//! Removes a directory from drive
void EDR::DeleteDirectory(const char* dir)
{
	if (_rmdir(dir) == -1) {
		eConsole::PrintLog("EDR::DeleteDirectory", strerror(errno));
		return;
	}
	else {
		eConsole::PrintLog("EDR::DeleteDirectory", "Successfully deleted directory.");
	}
	return;
}

//! Write a file to drive
void EDR::WriteFile(const char* dir, strg content)
{
	strg tok;
	std::ofstream File(dir);
	File.clear();
	File << content;
	File.close();
	return;
}

//! Write binary text to drive
void EDR::WriteBinary(const char* dir, strg content)
{
	std::ofstream file(dir, std::ios::out | std::ios::binary);
	uint64 size = content.size();
	file.write((char*)&size, sizeof(size));
	file.write(&content[0], size);
	file.close();
	return;
}

//! Read binary text from drive
strg EDR::ReadBinary(const char* dir)
{
	strg rtVl;
	std::ifstream file(dir, std::ios::in | std::ios::binary);
	uint64 size = 0;
	file.read((char*)&size, sizeof(size));
	rtVl.resize(size);
	file.read(&rtVl[0], size);
	file.close();
	return rtVl;
}

//! Write binary text to memory
strg EDR::WriteBinaryToString(strg& content)
{
	std::ostringstream oss(std::ios::out | std::ios::binary);
	uint64_t size = content.size();
	oss.write(reinterpret_cast<const char*>(&size), sizeof(size));
	oss.write(content.data(), size);
	return oss.str();
}

//! Read binary text from memory
strg EDR::ReadBinaryToString(strg& content)
{
	strg rtVl;
	std::istringstream iss(content, std::ios::in | std::ios::binary);
	uint64 size = 0;
	iss.read(reinterpret_cast<char*>(&size), sizeof(size));
	rtVl.resize(size);
	iss.read(&rtVl[0], size);
	return rtVl;
}

//! Open ZIP archive
EDR::Archive* EDR::OpenArchive(const char* directory)
{
	//Open ZIP archive
	int err = 0;
	zip* z = zip_open(directory, 0, &err);

	if (err == ZIP_ER_NOZIP) {
		eConsole::PrintLog("EDR::OpenZIPArchive", "Directory is not a ZIP archive.");
		return {};
	}
	return z;
}

//! Creates ZIP archive
EDR::Archive* EDR::CreateArchive(const char* directory)
{
	//Open ZIP archive
	int err = 0;
	zip* z = zip_open(directory, ZIP_CREATE, &err);

	if (err == ZIP_ER_NOZIP) {
		eConsole::PrintLog("EDR::CreateZIPArchive", "Directory is not a ZIP archive.");
		return {};
	}
	return z;
}

//! Adds file to archive
void EDR::AddFileToArchive(EDR::Archive* z, const char* filename, const char* content)
{
	zip_source_t* s;
	if ((s = zip_source_buffer(z, content, strlen(content), 0)) == NULL ||
		zip_file_add(z, filename, s, 0) < 0) {
		zip_source_free(s);
		eConsole::PrintLog("ERROR::EDR::AddFileToZIP", "Error adding File to zip archive!");
	}
}

//! Adds binary file to archive
void EDR::AddBinaryToArchive(EDR::Archive* z, const char* filename, const char* content, uint64 size)
{
	zip_source_t* s;
	if ((s = zip_source_buffer(z, content, size, 0)) == NULL ||
		zip_file_add(z, filename, s, 0) < 0) {
		zip_source_free(s);
		eConsole::PrintLog("ERROR::EDR::AddFileToZIP", "Error adding File to zip archive!");
	}
}

//! Copies existing file to archive
void EDR::CopyFileToArchive(EDR::Archive* z, const char* filename, const char* filepath)
{
	zip_source_t* imageSource = zip_source_file(z, filepath, 0, 0);
	zip_file_add(z, filename, imageSource, ZIP_FL_ENC_UTF_8);
}

//! Removes a file from the archive
void EDR::RemoveFileFromArchive(EDR::Archive* z, const char* filename)
{
	zip_int64_t index = zip_name_locate(z, filename, 0);
	if (index < 0)
	{
		eConsole::PrintLog("ERROR::EDR::RemoveFileFromZIP", "Failed to locate file in ZIP Archive.");
		eConsole::WriteToDisk();
		return;
	}

	if (zip_delete(z, static_cast<zip_uint64_t>(index)) < 0)
	{
		eConsole::PrintLog("ERROR::EDR::RemoveFileFromZIP", "Failed to delete file from ZIP Archive.");
		eConsole::WriteToDisk();
	}
}

//! Returns a files content from an archive. WARNING: RELEASE contents AFTER USE!! MEMORY LEAK!!
EDR::ZSTAT EDR::GetArchiveFileContents(EDR::Archive* z, const char* name)
{
	//Search for the file
	struct zip_stat st;
	zip_stat_init(&st);
	zip_stat(z, name, 0, &st);

	//Alloc memory for its uncompressed contents
	char* contents = new char[st.size];

	//Read the compressed file
	zip_file* f = zip_fopen(z, name, 0);
	zip_fread(f, contents, st.size);
	zip_fclose(f);

	//WARNING: RELEASE contents AFTER USE!! MEMORY LEAK!!
	return ZSTAT(contents, static_cast<zip_uint16_t>(st.size));
}

//! Copies archive file contents
void EDR::CopyArchiveFileContents(EDR::Archive* z, const char* name, const char* newPath, const bool isBinary)
{
	//Search for the file
	struct zip_stat st;
	zip_stat_init(&st);
	zip_stat(z, name, 0, &st);

	//Alloc memory for its uncompressed contents
	char* contents = new char[st.size];

	//Read the compressed file
	zip_file* f = zip_fopen(z, name, 0);
	zip_fread(f, contents, st.size);
	zip_fclose(f);

	std::ofstream endFile(newPath, isBinary ? (std::ios::out | std::ios::binary) : std::ios::out);
	endFile.write(contents, st.size);
	endFile.close();
	delete[] contents;
	return;
}

//! Copies archive file contents
strg EDR::CopyArchiveFileContents(EDR::Archive* z, const char* name)
{
	//Search for the file
	struct zip_stat st;
	zip_stat_init(&st);
	zip_stat(z, name, 0, &st);

	//Alloc memory for its uncompressed contents
	char* contents = new char[st.size];

	//Read the compressed file
	zip_file* f = zip_fopen(z, name, 0);
	zip_fread(f, contents, st.size);
	zip_fclose(f);

	strg rtStrg = "";
	for (int i = 0; i < st.size; i++)
		rtStrg += contents[i];

	delete[] contents;
	return rtStrg;
}

//! Closes archive. Changes to archive only take effect once this is called. Memory of written files must be accessable to this process!
zip* EDR::CloseArchive(EDR::Archive* z)
{
	//Close the archive
	zip_close(z);
	return z;
}

bool EDR::FileExists(const char* path)
{
	std::ifstream file(path);
	if (file.is_open()) {
		file.close();
		return true;
	}
	file.close();
	return false;
}

/**
	nt: 0 = full; 1 = name; 2 = extension;
*/
std::vector<strg> EDR::GetFilesInDir(strg dir, const int nt)
{
	if (std::filesystem::exists(dir))
	{
		std::vector<strg> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(dir))
		{
			if (entry.is_regular_file())
			{
				strg pb = entry.path().string();
				switch (nt)
				{
				case 1: {pb = entry.path().stem().string(); }break;
				case 2: {pb = entry.path().extension().string(); }break;
				}
				rtVl.push_back(pb);
			}
		}
		return rtVl;
	}
	return {};
}

//! For reading in a group
strg EDR::GetStringG(strg file, strg token, strg group, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return "Error: Could not find '" + file + "'";
	bool isGroup = false;
	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
				std::replace_if(tuk.begin(), tuk.end(), [](char c) { return c == '|'; }, ' ');
				File.close();
				return tuk;
			}
		}
	}
	File.close();
	return "";
}

//! For reading in a group
real6 EDR::GetNumberG(strg file, strg token, strg group, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return 0;
	bool isGroup = false;
	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				File.close();
				return ::atof(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
			}
		}
	}
	return 0;
}

//! For reading in a group
int EDR::GetIntG(strg file, strg token, strg group, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return 0;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	bool isGroup = false;
	while (File >> tok)
	{
		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				File.close();
				return ::atoi(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
			}
		}
	}
	return 0;
}

//! For reading in a group
bool EDR::GetBoolG(strg file, strg token, strg group, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
				if (tuk == "false" || tuk == "null" || tuk == "NULL" || tuk == "Null" || tuk == "0") {
					File.close();
					return false;
				}
				else if (tuk == "true" || tuk == "1") {
					File.close();
					return true;
				}
				else {
					File.close();
					return false;
				}
			}
		}
	}
	return false;
}

//! For reading in a sub-group
strg EDR::GetStringSG(strg file, strg token, strg group, strg groupS, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return "Error: Could not find '" + file + "'";
	bool isGroups = false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	strg newGroups = groupS;
	std::replace_if(newGroups.begin(), newGroups.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("<" + newGroups + ">") != strg::npos && !isGroups && isGroup)
			isGroups = true;

		if (tok.find("</" + newGroups + ">") != strg::npos && isGroups && isGroup)
			isGroups = false;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup && isGroups)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				File.close();
				strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
				std::replace_if(tuk.begin(), tuk.end(), [](char c) { return c == '|'; }, ' ');
				return tuk;
			}
		}
	}
	File.close();
	return "";
}

//! For reading in a sub-group
real6 EDR::GetNumberSG(strg file, strg token, strg group, strg groupS, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return 0;
	bool isGroups = false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	strg newGroups = groupS;
	std::replace_if(newGroups.begin(), newGroups.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("<" + newGroups + ">") != strg::npos && !isGroups && isGroup)
			isGroups = true;

		if (tok.find("</" + newGroups + ">") != strg::npos && isGroups && isGroup)
			isGroups = false;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup && isGroups)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				File.close();
				return ::atof(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
			}
		}
	}
	return 0;
}

//! For reading in a sub-group
int EDR::GetIntSG(strg file, strg token, strg group, strg groupS, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return 0;
	bool isGroups = false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	strg newGroups = groupS;
	std::replace_if(newGroups.begin(), newGroups.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("<" + newGroups + ">") != strg::npos && !isGroups && isGroup)
			isGroups = true;

		if (tok.find("</" + newGroups + ">") != strg::npos && isGroups && isGroup)
			isGroups = false;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup && isGroups)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				File.close();
				return ::atoi(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
			}
		}
	}
	return 0;
}

//! For reading in a sub-group
bool EDR::GetBoolSG(strg file, strg token, strg group, strg groupS, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return false;
	bool isGroups = false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	strg newGroups = groupS;
	std::replace_if(newGroups.begin(), newGroups.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("<" + newGroups + ">") != strg::npos && !isGroups && isGroup)
			isGroups = true;

		if (tok.find("</" + newGroups + ">") != strg::npos && isGroups && isGroup)
			isGroups = false;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup && isGroups)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
				if (tuk == "false" || tuk == "null" || tuk == "NULL" || tuk == "Null" || tuk == "0") {
					File.close();
					return false;
				}
				else if (tuk == "true" || tuk == "1") {
					File.close();
					return true;
				}
				else {
					File.close();
					return false;
				}
			}
		}
	}
	return false;
}

//!
uint EDR::GroupItemAmount(strg file, strg group, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return 0;

	uint rtAmount = 0;
	bool isGroup = false;
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("<") != strg::npos && tok.find("</") == strg::npos && isGroup)
			rtAmount += 1;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
		{
			isGroup = false;
			break;
		}
	}
	return rtAmount;
}

//! From memory
uint EDR::GroupItemAmountI(strg file, strg group, const bool binary)
{
	uint rtAmount = 0;
	bool isGroup = false;
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');

	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<") != strg::npos && isGroup && tok.find("</") == strg::npos)
			rtAmount += 1;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
		{
			isGroup = false;
			break;
		}
	}
	return rtAmount;
}

strg EDR::GroupItemName(strg file, strg group, uint place, const bool binary)
{
	strg tok;
	std::ifstream File;
	File.open(file, binary ? std::ios::binary | std::ios::in : std::ios::in);
	if (!File)
		return "ERROR: FILE NOT FOUND!";

	uint rtAmount = 0;
	bool isGroup = false;
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	while (File >> tok)
	{
		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
		{
			isGroup = false;
			break;
		}

		if (tok.find("<") != strg::npos && tok.find("</") == strg::npos && isGroup)
		{
			if (rtAmount == place)
			{
				tok = tok.replace(tok.begin(), tok.begin() + 1, "");
				tok = tok.replace(tok.end() - 1, tok.end(), "");
				return tok;
			}
			rtAmount += 1;
		}

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;
	}
	return "";
}

//! From memory
strg EDR::GroupItemNameI(strg file, strg group, uint place, const bool binary)
{
	uint rtAmount = 0;
	bool isGroup = false;
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<") != strg::npos && isGroup && tok.find("</") == strg::npos)
		{
			if (rtAmount == place)
			{
				tok = tok.replace(tok.begin(), tok.begin() + 1, "");
				tok = tok.replace(tok.end() - 1, tok.end(), "");
				return tok;
			}
			rtAmount += 1;
		}

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
		{
			isGroup = false;
			break;
		}
	}
	return "";
}

//! From memory
strg EDR::GetStringGI(strg file, strg token, strg group, const bool binary)
{
	bool isGroup = false;
	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
				std::replace_if(tuk.begin(), tuk.end(), [](char c) { return c == '|'; }, ' ');
				return tuk;
			}
		}
	}
	return "";
}

//! From memory
real6 EDR::GetNumberGI(strg file, strg token, strg group, const bool binary)
{
	bool isGroup = false;
	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				return ::atof(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
			}
		}
	}
	return 0;
}

//! From memory
int EDR::GetIntGI(strg file, strg token, strg group, const bool binary)
{
	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	bool isGroup = false;
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				return ::atoi(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
			}
		}
	}
	return 0;
}

//! From memory
bool EDR::GetBoolGI(strg file, strg token, strg group, const bool binary)
{
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
				if (tuk == "false" || tuk == "null" || tuk == "NULL" || tuk == "Null" || tuk == "0") {
					return false;
				}
				else if (tuk == "true" || tuk == "1") {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	return false;
}

//! From memory
strg EDR::GetStringSGI(strg file, strg token, strg group, strg groupS, const bool binary)
{
	bool isGroups = false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	strg newGroups = groupS;
	std::replace_if(newGroups.begin(), newGroups.end(), [](char c) { return c == ' '; }, '|');
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<" + newGroups + ">") != strg::npos && !isGroups && isGroup)
			isGroups = true;

		if (tok.find("</" + newGroups + ">") != strg::npos && isGroups && isGroup)
			isGroups = false;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup && isGroups)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
				std::replace_if(tuk.begin(), tuk.end(), [](char c) { return c == '|'; }, ' ');
				return tuk;
			}
		}
	}
	return "";
}

//! From memory
real6 EDR::GetNumberSGI(strg file, strg token, strg group, strg groupS, const bool binary)
{
	bool isGroups = false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	strg newGroups = groupS;
	std::replace_if(newGroups.begin(), newGroups.end(), [](char c) { return c == ' '; }, '|');
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<" + newGroups + ">") != strg::npos && !isGroups && isGroup)
			isGroups = true;

		if (tok.find("</" + newGroups + ">") != strg::npos && isGroups && isGroup)
			isGroups = false;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup && isGroups)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				return ::atof(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
			}
		}
	}
	return 0;
}

//! From memory
int EDR::GetIntSGI(strg file, strg token, strg group, strg groupS, const bool binary)
{
	bool isGroups = false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	strg newGroups = groupS;
	std::replace_if(newGroups.begin(), newGroups.end(), [](char c) { return c == ' '; }, '|');
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<" + newGroups + ">") != strg::npos && !isGroups && isGroup)
			isGroups = true;

		if (tok.find("</" + newGroups + ">") != strg::npos && isGroups && isGroup)
			isGroups = false;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup && isGroups)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				return ::atoi(tok.replace(tok.find(newTok), newTok.length() + 1, "").c_str());
			}
		}
	}
	return 0;
}

//! From memory
bool EDR::GetBoolSGI(strg file, strg token, strg group, strg groupS, const bool binary)
{
	bool isGroups = false;
	bool isGroup = false;

	strg newTok = token;
	std::replace_if(newTok.begin(), newTok.end(), [](char c) { return c == ' '; }, '|');
	strg newGroup = group;
	std::replace_if(newGroup.begin(), newGroup.end(), [](char c) { return c == ' '; }, '|');
	strg newGroups = groupS;
	std::replace_if(newGroups.begin(), newGroups.end(), [](char c) { return c == ' '; }, '|');
	std::istringstream iss(file);
	strg tok = "";
	while (iss >> tok)
	{
		if (tok.find("<" + newGroups + ">") != strg::npos && !isGroups && isGroup)
			isGroups = true;

		if (tok.find("</" + newGroups + ">") != strg::npos && isGroups && isGroup)
			isGroups = false;

		if (tok.find("<" + newGroup + ">") != strg::npos && !isGroup)
			isGroup = true;

		if (tok.find("</" + newGroup + ">") != strg::npos && isGroup)
			isGroup = false;

		if (isGroup && isGroups)
		{
			if (newTok == tok.substr(0, tok.find("="))) {
				strg tuk = tok.replace(tok.find(newTok), newTok.length() + 1, "");
				if (tuk == "false" || tuk == "null" || tuk == "NULL" || tuk == "Null" || tuk == "0") {
					return false;
				}
				else if (tuk == "true" || tuk == "1") {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	return false;
}