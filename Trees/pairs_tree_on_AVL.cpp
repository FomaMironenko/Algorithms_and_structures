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

	Bnode* brother;
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

	Bnode<T>* insert(T key)
	{
		return _insert(&root, key);
	}
	//recursive
	Bnode<T>* _insert(Bnode<T>** cur, T key)
	{
		if (!*cur)
		{
			*cur = new Bnode<T>(key);
			return *cur;
		}

		else if (key < (*cur)->key)
		{
			Bnode<T>* tmp = _insert(&((*cur)->left), key);
			balance(cur);
			return tmp;
		}
		else if (key > (*cur)->key)
		{
			Bnode<T>* tmp = _insert(&((*cur)->right), key);
			balance(cur);
			return tmp;
		}
		else
		{
			return nullptr;
		}
	}


	void del(T key)
	{
		del_find(&root, key);
	}

	void __del__(Bnode<T>** cur)
	{
		if ((*cur)->left && (*cur)->right)
		{
			get_left(&((*cur)->right), *cur);
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

	void get_left(Bnode<T>** cur, Bnode<T>* acceptor)
	{
		if (!(*cur)->left)
		{
			acceptor->key = (*cur)->key;
			(*cur)->brother->brother = acceptor;
			acceptor->brother = (*cur)->brother;
			__del__(cur);
			balance(cur);
			return;
		}
		get_left(&((*cur)->left), acceptor);
		balance(cur);
	}

	void del_find(Bnode<T>** cur, T key)
	{
		if (!*cur)
			return;
		if (key < (*cur)->key)
		{
			del_find(&((*cur)->left), key);
			balance(cur);
			return;
		}
		if (key > (*cur)->key)
		{
			del_find(&((*cur)->right), key);
			balance(cur);
			return;
		}
		if ((*cur)->key == key)
		{
			__del__(cur);
			balance(cur);
		}
	}


	pair<T*, T*> find(T key)
	{
		return _find(&root, key);
	}

	pair<T*, T*> _find(Bnode<T>** cur, T key)
	{
		if (!*cur)
		{
			return pair<T*, T*>(nullptr, nullptr);
		}
		if (key < (*cur)->key)
		{
			return _find(&((*cur)->left), key);
		}
		if (key > (*cur)->key)
		{
			return _find(&((*cur)->right), key);
		}
		if ((*cur)->key == key)
		{
			return pair<T*, T*>(&((*cur)->key), &((*cur)->brother->key));
		}
	}

	void dfs(Bnode<T>* vertex, bool first)
	{
		if (vertex == 0)
			return;
		dfs(vertex->left, first);
		if (first)
		{
			cout << vertex->key << " " << vertex->brother->key << endl;
		}
		else
		{
			cout << vertex->brother->key << " " << vertex->key << endl;
		}
		dfs(vertex->right, first);
	}

	void print(bool first)
	{
		dfs(root, first);
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


template<typename T>
struct Pair_tree
{
	Pair_tree()
	{
		First = new AVL<T>();
		Second = new AVL<T>();
	}

	Pair_tree(T f, T s)
	{
		First = new AVL(f);
		Second = new AVL(s);
		First->root->brother = Second->root->brother;
		Second->root->brother = First->root->brother;
	}


	///// 1 /////
	pair<T*, T*> find_by_first(T key1)
	{
		return First->find(key1);
	}
	///// 2 /////
	pair<T*, T*> find_by_second(T key2)
	{
		pair<T*, T*> tmp = Second->find(key2);
		return pair<T*, T*>(tmp.second, tmp.first);
	}
	///// 3 /////
	void insert(T key1, T key2)
	{
		Bnode<T>* ftmp = First->insert(key1);
		Bnode<T>* stmp = nullptr;
		if (ftmp != nullptr)
		{
			stmp = Second->insert(key2);
		}
		else
		{
			return;
		}
		if (stmp == nullptr)
		{
			First->del(key1);
			return;
		}
		ftmp->brother = stmp;
		stmp->brother = ftmp;
	}
	///// 4 /////
	void del_by_first(T key1)
	{
		pair<T*, T*> tmp = this->find_by_first(key1);
		if (tmp.first == nullptr)
		{
			return;
		}
		First->del(*tmp.first);
		Second->del(*tmp.second);
	}
	///// 5 /////
	void del_by_second(T key2)
	{
		pair<T*, T*> tmp = this->find_by_second(key2);
		if (tmp.second == nullptr)
		{
			return;
		}
		First->del(*tmp.first);
		Second->del(*tmp.second);
	}


	AVL<T>* First;
	AVL<T>* Second;
};


int main()
{
	Pair_tree<int> a;
	a.insert(2, 3);
	a.insert(3, 5); 
	a.insert(3, 6); // will not be added
	a.insert(0, 5); // will not be added
	a.insert(10, -2);
	a.insert(8, 7);
	a.insert(5, 10);

	a.del_by_second(3);
	a.del_by_first(5);
	a.insert(5, 3);

	cout << "\n** order by first  **\n";
	a.First->print(true);
	cout << "\n** order by second **\n";
	a.Second->print(false);


	return EXIT_SUCCESS;
}