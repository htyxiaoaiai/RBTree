#pragma once
#include <iostream>
using namespace std;

enum Colour
{
	RED,
	BLACK,
};

template <class K, class V>
struct RBTreeNode
{
	RBTreeNode(K key, V value)
		:_key(key)
		, _value(value)
		, _col(RED)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{}
public:
	K _key; //�ؼ���
	V _value; //��Ӧ��ֵ
	Colour _col; //�ڵ����ɫ
	RBTreeNode<K, V>* _left;//���ӽڵ�
	RBTreeNode<K, V>* _right;//�Һ��ӽڵ�
	RBTreeNode<K, V>* _parent;//���׽ڵ�
};

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
			_root->_col = BLACK;
			return true;
		}

		Node* cur = _root;
		Node* parent = NULL;
		//���Ҳ����λ��
		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		//���ڵ����
		cur = new Node(key, value);
		if (parent->_key > key)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_right = cur;
			cur->_parent = parent;
		}

		//������������������
		while (cur != _root&&parent->_col == RED)
		{
			Node* grandfather = parent->_parent;
			if (grandfather->_left == parent)
			{
				//case1���ڵ�P���常�ڵ�U���߶��Ǻ�ɫ
				Node* uncle = grandfather->_right;
				if (uncle&&uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					//case3���ڵ�P�Ǻ�ɫ���常�ڵ�U�Ǻ�ɫ��ȱ�٣�
					//�����½ڵ�N���丸�ڵ�P�����ӽڵ�����ڵ�P�����丸�ڵ�����ӽڵ㡣
					if (cur == parent->_right)
					{
						RotateL(parent);
						swap(parent, cur);
					}

					//case2���ڵ�P�Ǻ�ɫ���常�ڵ�U�Ǻ�ɫ��ȱ�٣�
					//�½ڵ�N���丸�ڵ�����ӽڵ㣬�����ڵ�P�����丸�ڵ�G�����ӽڵ㡣
					parent->_col = BLACK;
					grandfather->_col = RED;
					RotateR(grandfather);
					break;
				}
			}
			else
			{
				Node* uncle = grandfather->_left;
				if (uncle&&uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)
					{
						RotateR(parent);
						swap(parent, cur);
					}

					parent->_col = BLACK;
					grandfather->_col = RED;
					RotateL(grandfather);
					break;
				}
			}
		}

		_root->_col = BLACK;
		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}
	//bool Remove(const K& key);
	//Node* Find(const K& key);
	bool CheckRBTree()
	{
		if (_root == NULL)
			return true;

		if (_root->_col == RED)
			return false;

		int k = 0, count = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_col == BLACK)
				k++;

			cur = cur->_left;
		}

		return _CheckRBTree(_root, k, count);
	}
protected:
	//�����ǲ��Ǻ����
	bool _CheckRBTree(Node* root, int k, int count)
	{
		if (root == NULL)
			return true;

		if (root->_col == RED&&root->_parent->_col == RED)
		{
			cout << "�����ĺ�ڵ�" << root->_parent->_key << endl;
			return false;
		}

		if (root->_col == BLACK)
			++count;

		if (root->_left == NULL&&root->_right == NULL)
		{
			if (count != k)
			{
				cout << "��ɫ�ڵ�������ͬ" << root->_key << endl;
				return false;
			}

		}

		return _CheckRBTree(root->_left, k, count)
			&& _CheckRBTree(root->_right, k, count);
	}
	//����ݹ����
	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}
	//�ҵ���
	void RotateR(Node* parent)
	{
		Node* SubL = parent->_left;
		Node* SubLR = SubL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = SubLR;
		if (SubLR)
			SubLR->_parent = parent;

		SubL->_right = parent;
		parent->_parent = SubL;
		SubL->_parent = ppNode;

		if (ppNode == NULL)
		{
			_root = SubL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = SubL;
			}
			else
			{
				ppNode->_right = SubL;
			}
		}
	}
	//����
	void RotateL(Node* parent)
	{
		Node* SubR = parent->_right;
		Node* SubRL = SubR->_left;
		Node* ppNode = parent->_parent;

		parent->_right = SubRL;
		if (SubRL)
			SubRL->_parent = parent;

		SubR->_left = parent;
		parent->_parent = SubR;
		SubR->_parent = ppNode;

		if (ppNode == NULL)
		{
			_root = SubR;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = SubR;
			}
			else
			{
				ppNode->_right = SubR;
			}
		}

	}

private:
	Node*  _root;
};

void TestRBtree()
{
	RBTree<int, int> t;
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		t.Insert(a[i], i);
		cout << "IsRBTree?" << t.CheckRBTree() << endl;
	}

	t.InOrder();
	cout << "IsRBTree?" << t.CheckRBTree() << endl;
}