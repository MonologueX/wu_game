#define _CRT_SECURE_NO_WARNINGS

#include "Compress.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

#define READ_MAXSIZE 1024

std::string Compress::GetFileExtensio(const std::string& CompressName)
{
	size_t pos = CompressName.find_last_of('.');
	return CompressName.substr(pos);
}

std::string Compress::GetFilepath(const std::string& CompressName)
{
	size_t pos = CompressName.find_last_of('.');
	return CompressName.substr(0, pos);
}

int Compress::GetHuffmanCode(Node root)
{
	if (nullptr == root)
	{
		return -1;
	}
	Node cur = root;
	Node Parent = cur->m_parent;
	std::string& code = m_informations[(unsigned char)cur->m_weight.m_ch].m_code;
	while (Parent)
	{
		if (Parent->m_left == cur)
		{
			code += '0';
		}
		else
		{
			code += '1';
		}
		cur = Parent;
		Parent = cur->m_parent;
	}
	reverse(code.begin(), code.end());
	GetHuffmanCode(root->m_left);
	GetHuffmanCode(root->m_right);
	return 0;
}

int Compress::compress(const std::string& CompressName)
{
	///////////////////////////////////////////////
	//  1.��ȡԴ�ַ����ִ���
	///////////////////////////////////////////////
	FILE *fin = fopen(CompressName.c_str(), "rb");
	if (NULL == fin)
	{
		std::cout << "���ļ�ʧ��" << std::endl;
		return -1;
	}
	std::cout << "���ļ��ɹ�" << std::endl;

	char *ReadBuff = new char[READ_MAXSIZE];
	size_t SourceCount = 0;
	while (1)
	{
		size_t ReadSize = fread(ReadBuff, 1, 1024, fin);
		if (0 == ReadSize)
		{
			std::cout << "��ȡ���" << std::endl;
			break;
		}
		std::cout << "��ȡ�ɹ���[ReadSize] = " << ReadSize << std::endl;

		for (size_t i = 0; i < ReadSize; ++i)
		{
			m_informations[(unsigned char)ReadBuff[i]].m_count++;
			//std::cout << m_informations[(unsigned char)ReadBuff[i]].m_count << " ";
			SourceCount++;
		}
	}
	//std::cout << "Դ�ļ������ַ�����" << SourceCount << std::endl;
	/*for (size_t i = 0; i < 256 && m_informations[(unsigned char)ReadBuff[i]].m_count != 0; i++)
		std::cout << m_informations[i].m_count << " ";
	std::cout << std::endl;*/

	///////////////////////////////////////////////
	//  2.������������
	///////////////////////////////////////////////

	HuffmanTree<CharInformation> hf((m_informations));

	//////////////////////////////////////////////
	//  3.����Huffman������ȡHuffman����
	///////////////////////////////////////////////
	
	GetHuffmanCode(hf.GetRoot());
	std::cout << "�����������ȡ�ɹ�" << std::endl;

	///////////////////////////////////////////////
	//  4.��ÿ���ַ��ı����дԴ�ļ�
	///////////////////////////////////////////////
	
	Head head;
	head.FileExtension = GetFileExtensio(CompressName);
	for (size_t i = 0; i < 256; i++)
	{
		if (m_informations[i].m_count != 0)
		{
			head.Code += m_informations[i].m_ch;
			head.Code += ':';
			// itoa((int)m_informations[i].m_count, head.StrCount, 10);
			// Windows ������ itoa
			sprintf(head.StrCount, "%d", (int)m_informations[i].m_count);
			head.Code += head.StrCount;
			head.Code += '\n';
			head.LineCount++;
		}	
	}
	std::cout << "Code ��ȡ���" << std::endl;

	std::string HeadInformation;
	HeadInformation += head.FileExtension;
	HeadInformation += '\n';
	//itoa(head.LineCount, head.StrCount, 10);
	sprintf(head.StrCount, "%d", (int)head.LineCount);
	HeadInformation += head.StrCount;
	HeadInformation += '\n';
	HeadInformation += head.Code;
	string  compressfile = GetFilepath(CompressName);//��ȡ�ļ�ǰ׺
	compressfile += ".ggzip";//Ȼ����ǰ׺�������ѹ��֮��ĺ�׺
	std::cout << "ѹ����:" << compressfile << std::endl;
	FILE* fout = fopen(compressfile.c_str(), "wb");
	assert(fout);

	fwrite(HeadInformation.c_str(), 1, HeadInformation.size(), fout);
	char *WriteBuff = new char[READ_MAXSIZE];
	char pos = 0;
	char ptr = 0;
	size_t WriteSize = 0;
	fseek(fin, 0, SEEK_SET);

	int CompreeCount = 0;
	while (true)
	{
		size_t ReadSize = fread(ReadBuff, 1, READ_MAXSIZE, fin);
		if (0 == ReadSize)
		{
			break;
		}
		for (size_t i = 0; i < ReadSize; ++i)
		{
			string& code = m_informations[(unsigned char)ReadBuff[i]].m_code;
			for (size_t j = 0; j < code.size(); ++j)
			{
				pos++;
				if ('1' == code[j])
				{
					ptr |= 1;
				}
				else
				{
					ptr |= 0;
				}
				if (pos == 8)
				{
					WriteBuff[WriteSize++] = ptr;
					ptr = 0;
					if (READ_MAXSIZE == WriteSize)
					{
						fwrite(WriteBuff, 1, READ_MAXSIZE, fout);
						WriteSize = 0;
					}
					CompreeCount++;
					pos = 0;
				}
				ptr <<= 1;
			}
		}
	}
	
	//���û��8������λ
	if (pos < 8)
	{
		WriteBuff[WriteSize++] = (ptr << (8 - pos));
		CompreeCount++;
	}
	fwrite(WriteBuff, 1, WriteSize, fout);
	fclose(fin);
	fclose(fout);
	cout << "ѹ���˵��ַ�������" << CompreeCount << endl;
	return 1;
}

int Compress::uncompress(const string& CompressName)
{
	FILE* fin = fopen(CompressName.c_str(), "rb");
	assert(fin);

	/////////////////////////////////////////////////////
	// ��ȡѹ���ļ���
	/////////////////////////////////////////////////////

	string StrInformation;
	ReadLine(fin, StrInformation);
	string compressFiltpath = GetFilepath(CompressName);
	compressFiltpath += StrInformation;
	string compressFilepath = GetFilepath(CompressName);
	compressFilepath += StrInformation;

	/////////////////////////////////////////////////////
	// ��ȡ����
	/////////////////////////////////////////////////////
	string StrLineCount;
	ReadLine(fin, StrLineCount);
	size_t LineCount = atoi(StrLineCount.c_str());

	//string StrInformation;
	char ctemp = 0;
	int count = 0;
	//��ÿ���ַ����ֵĴ���
	for (size_t i = 0; i < LineCount; i++)
	{
		StrInformation.clear();
		ReadLine(fin, StrInformation);
		if (i == 255)
		{
			count = atoi(StrInformation.c_str() + 1);
			m_informations[255].m_count = count;
		}
		else
		{
			count = atoi(StrInformation.c_str() + 2);
			m_informations[(unsigned char)StrInformation[0]].m_count = count;//��ÿ���ַ����ֵĴ���
		}
	}

	//FILE* fout = fopen(compressFiltpath.c_str(), "wb");
	FILE* fout = fopen("test.txt", "wb");
	assert(fout);

	/////////////////////////////////////////////////////////////
	// ��ԭHuffman��
	/////////////////////////////////////////////////////////////
	HuffmanTree<CharInformation> ht(m_informations);
	size_t WriteSize = 0;
	size_t pos = 8;
	Node cur = ht.GetRoot();
	CountType FileSize = cur->m_weight.m_count;
	size_t UnCompressCount = 0;

	char* ReadBuff = new char[READ_MAXSIZE];
	char* WriteBuff = new char[READ_MAXSIZE];
	while (true)
	{
		size_t ReadSize = fread(ReadBuff, 1, READ_MAXSIZE, fin);
		if (0 == ReadSize)
		{
			std::cout << "��ȡ���" << std::endl;
			break;
		}
		for (size_t i = 0; i < ReadSize; i++)
		{
			pos = 8;
			while (pos--)
			{
				//Ҷ�ӽ��
				if (NULL == cur->m_left && NULL == cur->m_right)
				{
					WriteBuff[WriteSize++] = cur->m_weight.m_ch;
					if (WriteSize == READ_MAXSIZE)
					{
						fwrite(WriteBuff, 1, READ_MAXSIZE, fout);
						WriteSize = 0;
					}
					if (--FileSize == 0)
					{
						fwrite(WriteBuff, 1, WriteSize, fout);
						break;
					}
					cur = ht.GetRoot();
				}
				if (ReadBuff[i] & (1 << pos))//=1����ʾ�ñ���λΪ1
				{
					cur = cur->m_right;
				}
				else
				{
					cur = cur->m_left;
				}
			}
			UnCompressCount++;
		}
	}
	cout << "��ѹ�˵��ַ�������" << UnCompressCount << endl;
	fclose(fin);
	fclose(fout);
	return 1;
}

void ReadLine(FILE*fin, std::string& StrInformation)
{
	/*char ch;
	while (!feof(fin))
	{
		ch = fgetc(fin);
		if ('\n' != ch)
			return;
		StrInformation += ch;
	}*/
	char next = 0;
	char c = fgetc(fin);
	while (c != '\n')
	{
		StrInformation += c;
		c = fgetc(fin);
	}
	if ((next = fgetc(fin)) == ':')
	{
		char next_next = fgetc(fin);
		if (next_next == ':')
		{
			ungetc(next, fin);
			ungetc(next_next, fin);
		}
		else
		{
			StrInformation += c;
			StrInformation += next;
			StrInformation += next_next;
			ReadLine(fin, StrInformation);
		}
	}
	else
		ungetc(next, fin);
}

int main()
{
	Compress c;
	c.compress("1.txt");
	c.uncompress("1.ggzip");
	return 0;
}