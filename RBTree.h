#ifndef __RBTREE_H__
#define __RBTREE_H__


enum colour
{
	RED,
	BLACK,
};

template<class K,class V>
struct RBTreeNode
{
	int _col;
	K _key;
	V _value;
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	RBTreeNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _col(RED)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{}

};


template<class K,class V>
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

		Node* parent = NULL;
		Node* cur = _root;
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
				return false;
		}

		//插入位置
		if (parent->_key >key)
		{
			cur = new Node(key, value);
			parent->_left = cur;
			cur->_parent = parent;
		}
		else if (parent->_key < key)
		{
			cur = new Node(key, value);
			parent->_right = cur;
			cur->_parent = parent;
		}

		//插入位置以后，如何调整
		while (cur != _root && parent->_col == RED)
		{
			Node* grandfather = parent->_parent;
			Node* uncle = NULL;
			//左边的情况
			if (parent == grandfather->_left)
			{
				//情况一
				uncle = grandfather->_right;
				if (uncle && uncle->_col == RED)
				{
					//情况1-> 不需要旋转
					if (cur == parent->_left)
					{
						grandfather->_col = RED;
						parent->_col = BLACK;
						uncle->_col = BLACK;

						cur = grandfather;
						parent = cur->_parent;
					}

					//需要旋转
					else if (cur == parent->_right)
					{
						RotateL(parent);
						grandfather->_col = RED;
						parent->_col = BLACK;
						uncle->_col = BLACK;

						cur = grandfather;
						parent = cur->_parent;
					}
					
				}

				//情况2,情况3
				else if (uncle == NULL || (uncle && uncle->_col == BLACK))
				{
					//情况3
					if (cur == parent->_right)
					{
						RotateL(parent);
					}
					parent->_col = BLACK;
					grandfather->_col = RED;
					RotateR(grandfather);
					break;
				}
			}
			//右边的情况
			else if (parent == grandfather->_right)
			{
				//情况1
				uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)
				{
					//不需要旋转
					if (cur == parent->_right)
					{
						uncle->_col = BLACK;
						grandfather->_col = RED;
						parent->_col = BLACK;

						cur = grandfather;
						parent = cur->_parent;
					}

					//旋转
					else if (cur == parent->_left)
					{
						uncle->_col = BLACK;
						grandfather->_col = RED;
						parent->_col = BLACK;
						RotateR(parent);

						cur = grandfather;
						parent = cur->_parent;
					}
				}

				else if (uncle == NULL || (uncle && uncle->_col == BLACK))
				{
					//情况2，3
					if (cur == parent->_left)
					{
						RotateR(parent);
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

	bool isRBTree()
	{
		int blackNodeNum = 0;
		int curBlackNodeNum = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_col == BLACK)
				blackNodeNum++;

			cur = cur->_left;
		}
		return _isRBTree(_root,blackNodeNum,curBlackNodeNum);
	}

	void InOrder()
	{
		_InOrder(_root);
	}

protected:
	bool _isRBTree(Node* root,int blackNodeNum,int curBlackNodeNum)
	{
		if (root == NULL)
			return true;

		if (root->_col == BLACK)
			curBlackNodeNum++;

		if (blackNodeNum == curBlackNodeNum)
		{
			if (root->_parent == NULL)
				return true;
			else if (root->_col == RED && root->_col == root->_parent->_col)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		
		return _isRBTree(root->_left, blackNodeNum, curBlackNodeNum) && _isRBTree(root->_right, blackNodeNum, curBlackNodeNum);
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}
	void RotateL(Node*& parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		subR->_left = parent;
		subR->_parent = parent->_parent;
		parent->_parent = subR;
		parent = subR;

		if (parent->_parent == NULL)
			_root = parent;
		else if (parent->_parent->_key > parent->_key)
		{
			parent->_parent->_left = parent;
		}

		else if (parent->_key > parent->_parent->_key)
		{
			parent->_parent->_right = parent;
		}
	}

	void RotateR(Node*& parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		subL->_right = parent;
		subL->_parent = parent->_parent;
		parent->_parent = subL;

		parent = subL;

		if (parent->_parent == NULL)
			_root = parent;

		else if (parent->_parent->_key > parent->_key)
		{
			parent->_parent->_left = parent;
		}

		else if (parent->_parent->_key < parent->_key)
			parent->_parent->_right = parent;

	}

protected:
	Node* _root;
};



void testRBtree()
{
	RBTree<int, int> rbt;
	int arr[8] = { 2, 5, 12, 16, 18, 26, 3, 1 };
	for (int i = 0; i < 8; i++)
	{
		rbt.Insert(arr[i], i);
	}

	rbt.InOrder();
	cout << endl;

	cout << "isRBTree? ->:" << rbt.isRBTree() << endl;
	
}



#endif //__RBTREE_H__