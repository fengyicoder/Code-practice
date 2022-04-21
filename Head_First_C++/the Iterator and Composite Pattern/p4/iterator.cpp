#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <experimental/coroutine>
#include <experimental/generator>

using namespace std;

template<typename T> struct BinaryTree;

template<typename T>
struct Node
{
    T value;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    BinaryTree<T>* tree;

    explicit Node(const T& value):
        value(value), left(nullptr), right(nullptr),
        parent(nullptr), tree(nullptr) {}
    Node(const T& value, Node<T>* left, Node<T>* right):
        value(value), left(left), right(right),
        parent(nullptr), tree(nullptr)
        {
            this->left->tree = this->right->tree = tree;
            this->left->parent = this->right->parent = tree;
        }
    
    void set_tree(BinaryTree<T>* t)
    {
        tree = t;
        if (left) left->set_tree(t);
        if (right) right->set_tree(t);
    }
};

template<typename T>
struct BinaryTree
{
    Node<T>* root = nullptr;
    explicit BinaryTree(Node<T>* const root): root{root}
    {
        root->set_tree(this);
    }

    template<typename U>
    struct PreOrderIterator
    {
        Node<U>* current;
        explicit PreOrderIterator(Node<U>* current): current(current) {}

        bool opeartor!=(const PreOrderIterator<U>& other)
        {
            return this->current != other.current;
        }

        PreOrderIterator<U>& operator++()
        {
            if (current->right)
            {
                current = current->right;
                while(current->left) current = current->left;
            }
            else
            {
                Node<T>* p = current->parent;
                while(p && current == p->right)
                {
                    current = p;
                    p = p->parent;
                }
                current = p;
            }
            return *this;
        }
    };

    typedef PreOrderIterator<T> iterator;

    Node<U>& operator*()
    {
        return *current;
    }

    iterator begin()
    {
        Node<T>* n = root;
        if (n) 
        {
            while(n->left) n = n->left;
        }
        return iterator{n};
    }

    iterator end()
    {
        return iterator{nullptr};
    }

    class pre_order_traversal
    {
        BinaryTree<T>& tree;
    public:
        pre_order_traversal(BinaryTree<T>& tree): tree{tree} { }
        iterator begin() { return tree.begin();}
        iterator end() { return tree.end();}
    } pre_order;

    experimental::generator<Node<T>*> post_order()
    {
        return post_order_impl(root);
    }
private:
    experimental::generator<Node<T>*> post_order_impl(Node<T>* node)
    {
        if (node)
        {
            for (auto x: post_order_impl(node->left)) co_yield x;
            for (auto y: post_order_impl(node->right)) co_yield y;
            co_yield node;
        }
    }
};

void std_iterators()
{
  vector<string> names{ "john", "jane", "jill", "jack" };

  vector<string>::iterator it = names.begin(); // or begin(names)
  cout << "first name is " << *it << "\n";

  ++it; // advance the iterator
  it->append(string(" goodall"));
  cout << "second name is " << *it << "\n";

  while (++it != names.end())
  {
    cout << "another name: " << *it << "\n";
  }

  // traversing the entire vector backwards
  // note global rbegin/rend, note ++ not --
  // expand auto here
  for (auto ri = rbegin(names); ri != rend(names); ++ri)
  {
    cout << *ri;
    if (ri + 1 != rend(names)) // iterator arithmetic
      cout << ", ";
  }
  cout << endl;

  // constant iterators
  vector<string>::const_reverse_iterator jack = crbegin(names);
  // won't work
  //*jack += "reacher";

  for (auto& name : names)
    cout << "name = " << name << "\n";
}

// in order traversal
void binary_tree_iterator()
{
  //         me
  //        /  \
  //   mother   father
  //      / \
  //   m'm   m'f

  BinaryTree<string> family{
    new Node<string>{"me", 
      new Node<string>{"mother",
        new Node<string>{"mother's mother"},
        new Node<string>{"mother's father"}
      },
      new Node<string>{"father"}
    }
  };

  // pre order traversal
  for (auto it = family.begin(); it != family.end(); ++it)
  {
    cout << (*it).value << "\n";
  }

  cout << "=== and now, through a dedicated object:\n";

  // use iterator name
  for (const auto& it: family.pre_order)
  {
    cout << it.value << "\n";
  }

  cout << "=== postorder travesal with coroutines:\n";

  // use coroutines (yields pointers!)
  // postorder: m'm, m'f m f me
  for (auto it: family.post_order())
  {
    cout << it->value << endl;
  }
}


int main()
{
  //std_iterators();
  binary_tree_iterator();

  getchar();
  return 0;
}



