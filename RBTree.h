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
	K _key; //关键字
	V _value; //对应的值
	Colour _col; //节点的颜色
	RBTreeNode<K, V>* _left;//左孩子节点
	RBTreeNode<K, V>* _right;//右孩子节点
	RBTreeNode<K, V>* _parent;//父亲节点
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
		//查找插入的位置
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

		//将节点插入
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

		//调整满足红黑树的性质
		while (cur != _root&&parent->_col == RED)
		{
			Node* grandfather = parent->_parent;
			if (grandfather->_left == parent)
			{
				//case1父节点P和叔父节点U二者都是红色
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
					//case3父节点P是红色而叔父节点U是黑色或缺少，
					//并且新节点N是其父节点P的右子节点而父节点P又是其父节点的左子节点。
					if (cur == parent->_right)
					{
						RotateL(parent);
						swap(parent, cur);
					}

					//case2父节点P是红色而叔父节点U是黑色或缺少，
					//新节点N是其父节点的左子节点，而父节点P又是其父节点G的左子节点。
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
	//检验是不是红黑树
	bool _CheckRBTree(Node* root, int k, int count)
	{
		if (root == NULL)
			return true;

		if (root->_col == RED&&root->_parent->_col == RED)
		{
			cout << "连续的红节点" << root->_parent->_key << endl;
			return false;
		}

		if (root->_col == BLACK)
			++count;

		if (root->_left == NULL&&root->_right == NULL)
		{
			if (count != k)
			{
				cout << "黑色节点数量不同" << root->_key << endl;
				return false;
			}

		}

		return _CheckRBTree(root->_left, k, count)
			&& _CheckRBTree(root->_right, k, count);
	}
	//中序递归遍历
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
	//右单旋
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
	//左单旋
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