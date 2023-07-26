#ifndef SRC_TREE_H_
#define SRC_TREE_H_
#include <initializer_list>
#include <limits>
#include <utility>
#include <exception>

#include "treenode.h"
namespace sfleta {
template <typename K, typename T>
class Tree {
 protected:
    TreeNode<K, T>* root_;
    size_t size_;
    TreeNode<K, T>* nil_;

 private:
    void PrintN(TreeNode<K, T>* root);
    TreeNode<K, T>* Grandpa(TreeNode<K, T>* node) const;
    TreeNode<K, T>* Brother(TreeNode<K, T>* node) const;
    TreeNode<K, T>* Uncle(TreeNode<K, T>* node) const;
    void RotateLeft(TreeNode<K, T>* node);
    void RotateRight(TreeNode<K, T>* node);
    void InsertCase1(TreeNode<K, T>* node);
    void InsertCase2(TreeNode<K, T>* node);
    void InsertCase3(TreeNode<K, T>* node);
    void InsertCase4(TreeNode<K, T>* node);
    void InsertCase5(TreeNode<K, T>* node);
    void Clean(TreeNode<K, T>* node);
    void DeleteCase1(TreeNode<K, T>* node);
    void DeleteCase2(TreeNode<K, T>* node);
    void DeleteCase3(TreeNode<K, T>* node);
    void DeleteCase4(TreeNode<K, T>* node);
    void DeleteCase5(TreeNode<K, T>* node);
    void DeleteCase6(TreeNode<K, T>* node);
    void ReplaceNode(TreeNode<K, T>* node, TreeNode<K, T>* child);
    void DeleteOneChild(TreeNode<K, T>* node);
    TreeNode<K, T>* NextElem(TreeNode<K, T>* root) const;
    int NumberOfChild(TreeNode<K, T>* root) const;
    void SwapNode(TreeNode<K, T>* del, TreeNode<K, T>* next);

 public:
    class Iterator {
     public:
        TreeNode<K, T>* node_;
        Iterator() : node_(nullptr) {}
        void operator++();
        void operator--();
        bool operator==(const Iterator& other) { return this->node_ == other.node_; }
        bool operator!=(const Iterator& other) { return this->node_ != other.node_; }
        const K operator*();
    };
    Tree() : root_(nullptr), size_(0), nil_(nullptr) {}
    explicit Tree(const std::initializer_list<K>& items);
    Tree(const Tree<K, T>& t);
    ~Tree();
    Tree<K, T>& operator=(Tree<K, T>&& other);
    Iterator Begin() const;
    Iterator End() const;
    size_t Size() { return size_; }
    size_t MaxSize() { return std::numeric_limits<size_t>::max() / sizeof(TreeNode<K, T>) / 2; }
    void Swap(Tree<K, T>& other);
    void Merge(Tree<K, T>* other, bool is_set);
    Iterator Find(const K& key);
    bool Contains(const K& key);
    Iterator Insert(const K& value);
    void Clear();
    void Print();
    void Erase(Iterator pos);

 protected:
    std::pair<Iterator, bool> FindContains(const K& key);

 private:
    Iterator FindPlace(const K& value);
};

}  // namespace sfleta
#include "tree.cc"
#endif  // SRC_TREE_H_
