#define __COMPRESS_H__
#ifdef __COMPRESS_H__
#include "Huffman.hpp"
#include <string>
typedef long long CountType;

struct CharInformation
{
	bool operator>(const CharInformation& information)
	{
		return this->m_count > information.m_count;
	}
	bool operator!=(const CharInformation& information)
	{
		return this->m_count != information.m_count;
	}
	bool operator==(const CharInformation& information)
	{
		return this->m_count == information.m_count;
	}
	CharInformation operator+(const CharInformation& information)
	{
		CharInformation tmp;
		tmp.m_count = m_count + information.m_count;
		return tmp;
	}
	CharInformation(size_t count = 0)
		:m_count(count)
	{}
	unsigned char m_ch;
	CountType m_count;
	std::string m_code;
};

typedef struct Head
{
	std::string FileExtension;
	std::string Code;
	char StrCount[32];
	size_t LineCount = 0;
}Head;

class Compress
{
	typedef HuffmanNode<CharInformation>* Node;
public:
	Compress()
	{
		m_informations.resize(256);
		for (size_t i = 0; i < 256; ++i)
		{
			m_informations[i].m_ch = i;
		}
		std::cout << "构造成功" << std::endl;
	}
	int compress(const std::string& CompressName);
	int uncompress(const std::string& CompressName);
	std::string GetFileExtensio(const std::string& CompressName);
private:
	int GetHuffmanCode(Node root);
	std::string GetFilepath(const std::string& CompressName);
	std::vector<CharInformation> m_informations;
};

void ReadLine(FILE*fin, std::string& StrInformation);
#endif