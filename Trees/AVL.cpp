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
	Bnode(T data = 0) : key(data), left(0), right(0), height(1)
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

	T key;
	int height;
	Bnode* left;
	Bnode* right;
};


template<typename T>
struct AVL
{
	AVL(T key)
	{
		root = new Bnode<T>(key);
	}
	AVL()
	{
		root = 0;
	}

	Bnode<T>* left_turn(Bnode<T>* cur)
	{
		//cur->right - cur->left = 2
		Bnode<T>* tmp = cur->right;
		cur->right = cur->right->left;
		tmp->left = cur;
		cur->height = cur->corr_height();
		return tmp;
	}

	Bnode<T>* right_turn(Bnode<T>* cur)
	{
		//cur->right - cur->left = -2
		Bnode<T>* tmp = cur->left;
		cur->left = cur->left->right;
		tmp->right = cur;
		cur->height = cur->corr_height();
		return tmp;
	}

	void balance(Bnode<T>** cur)
	{
		//left turn
		if (!*cur)
			return;
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
		(*cur)->height = (*cur)->corr_height();
	}

	void insert(T key)
	{
		_insert(&root, key);
	}
	//recursive
	void _insert(Bnode<T>** cur, T key)
	{
		if (!*cur)
		{
			*cur = new Bnode<T>(key);
			return;
		}

		if (key < (*cur)->key)
		{
			_insert(&((*cur)->left), key);
			balance(cur);
		}
		if (key > (*cur)->key)
		{
			_insert(&((*cur)->right), key);
			balance(cur);
		}
		else
		{
			return;
		}
	}


	void __del__(Bnode<T>** cur)
	{
		if ((*cur)->left && (*cur)->right)
		{
			get_left(&((*cur)->right), &((*cur)->key));
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
			*write_to = (*cur)->key;
			__del__(cur);
			balance(cur);
			return;
		}
		get_left(&((*cur)->left), write_to);
		balance(cur);
	}

	void del(T key)
	{
		find(&root, key);
	}
	void find(Bnode<T>** cur, T key)
	{
		if (!*cur)
			return;
		if (key < (*cur)->key)
		{
			find(&((*cur)->left), key);
			balance(cur);
			return;
		}
		if (key > (*cur)->key)
		{
			find(&((*cur)->right), key);
			balance(cur);
			return;
		}
		if ((*cur)->key == key)
		{
			__del__(cur);
			balance(cur);
		}
	}


	void dfs(Bnode<T>* vertex)
	{
		if (vertex == 0)
			return;
		dfs(vertex->left);
		cout << vertex->key << " (";
		if (vertex->left)
		{
			cout << vertex->left->key;
		}
		cout << ",";
		if (vertex->right)
		{
			cout << vertex->right->key;
		}
		cout << ") " << vertex->get_height() << "\n";

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

	Bnode<T>* root;
};


int main()
{
	AVL<int> a;

	char ch;
	int v;
	while (cin >> ch >> v)
	{
		if (ch == 'i')
		{
			a.insert(v);
			a.print();
			cout << "\n";
		}
		else
		{
			a.del(v);
			a.print();
			cout << "\n";
		}
	}
	cout << a.root->get_height() << "\n\n";
	a.print();


	return EXIT_SUCCESS;
}
