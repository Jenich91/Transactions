namespace sfleta {

template <typename K, typename T>
TreeNode<K, T>* TreeNode<K, T>::NextNode() {
    if (p_right_) {
        return p_right_->MinimalNode();
    } else if (this == p_parent_->p_left_) {
        return p_parent_;
    } else {
        TreeNode<K, T> *x = p_parent_;
        if (x->p_parent_ == nullptr) {
            return nullptr;
        }
        while (x == x->p_parent_->p_right_) {
            x = x->p_parent_;
            if (x->p_parent_ == nullptr) {
                return nullptr;
            }
        }
        return x->p_parent_;
    }
}

template <typename K, typename T>
TreeNode<K, T>* TreeNode<K, T>::PrevNode() {
    if (p_left_) {
        return p_left_->MaximalNode();
    } else if (this == p_parent_->p_right_) {
        return p_parent_;
    } else {
        TreeNode<K, T> *x = p_parent_;
        if (x->p_parent_ == nullptr) {
            return nullptr;
        }
        while (x == x->p_parent_->p_left_) {
            x = x->p_parent_;
            if (x->p_parent_ == nullptr) {
                return nullptr;
            }
        }
        return x->p_parent_;
    }
}

template <typename K, typename T>
TreeNode<K, T>* TreeNode<K, T>::MinimalNode() {
    TreeNode<K, T> *min = this;
    while (min->p_left_) {
        min = min->p_left_;
    }
    return min;
}

template <typename K, typename T>
TreeNode<K, T>* TreeNode<K, T>::MaximalNode() {
    TreeNode<K, T> *max = this;
    while (max->p_right_) {
        max = max->p_right_;
    }
    return max;
}

template <typename K, typename T>
TreeNode<K, T>* TreeNode<K, T>::FindNillFromMinimal() {
    TreeNode<K, T> *nil = this;
    while (nil->p_parent_) {
        nil = nil->p_parent_;
    }
    while (nil->p_right_) {
        nil = nil->p_right_;
    }
    return nil;
}

template <typename K, typename T>
TreeNode<K, T>* TreeNode<K, T>::LeftOrRigth() const {
  return p_left_ ? p_left_ : p_right_;
}

}  // namespace sfleta
