#define __HUFFMAN_H__ 
#ifdef __HUFFMAN_H__

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <queue>

template<class W>
struct HuffmanNode
{
	HuffmanNode(W weight = 0)
	:m_left(NULL)
	, m_right(NULL)
	, m_parent(NULL)
	, m_weight(weight)
	{}
	HuffmanNode *m_left;
	HuffmanNode *m_right;
	HuffmanNode *m_parent;
	W m_weight;
};

template<class W>
class HuffmanTree
{
	typedef HuffmanNode<W> Node;
	typedef HuffmanNode<W> *PNode;
	struct cmp
	{
		bool operator()(PNode &left, PNode &right)
		{
			return (left->m_weight) > (right->m_weight);
		}
	};
public:
	HuffmanTree()
		:m_root(NULL)
	{}

	HuffmanTree(std::vector<W> &array)
	{
		CreateHuffmanTree(array);
	}
	~HuffmanTree()
	{
		DestoryHuffmanTree(m_root);
		std::cout << "哈夫曼树销毁成功" << std::endl;
	}
	PNode GetRoot()
	{
		return m_root;
	}
private:
	int CreateHuffmanTree(std::vector<W> &array)
	{
		std::priority_queue<PNode, std::vector<PNode>, cmp> q;
		for (int i = 0; i < 256; i++)
		{
			if (array[i].m_count)
			{
				PNode node = new Node;
				node->m_weight = array[i];
				q.push(node);
			}	
		}
		if (q.size() == 0)
		{
			std::cout << "数组为空" << std::endl;
			m_root = NULL;
			return -1;
		}
		while (q.size() > 1)
		{
			PNode pLeft = q.top();
			std::cout << "Left:" << pLeft->m_weight.m_count << std::endl;
			q.pop();
			PNode pRight = q.top();
			std::cout << "Right:" << pRight->m_weight.m_count << std::endl;
			q.pop();

			PNode pParent = new Node(pRight->m_weight + pLeft->m_weight);
			pParent->m_left = pLeft;
			pLeft->m_parent = pParent;
			pParent->m_right = pRight;
			pRight->m_parent = pParent;
			q.push(pParent);
		}
		m_root = q.top();
		std::cout << "哈夫曼树创建成功" << std::endl;
		return 1;
	}
	int DestoryHuffmanTree(PNode &pRoot)
	{
		if (pRoot)
		{
			DestoryHuffmanTree(pRoot->m_left);
			DestoryHuffmanTree(pRoot->m_right);
			delete pRoot;
			pRoot = NULL;
		}
		return 1;
	}
private:
	PNode m_root;
};
#endif
