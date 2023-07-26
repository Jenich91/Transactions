#ifndef SRC_TREENODE_H_
#define SRC_TREENODE_H_
#include <stddef.h>
namespace sfleta {
enum node_colors { kRed, kBlack };
template <typename K, typename T>
class TreeNode {
 public:
    std::pair<K, T>* data_;
    TreeNode* p_right_;
    TreeNode* p_left_;
    TreeNode* p_parent_;
    node_colors color_;
    TreeNode() : p_right_(nullptr), p_left_(nullptr), p_parent_(nullptr), color_(kBlack)
    {data_ = new std::pair<K, T>;}
    TreeNode(const TreeNode<K, T> &other) : TreeNode() {
        *this = other;
    }
    ~TreeNode() {delete data_; data_ = nullptr;}
    TreeNode<K, T>& operator=(const TreeNode<K, T> &other) {
        data_ = other.data_;
        p_right_ = other.p_right_;
        p_left_ = other.p_left_;
        p_parent_ = other.p_parent_;
        color_ = other.color_;
        return *this;
    }
    TreeNode<K, T>* NextNode();
    TreeNode<K, T>* PrevNode();
    TreeNode<K, T>* MinimalNode();
    TreeNode<K, T>* MaximalNode();
    TreeNode<K, T>* FindNillFromMinimal();
    TreeNode<K, T>* LeftOrRigth() const;
};
}  // namespace sfleta
#include "treenode.cc"
#endif  // SRC_TREENODE_H_
