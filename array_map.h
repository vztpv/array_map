#ifndef ARRAY_MAP_H
#define ARRAY_MAP_H

#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <utility>
#include <execution>

// like set, map,   instead of map!
namespace wiz {

	template <class T>
	class ASC {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 < t2; }
	};

	template <class T>
	class EQ { // EE -> EQ!
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 == t2; }
	};

	template <class T>
	inline int test(const T& t1, const T& t2) noexcept {
		if (t1 < t2) {
			return -1;
		}
		else if (t2 < t1) {
			return 1;
		}
		else {
			return 0;
		}
	}

	template <>
	inline int test(const std::string& str1, const std::string& str2) noexcept
	{
		return strcmp(str1.c_str(), str2.c_str());
	}

	template <class Key, class Data>
	class array_map {
		typedef enum _Color { BLACK = 0, RED = 1 } Color;

		template < class Key, class Data >
		class RB_Node // no REMOVE method!
		{
		public:
			long long id = 0; // NULL value? id == -1 ?
			Key first;
			Data second;
			long long  left = 0;
			long long  right = 0;
			long long  p = 0; // parent
			Color color = BLACK;
			long long next = 0; // for dead.. or remain.
			bool dead = false; //
		public:
			void Clear()
			{
				id = 0;
				first = Key();
				second = Data();
				left = 0;
				right = 0;
				p = 0;
				color = RED;
				next = 0;
				dead = false;
			}
		public:
			explicit RB_Node(const Key& key = Key(), const Data& data = Data()) : first(key), second(data), color(BLACK) { }

		public:
			bool operator==(const RB_Node<Key, Data>& other)const {
				return this->id == other.id && other.id != -1;
			}
			bool operator<(const RB_Node<Key, Data>& other) const {
				return this->first < other.first;
			}
		};


		template <class T>
		class TEST // need to rename!
		{
		public:
			inline   long long operator()(const T& t1, const T& t2) const noexcept {
				return test(t1, t2);
			}
		};


		template < class Key, class Data, class COMP = ASC<Key>, class COMP2 = EQ<Key>, class COMP3 = TEST<Key>>
		class RB_Tree
		{
		private:
			inline  bool IsNULL(const RB_Node<Key, Data>& x)const noexcept
			{
				return x.id <= 0 || x.id > arr.size();
			}
			inline  bool IsNULL(const unsigned long long id) const noexcept
			{
				return id <= 0 || id > arr.size();
			}

			void Clear()
			{
				arr.clear();
				arr.push_back(RB_Node<Key, Data>());
				root = 0;
				count = 0;

				remain_list.clear();
			}

		public:
			std::vector<RB_Node<Key, Data>> arr = { RB_Node<Key, Data>() };
		private:
			std::vector<std::pair<Key, Data>> remain_list;

			long long root = 0;
			long long count = 0;
			long long dead_list = 0;

			bool first_time = true;

		public:
			explicit RB_Tree() {  }
			virtual ~RB_Tree() {
				//
			}
			void reserve(size_t num) {
				arr.reserve(num + 1);
				//remain_list.reserve(num);
			}

			using iterator = typename std::vector<RB_Node<Key, Data>>::iterator;
			using const_iterator = typename std::vector<RB_Node<Key, Data>>::const_iterator;

			iterator begin() {
				return arr.begin() + 1;
			}
			const_iterator begin() const {
				return arr.cbegin() + 1;
			}
			iterator end() {
				return arr.end();
			}
			const_iterator end() const {
				return arr.cend();
			}
		private:// LEFT ROTATE Tree x
			void LEFT_ROTATE(RB_Tree<Key, Data, COMP>* tree, RB_Node<Key, Data>* x) {
				//if( x !=NIL<T>::Get() || tree.root != NIL<T>::Get() ) { return; }
				RB_Node<Key, Data>* y;
				// y = right[x]
				y = &(tree->arr[x->right]);
				// right[x] = left[y]
				x->right = y->left;
				// if(left[y] != nil[T]
				if (!IsNULL(tree->arr[y->left])) {
					//    then p[left[y]] = x
					tree->arr[y->left].p = x->id;
				}
				// p[y] = p[x]
				y->p = x->p;

				// if( p[x] = nil[T]
				if (IsNULL(tree->arr[x->p])) {
					//  then root[T] = y
					tree->root = y->id;
				}
				//  else if x = left[p[x]]
				else if (x == &(tree->arr[tree->arr[x->p].left])) {
					//      then left[p[x]] = y
					tree->arr[x->p].left = y->id;
				}
				//  else right[p[x]] = y
				else { tree->arr[x->p].right = y->id; }
				// left[y] = x
				y->left = x->id;
				// p[x] = y
				x->p = y->id;
			}
			// Right ROTATE Tree x // left <> right
			void RIGHT_ROTATE(RB_Tree<Key, Data, COMP>* tree, RB_Node<Key, Data>* x) {
				//if( x !=NIL<T>::Get() || tree.root != NIL<T>::Get() ) { return; }

				RB_Node<Key, Data>* y;
				// y = right[x]
				y = &(tree->arr[x->left]);
				// right[x] = left[y]
				x->left = y->right;
				// if(left[y] != nil[T]
				if (!IsNULL(tree->arr[y->right])) {
					//    then p[left[y]] = x
					tree->arr[y->right].p = x->id;
				}
				// p[y] = p[x]
				y->p = x->p;

				// if( p[x] = nil[T]
				if (IsNULL(tree->arr[x->p])) {
					//  then root[T] = y
					tree->root = y->id;
				}
				//  else if x = left[p[x]]
				else if (x == &(tree->arr[tree->arr[x->p].right])) {
					//      then left[p[x]] = y
					tree->arr[x->p].right = y->id;
				}
				//  else right[p[x]] = y
				else { tree->arr[x->p].left = y->id; }
				// left[y] = x
				y->right = x->id;
				// p[x] = y
				x->p = y->id;
			}
			const RB_Node<Key, Data>* SEARCH(const RB_Node<Key, Data>* x, const Key& k) const
			{
				COMP comp;
				COMP3 comp3;

				while (!IsNULL(*x)) { // != nil
					long long temp = comp3(k, x->first);
					if (temp < 0) { // k < x.key
						x = &arr[x->left];
					}
					else if (temp > 0) {
						x = &arr[x->right];
					}
					else {
						break;
					}
				}

				return x;
			}

			

			RB_Node<Key, Data>* SEARCH(RB_Node<Key, Data>* x, const Key& k)
			{
				COMP3 comp3;
				//long long count = 0;
				while (x->id) { // != nil
					//count++;
					int temp = comp3(k, x->first);
					if (temp < 0) { // k < x.key
						x = &arr[x->left];
					}
					else if (temp > 0) {
						x = &arr[x->right];
					}
					else {
						break;
					}
				}
				//std::cout << count << "\n";
				return x;
			}



			const RB_Node<Key, Data>* SEARCH(const RB_Node<Key, Data>* x, Key&& k, Key* temp) const
			{
				COMP comp;
				COMP3 comp3;

				while (!IsNULL(*x)) { // != nil
					long long temp = comp3(k, x->first);
					if (temp < 0) { // k < x.key
						x = &arr[x->left];
					}
					else if (temp > 0) {
						x = &arr[x->right];
					}
					else {
						break;
					}
				}
				if (temp) {
					*temp = std::move(k);
				}
				return x;
			}
			RB_Node<Key, Data>* SEARCH(RB_Node<Key, Data>* x, Key&& k, Key* temp)
			{
				COMP comp;
				COMP3 comp3;

				while (!IsNULL(*x)) { // != nil
					long long temp = comp3(k, x->first);
					if (temp < 0) { // k < x.key
						x = &arr[x->left];
					}
					else if (temp > 0) {
						x = &arr[x->right];
					}
					else {
						break;
					}
				}
				if (temp) {
					*temp = std::move(k);
				}
				return x;
			}
			void INSERT_FIXUP(RB_Tree<Key, Data, COMP>* tree, RB_Node<Key, Data>* z) /// google ..
			{
				RB_Node<Key, Data>* y;
				while (tree->arr[z->p].color == RED) {
					if (z->p == tree->arr[tree->arr[z->p].p].left) {
						y = &(tree->arr[tree->arr[tree->arr[z->p].p].right]);
						if (y->color == RED) {
							tree->arr[z->p].color = BLACK;
							y->color = BLACK;
							tree->arr[tree->arr[z->p].p].color = RED;
							z = &(tree->arr[tree->arr[z->p].p]);
						}
						else
						{
							if (z == &(tree->arr[tree->arr[z->p].right])) {
								z = &(tree->arr[z->p]);
								LEFT_ROTATE(tree, z);
							}
							tree->arr[z->p].color = BLACK;
							tree->arr[tree->arr[z->p].p].color = RED;
							RIGHT_ROTATE(tree, &(tree->arr[tree->arr[z->p].p]));
						}
					}
					else {
						y = &(tree->arr[arr[tree->arr[z->p].p].left]);
						if (y->color == RED) {
							tree->arr[z->p].color = BLACK;
							y->color = BLACK;
							tree->arr[tree->arr[z->p].p].color = RED;
							z = &(tree->arr[tree->arr[z->p].p]);
						}
						else {
							if (z == &(tree->arr[tree->arr[z->p].left])) {
								z = &(tree->arr[z->p]);
								RIGHT_ROTATE(tree, z);
							}
							tree->arr[z->p].color = BLACK;
							tree->arr[tree->arr[z->p].p].color = RED;
							LEFT_ROTATE(tree, &(tree->arr[tree->arr[z->p].p]));
						}
					}
				}
				tree->arr[tree->root].color = BLACK;
			}
			long long INSERT(RB_Tree<Key, Data, COMP>* tree, const std::pair<Key, Data>& key, long long hint = 0)
			{
				COMP comp;
				COMP2 eq;
				COMP3 comp3;

				long long y_idx = 0;
				long long x_idx = tree->root;
				auto& chk = tree->arr;

				long long now = tree->arr.size(); //


				if (!IsNULL(tree->root)) {
					while (//!IsNULL(tree->arr[x_idx]) &&
						!IsNULL(x_idx) //&& !hint
						)
					{
						y_idx = x_idx;
						long long test = comp3(key.first, tree->arr[x_idx].first);

						if (test < 0)
						{
							x_idx = tree->arr[x_idx].left;
						}
						else if (test > 0) {
							x_idx = tree->arr[x_idx].right;
						}
						else {
							break;
						}
					}

					if (!IsNULL(x_idx) && eq(key.first, tree->arr[x_idx].first)) {
						tree->arr[x_idx].first = std::move(key.first);
						tree->arr[x_idx].second = std::move(key.second);
						return x_idx;
					}
				}

				RB_Node<Key, Data>* z = nullptr;

				if (0 == tree->dead_list) {
					tree->arr.push_back(RB_Node<Key, Data>());
					tree->arr.back().id = now;
					tree->arr.back().first = std::move(key.first);
					tree->arr.back().second = std::move(key.second);

					z = &(tree->arr.back());
				}
				else {
					now = tree->dead_list;
					tree->dead_list = tree->arr[now].next;
					long long id = tree->arr[now].id;
					long long next = tree->arr[now].next;

					tree->arr[now].Clear();
					tree->arr[now].id = id;
					tree->arr[now].first = std::move(key.first);
					tree->arr[now].second = std::move(key.second);
					tree->arr[now].next = next;
					tree->arr[now].dead = false;

					z = &(tree->arr[now]);
				}

				z->p = tree->arr[y_idx].id;

				if (IsNULL(tree->arr[y_idx])) {
					tree->root = z->id;
				}
				else if (comp(z->first, tree->arr[y_idx].first)) { // comp3? 
					tree->arr[y_idx].left = z->id;//
				}
				else {

					tree->arr[y_idx].right = z->id;//
				}

				z->left = 0; // = nil
				z->right = 0;
				z->color = RED; // = RED

				// insert fixup
				INSERT_FIXUP(tree, z);

				count++;

				return z->id;
			}
			long long INSERT(RB_Tree<Key, Data, COMP>* tree, std::pair<Key, Data>&& key, long long hint = 0)
			{

				//std::cout << "key " << key.first << " hint " << hint << " ";

				COMP comp;
				COMP2 eq;
				COMP3 comp3;

				long long y_idx = 0;
				long long x_idx = tree->root;
				auto& chk = tree->arr;

				long long now = tree->arr.size(); //

				if (!IsNULL(tree->root)) {
					if (hint) {
						x_idx = hint;
					}
					while (//!IsNULL(tree->arr[x_idx]) &&
						!IsNULL(x_idx) //&& !hint
						)
					{
						y_idx = x_idx;
						long long test = comp3(key.first, tree->arr[x_idx].first);

						if (test < 0)
						{
							x_idx = tree->arr[x_idx].left;
						}
						else if (test > 0) {
							x_idx = tree->arr[x_idx].right;
						}
						else {
							break;
						}
					}


					if (!IsNULL(x_idx) && eq(key.first, tree->arr[x_idx].first)) {
						tree->arr[x_idx].first = std::move(key.first);
						tree->arr[x_idx].second = std::move(key.second);
						return x_idx;
					}
				}

				RB_Node<Key, Data>* z = nullptr;

				if (0 == tree->dead_list) {
					tree->arr.push_back(RB_Node<Key, Data>());
					tree->arr.back().id = now;
					tree->arr.back().first = std::move(key.first);
					tree->arr.back().second = std::move(key.second);

					z = &(tree->arr.back());
				}
				else {
					now = tree->dead_list;
					tree->dead_list = tree->arr[now].next;
					long long id = tree->arr[now].id;
					long long next = tree->arr[now].next;

					tree->arr[now].Clear();
					tree->arr[now].id = id;
					tree->arr[now].first = std::move(key.first);
					tree->arr[now].second = std::move(key.second);
					tree->arr[now].next = next;
					tree->arr[now].dead = false;

					z = &(tree->arr[now]);
				}

				z->p = tree->arr[y_idx].id;

				if (IsNULL(tree->arr[y_idx])) {
					tree->root = z->id;
				}
				else if (comp(z->first, tree->arr[y_idx].first)) {
					tree->arr[y_idx].left = z->id;//
				}
				else {
					tree->arr[y_idx].right = z->id;//
				}

				z->left = 0; // = nil
				z->right = 0;
				z->color = RED; // = RED

				// insert fixup
				INSERT_FIXUP(tree, z);

				count++;

				//std::cout << "id " << z->id << "\n";
				return z->id;
			}
			void LAZYINSERT(RB_Tree<Key, Data, COMP>* tree, const std::pair<Key, Data>& key) {
				tree->remain_list.push_back(key);
			}
			void LAZYINSERT(RB_Tree<Key, Data, COMP>* tree, std::pair<Key, Data>&& key) {
				tree->remain_list.push_back(std::move(key));
			}
			void PrintTree(RB_Tree<Key, Data, COMP>* tree) {
				if (tree->empty()) { return; }
				std::queue<int> que;

				que.push(tree->root);
				std::cout << "chk " << "\n";

				while (!que.empty()) {
					int id = que.front(); que.pop();

					std::cout << tree->arr[id].p << " " << id << "\n";

					if (!IsNULL(tree->arr[id].left)) {
						que.push(tree->arr[id].left);
					}
					else {
						std::cout << " X ";
					}
					if (!IsNULL(tree->arr[id].right)) {
						que.push(tree->arr[id].right);
					}
					else {
						std::cout << "X ";
					}
				}
			}
		private:

			void _test(long long left, long long right, long long hint, RB_Tree<Key, Data, COMP>* tree, std::vector<std::pair<Key, Data>>* vec, bool use_hint = false) {
				if (left > right) {
					return;
				}

				long long middle = (left + right) / 2;

				hint = INSERT(tree, std::move((*vec)[middle]), use_hint? hint : 0);

				_test(left, middle - 1, hint, tree, vec, use_hint);
				_test(middle + 1, right, hint, tree, vec, use_hint);
			}

			void REALINSERT(RB_Tree<Key, Data, COMP>* tree) {

				if (tree->remain_list.empty()) {
					return;
				}

				std::stable_sort(std::execution::par, tree->remain_list.begin(), tree->remain_list.end());

				// remove dupplication? but no remove last dup?

				std::vector<std::pair<Key, Data>> vec;
				vec.reserve(tree->remain_list.size());

				bool first = true;
				std::pair<Key, Data> before;
				for (auto&& x : tree->remain_list) {
					if (!first && before.first == x.first) {
						continue;
					}
					before = x;
					first = false;
					vec.push_back(std::move(x));

					//INSERT(tree, std::move(x));
				}
				tree->remain_list.clear();

				// remove dupplication? but no remove last dup?
				if (first_time) {
					_test(0, vec.size() - 1, 0, tree, &vec, true);
				}
				else {
					_test(0, vec.size() - 1, 0, tree, &vec, false);
				}

				first_time = false;
				return;
			}

			RB_Node<Key, Data>* MAXIMUM(RB_Node<Key, Data>* x) {
				while (!IsNULL(x->right)) {
					x = &arr[x->right];
				}
				return x;
			}
			RB_Node<Key, Data>* MINIMUM(RB_Node<Key, Data>* x)
			{
				while (!IsNULL(x->left)) { // != nil
					x = &arr[x->left];
				}
				return x;
			}
			RB_Node<Key, Data>* SUCCESSOR(RB_Node<Key, Data>* x)
			{
				if (!IsNULL(x->right)) { // nil
					return MINIMUM(&arr[x->right]);
				}

				RB_Node<Key, Data>* y = &arr[x->p];
				while (!IsNULL(y->id) && x == &arr[y->right])
				{
					x = y; y = &arr[y->p];
				}
				return y;
			}

			void REMOVE_FIXUP(RB_Tree<Key, Data, COMP>* tree, RB_Node<Key, Data>* x)
			{
				RB_Node<Key, Data>* w;

				while (x->id != root && x->color == BLACK)
				{
					if (x == &(arr[arr[x->p].left])) {
						w = &(arr[arr[x->p].right]);
						if (w->color == RED) {
							w->color = BLACK;
							arr[x->p].color = RED;
							LEFT_ROTATE(tree, &arr[x->p]);
							w = &arr[arr[x->p].right];
						}
						if (arr[w->left].color == BLACK && arr[w->right].color == BLACK) {
							w->color = RED;
							x = &arr[x->p];
						}
						else {
							if (arr[w->right].color == BLACK) {
								arr[w->left].color = BLACK;
								w->color = RED;
								RIGHT_ROTATE(tree, w);
								w = &arr[arr[x->p].right];
							}
							w->color = arr[x->p].color;
							arr[x->p].color = BLACK;
							arr[w->right].color = BLACK;
							LEFT_ROTATE(tree, &arr[x->p]);
							x = &arr[root];
						}
					}
					else {
						w = &arr[arr[x->p].left];
						if (w->color == RED) {
							w->color = BLACK;
							arr[x->p].color = RED;
							RIGHT_ROTATE(tree, &arr[x->p]);
							w = &(arr[arr[x->p].left]);
						}
						if (arr[w->left].color == BLACK && arr[w->right].color == BLACK) {
							w->color = RED;
							x = &arr[x->p];
						}
						else {
							if (arr[w->left].color == BLACK) {
								arr[w->right].color = BLACK;
								w->color = RED;
								LEFT_ROTATE(tree, w);
								w = &(arr[arr[x->p].left]);
							}
							w->color = arr[x->p].color;
							arr[x->p].color = BLACK;
							arr[w->left].color = BLACK;
							RIGHT_ROTATE(tree, &arr[x->p]);
							x = &arr[root];
						}
					}
				}
				x->color = BLACK;
			}


			RB_Node<Key, Data>* REMOVE(RB_Tree<Key, Data, COMP>* tree, RB_Node<Key, Data>* z)
			{
				RB_Node<Key, Data>* x;
				RB_Node<Key, Data>* y;

				if (IsNULL(z->left)
					|| IsNULL(z->right)) {
					y = z;
				}
				else { y = SUCCESSOR(z); }

				if (!IsNULL(y->left)) {
					x = &(tree->arr[y->left]);
				}
				else { x = &(tree->arr[y->right]); }

				x->p = y->p;

				if (IsNULL(y->p)) {
					tree->root = x->id;
				}
				else if (y == &(arr[arr[y->p].left])) {
					arr[y->p].left = x->id;
				}
				else { arr[y->p].right = x->id; }

				if (y != z) { //important part!
					z->first = std::move(y->first); // chk??
					z->second = std::move(y->second);
					std::swap(z->dead, y->dead);
					std::swap(z->next, y->next);
				}
				if (y->color == BLACK) {
					REMOVE_FIXUP(tree, x);
				}
				return y;
			}
		public:
			void PrintTree() {
				PrintTree(this);
			}
			void LazyInsert(RB_Tree<Key, Data, COMP>* tree, const std::pair<Key, Data>& key) {
				LAZYINSERT(this, key);
			}
			void LazyInsert(RB_Tree<Key, Data, COMP>* tree, std::pair<Key, Data>&& key) {
				LAZYINSERT(this, std::move(key));
			}
			// insert, search, remove.
			long long Insert(const std::pair<Key, Data>& key)
			{
				return INSERT(this, key);
			}
			long long Insert(std::pair<Key, Data>&& key)
			{
				return INSERT(this, key);
			}

			bool IsExist(const Key& key) const
			{
				// NULL != ~
				return !IsNULL(*SEARCH(&arr[root], key));
			}
			bool IsExist(Key&& key) const
			{
				// NULL != ~
				return !IsNULL(*SEARCH(&arr[root], key));
			}
			template <typename T> // T must be signed type!


			RB_Node<Key, Data>* Search(const Key& key, long long* id = nullptr) {
				RB_Node<Key, Data>* x = &arr[root];


				x = SEARCH(x, key);

				if (nullptr != id) {
					*id = x->id;
				}

				return x;
			}

			const RB_Node<Key, Data>* Search(const Key& key, long long* id = nullptr) const {
				const RB_Node<Key, Data>* x = &arr[root];

				x = SEARCH(x, key);

				if (nullptr != id) {
					*id = x->id;
				}

				return x;
			}

			RB_Node<Key, Data>* Search(Key&& key, long long* id = nullptr, Key* temp = nullptr) {
				RB_Node<Key, Data>* x = SEARCH(&arr[root], std::move(key), temp);

				if (nullptr != id) {
					*id = x->id;
				}

				return x;
			}

			const RB_Node<Key, Data>* Search(Key&& key, long long* id = nullptr, Key* temp = nullptr) const {
				RB_Node<Key, Data>* x = &arr[root];


				x = SEARCH(temp, key);

				if (nullptr != id) {
					*id = x->id;
				}

				return x;
			}

			void RealInsert()
			{
				REALINSERT(this);
			}

			void Remove(const Key& key)
			{
				RB_Node<Key, Data>* node = SEARCH(&arr[root], key);

				if (!IsNULL(*node))
				{

					RB_Node<Key, Data>* temp = REMOVE(this, node);

					temp->dead = true;
					temp->next = this->dead_list;
					this->dead_list = temp->id;
					count--;
				}
			}

			void Remove(Key&& key)
			{
				RB_Node<Key, Data>* node = SEARCH(&arr[root], std::move(key));

				if (!IsNULL(*node))
				{
					RB_Node<Key, Data>* temp = REMOVE(this, node);

					temp->dead = true;
					temp->next = this->dead_list;
					this->dead_list = temp->id;
					count--;
				}
			}

			Data& DataVal(long long idx) {
				return arr[idx].second;
			}
			const Data& DataVal(long long idx) const {
				return arr[idx].second;
			}

			bool IsEmpty() const
			{
				return 0 == count;
			}
			bool empty() const { return IsEmpty(); }
			long long GetCount() const
			{
				return count;
			}
			long long size() const { return count; }
			void clear() {
				Clear();
			}
		};

	public:
		using iterator = typename std::vector<RB_Node<Key, Data>>::iterator;
		using const_iterator = typename std::vector<RB_Node<Key, Data>>::const_iterator;
	private:
		mutable RB_Tree<Key, Data> arr;
	public:
		explicit array_map(size_t reserve_num = 0) {
			if (reserve_num > 0) {
				arr.reserve(reserve_num);
			}
		}

	public:
		void PrintTree() {
			arr.PrintTree();
		}
		bool empty() const {
			arr.RealInsert();

			return arr.empty();
		}
		auto size() const {
			arr.RealInsert();

			return arr.size();
		}
		void clear() {
			arr.clear();
		}
		void reserve(long long x) {
			arr.reserve(x);
		}
	public:
		iterator begin() {
			arr.RealInsert();

			return arr.begin();
		}
		iterator end() {
			arr.RealInsert();

			return arr.end();
		}
		const_iterator begin() const {
			arr.RealInsert();

			return arr.begin();
		}
		const_iterator end() const {
			arr.RealInsert();

			return arr.end();
		}
		iterator find(const Key& key) {
			arr.RealInsert();

			long long id;
			const RB_Node<Key, Data>* x = arr.Search(key, &id);
			if (0 == x->id) {
				return arr.end();
			}

			auto result = begin() + (x->id - 1);

			return result;
		}
		const_iterator find(const Key& key) const {
			arr.RealInsert();

			long long id;
			RB_Node<Key, Data>* x = arr.Search(key, &id);
			if (0 == x->id) {
				return arr.end();
			}
			return arr.begin() + (x->id - 1);
		}
		iterator find(Key&& key) {
			arr.RealInsert();

			long long id;
			RB_Node<Key, Data>* x = arr.Search(std::move(key), &id);
			if (0 == x->id) {
				return arr.end();
			}
			return begin() + (x->id - 1);
		}
		const_iterator find(Key&& key) const {
			arr.RealInsert();

			long long id;
			RB_Node<Key, Data>* x = arr.Search(std::move(key), &id);
			if (0 == x->id) {
				return arr.end();
			}
			return arr.begin() + (x->id - 1);
		}

	public:
		// different polong long compared by std::map?
		void insert(const std::pair<Key, Data>& value) {
			lazy_insert(value);

			//	arr.RealInsert();

			//	arr.Insert(wiz::Pair<Key, Data>(value.first, value.second));
		}
		void insert(std::pair<Key, Data>&& value) {
			lazy_insert(value);

			//arr.RealInsert();

		//	arr.Insert(wiz::Pair<Key, Data>(value.first, value.second));
		}
		void update()
		{
			arr.RealInsert();
		}
		void lazy_insert(const std::pair<Key, Data>& value) {
			arr.LazyInsert(&arr, value);
		}
		void lazy_insert(std::pair<Key, Data>&& value) {
			arr.LazyInsert(&arr, std::move(value));
		}

		void remove(const std::pair<Key, Data>& value)
		{
			arr.RealInsert();

			arr.Remove(value.first);
		}
		void remove(std::pair<Key, Data>&& value)
		{
			arr.RealInsert();

			arr.Remove(std::move(value.first));
		}
		Data& at(const Key& key) {
			return (*this)[key];
		}
		const Data& at(const Key& key) const {
			return find(key)->second;
		}
		Data& at(Key&& key) {
			return (*this)[std::move(key)];
		}
		const Data& at(Key&& key) const {
			return find(std::move(key))->second;
		}

		Data& operator[](const Key& key) {
			arr.RealInsert();

			RB_Node<Key, Data>* idx = arr.Search(key);

			if (0 == idx->id) {
				long long _idx = arr.Insert(std::pair<Key, Data>(key, Data())); //// return positon? - to do
				return arr.DataVal(_idx);
			}
			else {
				return idx->second;
			}
		}

		const Data& operator[](const Key& key) const {
			arr.RealInsert();

			RB_Node<Key, Data>* idx = arr.Search(key);
			if (0 == idx->id) {
				//long long _idx = arr.Insert(wiz::Pair<Key, Data>(key, Data())); //// return positon? - to do
				//return arr.DataVal(_idx);
				throw "do not exist in array_map";
			}
			else {
				return idx->second;
			}
		}

		Data& operator[](Key&& key) {
			arr.RealInsert();
			Key temp;

			RB_Node<Key, Data>* idx = arr.Search(std::move(key), nullptr, &temp);
			if (0 == idx->id) {
				long long _idx = arr.Insert(std::pair<Key, Data>(std::move(temp), Data())); //// return positon? - to do
				return arr.DataVal(_idx);
			}
			else {
				return idx->second;
			}
		}

		const Data& operator[](Key&& key) const {
			arr.RealInsert();

			RB_Node<Key, Data>* idx = arr.Search(std::move(key));
			if (0 == idx->id) {
				//long long _idx = arr.Insert(wiz::Pair<Key, Data>(std::move(key), Data())); //// return positon? - to do
				//return arr.Idx(_idx).second;
				throw "do not exist in array_map";
			}
			else {
				return idx->second;
			}
		}

	};
}

#endif
