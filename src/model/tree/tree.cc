#include <fstream>
#include <string>
#include <iostream>
namespace sfleta {

template <typename K, typename T>
Tree<K, T>::Tree(const std::initializer_list<K> &items) : Tree() {
    for (auto &value : items) Insert(value);
}

template <typename K, typename T>
Tree<K, T>::Tree(const Tree<K, T> &t) : Tree<K, T>() {
    for (auto &&value : t) Insert(value);
}

template <typename K, typename T>
Tree<K, T>::~Tree() {
    Clear();
}

template <typename K, typename T>
typename Tree<K, T>::Iterator Tree<K, T>::Insert(const K& value) {
    Tree<K, T>::Iterator it;
    if (root_ == nullptr) {
        root_ = new TreeNode<K, T>();
        root_->data_->first = value;
        root_->color_ = kBlack;
        nil_ = new TreeNode<K, T>();
        nil_->p_parent_ = root_;
        root_->p_right_ = nil_;
        it.node_ = root_;
    } else {
        if (Size() == MaxSize()) {
        throw std::overflow_error("ERROR: Container is overflow!");
        }
        it = FindPlace(value);
    }
    size_++;
    return it;
}

template <typename K, typename T>
typename Tree<K, T>::Iterator Tree<K, T>::FindPlace(const K& value) {
    Tree<K, T>::Iterator it;
    TreeNode<K, T>* tmp = root_;
    while (tmp != nullptr) {
        if (value >= tmp->data_->first) {
            if (tmp->p_right_ == nullptr || tmp->p_right_ == nil_) {
                TreeNode<K, T>* new_node = new TreeNode<K, T>();
                if (tmp->p_right_ == nil_) {
                    new_node->p_right_ = nil_;
                    nil_->p_parent_ = new_node;
                }
                tmp->p_right_ = new_node;
                new_node->p_parent_ = tmp;
                new_node->data_->first = value;
                new_node->color_ = kRed;
                InsertCase2(new_node);
                it.node_ = new_node;
                return it;
            } else {
                tmp = tmp->p_right_;
            }
        } else {
            if (tmp->p_left_ == nullptr) {
                TreeNode<K, T>* new_node = new TreeNode<K, T>();
                tmp->p_left_ = new_node;
                new_node->p_parent_ = tmp;
                new_node->data_->first = value;
                new_node->color_ = kRed;
                InsertCase2(new_node);
                it.node_ = new_node;
                return it;
            } else {
                tmp = tmp->p_left_;
            }
        }
    }
    return it;
}

template <typename K, typename T>
TreeNode<K, T>* Tree<K, T>::Grandpa(TreeNode<K, T>* node) const {
    if (node && node->p_parent_) {
        return node->p_parent_->p_parent_;
    } else {
        return nullptr;
    }
}

template <typename K, typename T>
TreeNode<K, T>* Tree<K, T>::Uncle(TreeNode<K, T>* node) const {
    TreeNode<K, T>* grandpa = Grandpa(node);
    if (grandpa == nullptr) {
        return nullptr;
    }
    if (node->p_parent_ == grandpa->p_right_) {
        return grandpa->p_left_;
    } else {
        return grandpa->p_right_;
    }
}

template <typename K, typename T>
TreeNode<K, T>* Tree<K, T>::Brother(TreeNode<K, T>* node) const {
    if (node == node->p_parent_->p_left_ && node->p_parent_->p_right_)
        return node->p_parent_->p_right_;
    else if (node->p_parent_->p_left_)
        return node->p_parent_->p_left_;
    return node;
}

template <typename K, typename T>
void Tree<K, T>::RotateLeft(TreeNode<K, T>* node) {
    TreeNode<K, T>* pivot = node->p_right_;
    pivot->p_parent_ = node->p_parent_;
    if (node->p_parent_) {
        if (node->p_parent_->p_left_ == node) {
            node->p_parent_->p_left_ = pivot;
        } else {
            node->p_parent_->p_right_ = pivot;
        }
    }
    node->p_right_ = pivot->p_left_;
    if (pivot->p_left_) {
        pivot->p_left_->p_parent_ = node;
    }
    node->p_parent_ = pivot;
    pivot->p_left_ = node;
    if (pivot->p_parent_ == nullptr) {
        root_ = pivot;
        InsertCase1(root_);
    }
}

template <typename K, typename T>
void Tree<K, T>::RotateRight(TreeNode<K, T>* node) {
    TreeNode<K, T>* pivot = node->p_left_;
    pivot->p_parent_ = node->p_parent_;
    if (node->p_parent_) {
        if (node->p_parent_->p_left_ == node) {
            node->p_parent_->p_left_ = pivot;
        } else {
            node->p_parent_->p_right_ = pivot;
        }
    }
    node->p_left_ = pivot->p_right_;
    if (pivot->p_right_) {
        pivot->p_right_->p_parent_ = node;
    }
    node->p_parent_ = pivot;
    pivot->p_right_ = node;
    if (pivot->p_parent_ == nullptr) {
        root_ = pivot;
        InsertCase1(root_);
    }
}

template <typename K, typename T>
void Tree<K, T>::InsertCase1(TreeNode<K, T>* node) {
    if (node->p_parent_ == nullptr) {
        node->color_ = kBlack;
    } else {
        InsertCase2(node);
    }
}

template <typename K, typename T>
void Tree<K, T>::InsertCase2(TreeNode<K, T>* node) {
    if (node->p_parent_->color_ == kBlack) {
        return;
    } else {
        InsertCase3(node);
    }
}

template <typename K, typename T>
void Tree<K, T>::InsertCase3(TreeNode<K, T> *node) {
    TreeNode<K, T> *uncle = Uncle(node);
    if (uncle && uncle->color_ == kRed) {
        node->p_parent_->color_ = kBlack;
        uncle->color_ = kBlack;
        TreeNode<K, T> *grandpa = Grandpa(node);
        grandpa->color_ = kRed;
        InsertCase1(grandpa);
    } else {
        InsertCase4(node);
    }
}

template <typename K, typename T>
void Tree<K, T>::InsertCase4(TreeNode<K, T>* node) {
    TreeNode<K, T>* grandpa = Grandpa(node);
    if (node == node->p_parent_->p_right_ && node->p_parent_ == grandpa->p_left_) {
        RotateLeft(node->p_parent_);
        node = node->p_left_;
    } else if (node == node->p_parent_->p_left_ && node->p_parent_ == grandpa->p_right_) {
        RotateRight(node->p_parent_);
        node = node->p_right_;
    }
    InsertCase5(node);
}

template <typename K, typename T>
void Tree<K, T>::InsertCase5(TreeNode<K, T>* node) {
    TreeNode<K, T>* grandpa = Grandpa(node);
    node->p_parent_->color_ = kBlack;
    grandpa->color_ = kRed;
    if (node == node->p_parent_->p_left_ && node->p_parent_ == grandpa->p_left_) {
        RotateRight(grandpa);
    } else {
        RotateLeft(grandpa);
    }
}

template <typename K, typename T>
TreeNode<K, T>* Tree<K, T>::NextElem(TreeNode<K, T>* root) const {
    if (root->p_right_) return NextElem(root->p_right_);
    return root == nil_ ? root->p_parent_ : root;
}

template <typename K, typename T>
int Tree<K, T>::NumberOfChild(TreeNode<K, T> *root) const {
  if ((!root->p_left_) && (!root->p_right_ || root->p_right_ == nil_))
    return 0;
  else if ((!root->p_left_) ^ (!root->p_right_ || root->p_right_ == nil_))
    return 1;
  return 2;
}

template <typename K, typename T>
void Tree<K, T>::Erase(typename Tree<K, T>::Iterator pos) {
    TreeNode<K, T>* del = pos.node_;
    if (!(--size_)) {
        Clear();
        return;
    }
    if (NumberOfChild(del) < 2) {
        if (del->p_parent_) {
            DeleteOneChild(del);
            delete del;
        } else {
            SwapNode(del, NextElem(del->LeftOrRigth()));
        }
    } else {
        SwapNode(del, NextElem(del->p_left_));
    }
}


template <typename K, typename T>
void Tree<K, T>::SwapNode(TreeNode<K, T>* del, TreeNode<K, T>* next) {
    DeleteOneChild(next);
    del->data_->first = next->data_->first;
    del->data_->second = next->data_->second;
    delete next;
}

template <typename K, typename T>
void Tree<K, T>::ReplaceNode(TreeNode<K, T>* node, TreeNode<K, T>* child) {
    if (node->p_parent_->p_left_ && node == node->p_parent_->p_left_) {
        if (child) child->p_parent_ = node->p_parent_;
        node->p_parent_->p_left_ = child;
    } else if (node->p_parent_->p_right_ && node == node->p_parent_->p_right_) {
        if (child) child->p_parent_ = node->p_parent_;
        node->p_parent_->p_right_ = child;
    }
    if (node->p_left_ && node->p_right_ == nil_) child->p_right_ = nil_;
}

template <typename K, typename T>
void Tree<K, T>::DeleteOneChild(TreeNode<K, T>* node) {
    TreeNode<K, T>* child = node->LeftOrRigth();
    if ((!child || child == nil_) && node->color_ == kBlack) DeleteCase1(node);
    ReplaceNode(node, child);
    if (node->color_ == kBlack) {
        if (child && child != nil_ && child->color_ == kRed) {
            child->color_ = kBlack;
        } else if (child && child != nil_) {
            DeleteCase1(child);
        }
    }
}

template <typename K, typename T>
void Tree<K, T>::DeleteCase1(TreeNode<K, T>* node) {
    if (node->p_parent_) DeleteCase2(node);
}

template <typename K, typename T>
void Tree<K, T>::DeleteCase2(TreeNode<K, T>* node) {
    TreeNode<K, T>* bro = Brother(node);
    if (bro->color_ == kRed) {
        node->p_parent_->color_ = kRed;
        bro->color_ = kBlack;
        if (node == node->p_parent_->p_left_)
            RotateLeft(node->p_parent_);
        else
            RotateRight(node->p_parent_);
    }
    DeleteCase3(node);
}

template <typename K, typename T>
void Tree<K, T>::DeleteCase3(TreeNode<K, T> *node) {
  TreeNode<K, T> *bro = Brother(node);
  if ((node->p_parent_->color_ == kBlack) && (bro->color_ == kBlack) &&
      (!bro->p_left_ || bro->p_left_->color_ == kBlack) &&
      (!bro->p_right_ || bro->p_right_->color_ == kBlack)) {
    bro->color_ = kRed;
    DeleteCase1(node->p_parent_);
  } else {
    DeleteCase4(node);
  }
}

template <typename K, typename T>
void Tree<K, T>::DeleteCase4(TreeNode<K, T> *node) {
  TreeNode<K, T> *bro = Brother(node);
  if ((node->p_parent_->color_ == kRed) && (bro->color_ == kBlack) &&
      (!bro->p_left_ || bro->p_left_->color_ == kBlack) &&
      (!bro->p_right_ || bro->p_right_->color_ == kBlack)) {
    bro->color_ = kRed;
    node->p_parent_->color_ = kBlack;
  } else {
    DeleteCase5(node);
  }
}

template <typename K, typename T>
void Tree<K, T>::DeleteCase5(TreeNode<K, T>* node) {
    TreeNode<K, T>* bro = Brother(node);
    if (bro->color_ == kBlack) {
        if ((node == node->p_parent_->p_left_) &&
            (!bro->p_right_ || bro->p_right_->color_ == kBlack) && (bro->p_left_->color_ == kRed)) {
            bro->color_ = kRed;
            bro->p_left_->color_ = kBlack;
            RotateRight(bro);
        } else if ((node == node->p_parent_->p_right_) &&
            (!bro->p_left_ || bro->p_left_->color_ == kBlack) &&
            (bro->p_right_->color_ == kRed)) {
            bro->color_ = kRed;
            bro->p_right_->color_ = kBlack;
            RotateLeft(bro);
        }
    }
    DeleteCase6(node);
}

template <typename K, typename T>
void Tree<K, T>::DeleteCase6(TreeNode<K, T>* node) {
    TreeNode<K, T>* bro = Brother(node);
    bro->color_ = node->p_parent_->color_;
    node->p_parent_->color_ = kBlack;
    if (node == node->p_parent_->p_left_) {
        bro->p_right_->color_ = kBlack;
        RotateLeft(node->p_parent_);
    } else {
        bro->p_left_->color_ = kBlack;
        RotateRight(node->p_parent_);
    }
}

template <typename K, typename T>
void Tree<K, T>::PrintN(TreeNode<K, T>* root) {
    std::ofstream fout;
    fout.open("draw.dot", std::ios::app);
    if (root == nullptr) {
        return;
    }
    std::string color;
    std::string color_left;
    std::string color_right;
    if (root->color_ == kRed) {
        color = { "red" };
    } else {
        color = { "black" };
    }
    if (root->p_left_) {
        fout << std::to_string(root->data_->first) << "[label=" "\"" << "key: " <<
        std::to_string(root->data_->first) + "\n" << "data: " << std::to_string(root->data_->second)
        << "\"" ", style = filled, fontcolor = white, color = " << color << ", shape = circle]\n";
        if (root->p_left_->color_ == kRed) {
            color_left = { "red" };
        } else {
            color_left = { "black" };
        }
        fout << std::to_string(root->p_left_->data_->first) << "[label=" "\"" << "key: "
        << std::to_string(root->p_left_->data_->first) + "\n" << "data: " <<
        std::to_string(root->p_left_->data_->second) << "\"" ",style=filled,fontcolor=white,color="
            << color_left << ",shape=circle]\n";
        fout << root->data_->first << "->" << root->p_left_->data_->first << "\n";
    }
    if (root->p_right_) {
        fout << std::to_string(root->data_->first) << "[label=" "\"" << "key: " <<
        std::to_string(root->data_->first)+ "\n" << "data: " << std::to_string(root->data_->second)
        << "\"" ",style=filled,fontcolor=white,color=" << color << ",shape=circle]\n";
        if (root->p_right_->color_ == kRed) {
            color_right = { "red" };
        } else {
            color_right = { "black" };
        }
        fout << std::to_string(root->p_right_->data_->first) << "[label=" "\"" << "key: " <<
        std::to_string(root->p_right_->data_->first) + "\n" << "data: " <<
        std::to_string(root->p_right_->data_->second) << "\"" ",style=filled,fontcolor=white,color="
            << color_right << ",shape=circle]\n";
        fout << root->data_->first << "->" << root->p_right_->data_->first << "\n";
    }
    fout.close();
    PrintN(root->p_left_);
    PrintN(root->p_right_);
}

template <typename K, typename T>
void Tree<K, T>::Print() {
    std::ofstream fout;
    fout.open("draw.dot");
    fout << "digraph G {\n";
    fout.close();
    PrintN(root_);
    fout.open("draw.dot", std::ios::app);
    fout << "\n}\n";
    fout.close();
}

template <typename K, typename T>
void Tree<K, T>::Clean(TreeNode<K, T>* node) {
    if (node) {
    Clean(node->p_left_);
    Clean(node->p_right_);
    delete node;
    }
    root_ = nullptr;
    size_ = 0;
}

template <typename K, typename T>
void Tree<K, T>::Clear() {
    Clean(root_);
}

template <typename K, typename T>
typename Tree<K, T>::Iterator Tree<K, T>::Begin() const {
    Iterator it;
    if (root_) {
        it.node_ = root_->MinimalNode();
    }
    return it;
}

template <typename K, typename T>
typename Tree<K, T>::Iterator Tree<K, T>::End() const {
    Iterator it;
    if (root_) {
        it.node_ = nil_;
    }
    return it;
}

template <typename K, typename T>
void Tree<K, T>::Swap(Tree<K, T>& other) {
    Tree<K, T> tmp(*this);
    *this = std::move(other);
    other = std::move(tmp);
}

template <typename K, typename T>
void Tree<K, T>::Merge(Tree<K, T>* other, bool is_set) {
    Tree<K, T> tmp(*other);
    int pos = 0;
    for (auto &&value : tmp) {
        if (is_set) {
            if (!Contains(value)) {
                Insert(value);
                auto it = other->Begin();
                for (int i = 0; i < pos; i++) {
                    ++it;
                }
                other->Erase(it);
            } else {
                pos++;
            }
        } else {
            Insert(value);
            auto it = other->Begin();
            other->Erase(it);
        }
    }
}

template <typename K, typename T>
std::pair<typename Tree<K, T>::Iterator, bool> Tree<K, T>::FindContains(const K& key) {
    std::pair<typename Tree<K, T>::Iterator, bool> result;
    result.second = false;
    TreeNode<K, T>* tmp = root_;
    while (tmp != nullptr && tmp != nil_) {
        if (key == tmp->data_->first) {
            result.first.node_ = tmp;
            result.second = true;
            return result;
        } else if (key > tmp->data_->first) {
            tmp = tmp->p_right_;
        } else {
            tmp = tmp->p_left_;
        }
    }
    if (tmp == nil_ || tmp == nullptr) {
        result.first.node_ = nil_;
    }
    return result;
}

template <typename K, typename T>
typename Tree<K, T>::Iterator Tree<K, T>::Find(const K& key) {
    return FindContains(key).first;
}

template <typename K, typename T>
bool Tree<K, T>::Contains(const K& key) {
    return FindContains(key).second;
}

template <typename K, typename T>
void Tree<K, T>::Iterator::operator++() {
    if (node_->NextNode()) {
        node_ = node_->NextNode();
    } else {
        node_ = node_->PrevNode();
    }
}

template <typename K, typename T>
void Tree<K, T>::Iterator::operator--() {
    if (node_->PrevNode()) {
        node_ = node_->PrevNode();
    } else {
        node_ = node_->FindNillFromMinimal();
    }
}

template <typename K, typename T>
const K Tree<K, T>::Iterator::operator*() {
    if (!node_) {
        throw std::out_of_range("ERROR: iterator is nullptr");
    }
    return node_->data_->first;
}

template <typename K, typename T>
Tree<K, T>& Tree<K, T>::operator=(Tree<K, T>&& other) {
    if (this == &other) {
        return *this;
    }
    if (root_) {
        Clear();
    }
    size_ = other.size_;
    root_ = other.root_;
    nil_ = other.nil_;
    other.size_ = 0;
    other.root_ = nullptr;
    other.nil_ = nullptr;
    return *this;
}

}  // namespace sfleta
