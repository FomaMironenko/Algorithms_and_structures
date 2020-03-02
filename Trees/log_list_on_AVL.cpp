#include "pch.h"
#include <iostream>

using namespace std;

int max(int a, int b)
{
	return a > b ? a : b;
}


template<typename T>
struct Bnode
{
	Bnode(T data) : data(data), l(0), r(0), left(0), right(0), height(1)
	{	}

	int get_height()
	{
		return this != NULL ? this->height : 0;
	}
	int leftH()
	{
		return this->left->get_height();
	}
	int rightH()
	{
		return this->right->get_height();
	}
	int corr_height()
	{
		return this->height = 1 + ::max(this->left->get_height(), this->right->get_height());
	}
	int get_size()
	{
		return this != NULL ? 1 + l + r : 0;
	}
	int leftS()
	{
		return this->l = this->left->get_size();
	}
	int rightS()
	{
		return this->r = this->right->get_size();
	}

	T data;
	int l; // the number of elements in the left subtree
	int r; // the number of elements in the right subtree
	int height;
	Bnode* left;
	Bnode* right;
};


template<typename T>
struct AVL
{
	AVL(T data)
	{
		root = new Bnode<T>(data);
	}
	AVL()
	{
		root = 0;
	}

	Bnode<T>* left_turn(Bnode<T>* cur)
	{
		//cur->right - cur->left = 2
		Bnode<T>* tmp = cur->right;
		cur->r = tmp->l;
		cur->right = tmp->left;
		tmp->l = cur->l + cur->r + 1;
		tmp->left = cur;
		cur->height = cur->corr_height();
		tmp->height = tmp->corr_height();
		return tmp;
	}

	Bnode<T>* right_turn(Bnode<T>* cur)
	{
		//cur->right - cur->left = -2
		Bnode<T>* tmp = cur->left;
		cur->l = tmp->r;
		cur->left = tmp->right;
		tmp->r = cur->l + cur->r + 1;
		tmp->right = cur;
		cur->height = cur->corr_height();
		tmp->height = tmp->corr_height();
		return tmp;
	}

	void balance(Bnode<T>** cur)
	{
		//left turn
		if (!*cur) { return; }
		(*cur)->l = (*cur)->leftS();
		(*cur)->r = (*cur)->rightS();
		if ((*cur)->rightH() - (*cur)->leftH() == 2)
		{
			if ((*cur)->right->leftH() > (*cur)->right->rightH())
			{
				(*cur)->right = right_turn((*cur)->right);
			}
			(*cur) = left_turn(*cur);
		}

		if ((*cur)->leftH() - (*cur)->rightH() == 2)
		{
			if ((*cur)->left->rightH() > (*cur)->left->leftH())
			{
				(*cur)->left = left_turn((*cur)->left);
			}
			(*cur) = right_turn(*cur);
		}
	}

	////////// 1 //////////

	T & operator[](int pos)
	{
		if (!root) { return buffer; }
		return find(&root, pos, root->l);
	}

	T & find(Bnode<T>** cur, int pos, int cur_pos)
	{
		if (!*cur)
		{
			return buffer;
		}
		if (pos < cur_pos)
			// (*cur)->left != 0
		{
			return find(&((*cur)->left), pos, cur_pos - 1 - (*cur)->left->rightS());
		}
		else if (pos > cur_pos)
		{
			if (!(*cur)->right)
			{
				return buffer;
			}
			return find(&((*cur)->right), pos, cur_pos + 1 + (*cur)->right->leftS());
		}
		else
		{
			return (*cur)->data;
		}
	}
	///////////////////////



	////////// 2 //////////

	// if pos is more then the number of elements, new element will be added to the end
	void insert(int pos, T data)
	{
		if (pos < 0) { return; }
		if (!root)
		{
			root = new Bnode<T>(data);
			return;
		}
		_insert(&root, pos, data, root->l);
	}

	// cur_pos = number of elements from the left (numeration starts with 0)
	void _insert(Bnode<T>** cur, int pos, T data, int cur_pos)
	{
		if (pos < cur_pos)
			// (*cur)->left != 0
		{
			_insert(&((*cur)->left), pos, data, cur_pos - 1 - (*cur)->left->rightS());
		}
		else if (pos > cur_pos)
		{
			if ((*cur)->right == 0)
				// means that (*cur) is the right element
			{
				(*cur)->right = new Bnode<T>(data);
			}
			else
			{
				_insert(&((*cur)->right), pos, data, cur_pos + 1 + (*cur)->right->leftS());
			}
		}
		else
		{
			if ((*cur)->left == 0)
			{
				(*cur)->left = new Bnode<T>(data);
			}
			else
			{
				get_right(&((*cur)->left), data);
			}
		}
		balance(cur);
	}

	void get_right(Bnode<T>** cur, T data)
	{
		if (!(*cur)->right)
		{
			(*cur)->right = new Bnode<T>(data);
			balance(cur);
			return;
		}
		get_right(&((*cur)->right), data);
		balance(cur);
	}

	///////////////////////


	////////// 3 //////////

	void del(int pos)
	{
		if (!root) { return; }
		del_find(&root, pos, root->l);
	}

	void del_find(Bnode<T>** cur, int pos, int cur_pos)
	{
		if (!*cur)
			return;
		if (pos < cur_pos)
			// (*cur)->left != 0
		{
			del_find(&((*cur)->left), pos, cur_pos - 1 - (*cur)->left->rightS());
		}
		else if (pos > cur_pos)
		{
			if (!(*cur)->right)
			{
				return;
			}
			del_find(&((*cur)->right), pos, cur_pos + 1 + (*cur)->right->leftS());
		}
		else
		{
			__del__(cur);
		}
		balance(cur);
	}

	void __del__(Bnode<T>** cur)
	{
		if ((*cur)->left && (*cur)->right)
		{
			get_left(&((*cur)->right), &((*cur)->data));
			balance(cur);
			return;
		}
		if (!(*cur)->left && (*cur)->right)
		{
			Bnode<T>* tmp = *cur;
			*cur = (*cur)->right;
			delete tmp;
			(*cur)->height = (*cur)->corr_height();
			return;
		}
		if ((*cur)->left && !(*cur)->right)
		{
			Bnode<T>* tmp = *cur;
			*cur = (*cur)->left;
			delete tmp;
			(*cur)->height = (*cur)->corr_height();
			return;
		}
		if (!(*cur)->left && !(*cur)->right)
		{
			Bnode<T>* tmp = *cur;
			*cur = 0;
			delete tmp;
			return;
		}
	}

	void get_left(Bnode<T>** cur, T* write_to)
	{
		if (!(*cur)->left)
		{
			*write_to = (*cur)->data;
			__del__(cur);
			balance(cur);
			return;
		}
		get_left(&((*cur)->left), write_to);
		balance(cur);
	}

	//////////////////////


	void dfs(Bnode<T>* vertex)
	{
		if (vertex == 0)
			return;
		dfs(vertex->left);
		cout << vertex->data << "  ";
		dfs(vertex->right);
	}

	void print()
	{
		dfs(root);
	}

	void __deleting_DFS__(Bnode<T>* cur)
	{
		if (!cur)
			return;
		__deleting_DFS__(cur->left);
		__deleting_DFS__(cur->right);
		if (cur != root)
			delete cur;
	}

	~AVL()
	{
		__deleting_DFS__(root);
		delete root;
	}

	T buffer = 0;
	Bnode<T>* root;
};


int main()
{
	AVL<int> a;
	char tp;
	int i, d;
	cout << "To insert enter: i index value\nTo delete enter: d index\n";
	cout << "Example:\ni 0 2\nd 0\n\n";

	while (cin >> tp)
	{
		if (tp == 'i')
		{
			cin >> i >> d;
			a.insert(i, d);
			a.print(); cout << "\n";
		}
		else if (tp == 'd')
		{
			cin >> i;
			a.del(i);
			a.print(); cout << "\n";
		}
		else
		{
			cout << "Incorrect input";
			break;
		}
	}

	a[0] = 0;
	a[1] = 1;
	a.print();

	return EXIT_SUCCESS;
}
