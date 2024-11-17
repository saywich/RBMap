#include <cstdint>
#include <iostream>

namespace NSRBTree {

enum Color: uint8_t {
    kRed, kBlack
};
    
template<typename K, typename V>
class TreeNode {
public:    
    TreeNode<K, V>(
        K key,
        V value,
        TreeNode<K, V>* parent=nullptr,
        TreeNode<K, V>* left=nullptr,
        TreeNode<K, V>* right=nullptr,
        Color color=kRed
    ) : key_(key), value_(value), parent_(parent), left_(left), right_(right), color_(color) {}
    
    TreeNode<K, V>* GetRight() { return this->right_; }
    TreeNode<K, V>* GetLeft() { return this->left_; }
    TreeNode<K, V>* GetParent() { return this->parent_; }
    Color GetColor() { return this->color_; }
    
    const K& GetKey() { return this->key_; }
    V& GetValue() { return this->value_; }
    
    void SetRight(TreeNode<K, V>* right) { this->right_ = right; }
    void SetLeft(TreeNode<K, V>* left) { this->left_ = left; }
    void SetParent(TreeNode<K, V>* parent) { this->parent_ = parent; }
    void SetColor(Color color) { this->color_ = color; }
    
    void SetKey(const K& key) { this->key_ = key; }
    void SetValue(const V& value) { this->value_ = value; }

private:
    K key_;
    V value_;
    TreeNode<K, V>* parent_;
    TreeNode<K, V>* left_;
    TreeNode<K, V>* right_;
    
    Color color_;
};

template<typename K, typename V>
class RBTree {
public:
    RBTree() : root_(nullptr) {}
    
    void RotateLeft(TreeNode<K, V>* root) {
        TreeNode<K, V>* parent = root->GetParent();
        TreeNode<K, V>* right = root->GetRight();
        TreeNode<K, V>* right_left = right->GetLeft();
        
        right->SetLeft(root);
        root->SetParent(right);
        root->SetRight(right_left);
        
        if (right_left != nullptr) right_left->SetParent(root);
        
        if (parent == nullptr) {
            right->SetParent(nullptr);
            this->root_ = right;
        } else {
            if (root->GetKey() < parent->GetKey()) {
                right->SetParent(parent);
                parent->SetLeft(right);
            } else {
                right->SetParent(parent);
                parent->SetRight(right);
            }
        }
    }
    
    void RotateRight(TreeNode<K, V>* root) {
        TreeNode<K, V>* parent = root->GetParent();
        TreeNode<K, V>* left = root->GetLeft();
        TreeNode<K, V>* left_right = left->GetRight();
        
        left->SetRight(root);
        root->SetParent(left);
        root->SetLeft(left_right);
        if (left_right != nullptr) left_right->SetParent(root);
        
        if (parent == nullptr) {
            left->SetParent(nullptr);
            this->root_ = left;
        } else {
            if (root->GetKey() < parent->GetKey()) {
                left->SetParent(parent);
                parent->SetLeft(left);
            } else {
                left->SetParent(parent);
                parent->SetRight(left);
            }
        }
    }
    
    void Push(const K& key, const V& value) {
        TreeNode<K, V>* new_node = new TreeNode<K, V>(key, value);
        
        if (this->root_ == nullptr) {
            this->root_ = new_node;
            this->root_->SetColor(kBlack);
            return;
        }
        
        TreeNode<K, V>* parent_node = nullptr;
        TreeNode<K, V>* current_node = this->root_;
        
        while (current_node != nullptr) {
            if (key > current_node->GetKey()) {
                parent_node = current_node;
                current_node = current_node->GetRight();
            } else if (key < current_node->GetKey()) {
                parent_node = current_node;
                current_node = current_node->GetLeft();
            } else {
                current_node->SetValue(value);
                return;
            }
        }
        
        if (key > parent_node->GetKey()) {
            new_node->SetParent(parent_node);
            parent_node->SetRight(new_node);
        } else {
            new_node->SetParent(parent_node);
            parent_node->SetLeft(new_node);
        }
        
        // std::cout << "123" << std::endl;
        // std::cout.flush();
        this->FixColorsAfterInsert(new_node);
    };
    
    void FixColorsAfterInsert(TreeNode<K, V>* root) {
        TreeNode<K, V>* father = root->GetParent();
        while (father != nullptr && father->GetColor() == kRed) {
            // std::cout << root->GetKey() << std::endl;
            // std::cout.flush();
            TreeNode<K, V>* grandfather = father->GetParent();
            if (father->GetKey() < grandfather->GetKey()) {
                if (grandfather->GetRight() != nullptr && grandfather->GetRight()->GetColor() == kRed) {
                    father->SetColor(kBlack);
                    grandfather->GetRight()->SetColor(kBlack);
                    grandfather->SetColor(kRed);
                    root = grandfather;
                } else {
                    if (root->GetKey() > father->GetKey()) {
                        root = father;
                        RotateLeft(root);
                    }
                    father->SetColor(kBlack);
                    grandfather->SetColor(kRed);
                    this->RotateRight(grandfather);
                }
            } else {
                if (grandfather->GetLeft() != nullptr && grandfather->GetLeft()->GetColor() == kRed) {
                    father->SetColor(kBlack);
                    grandfather->GetLeft()->SetColor(kBlack);
                    grandfather->SetColor(kRed);
                    root = grandfather;
                } else {
                    if (root->GetKey() < father->GetKey()) {
                        root = father;
                        this->RotateRight(root);
                    }
                    father->SetColor(kBlack);
                    grandfather->SetColor(kRed);
                    this->RotateLeft(grandfather);
                }
            }
            father = root->GetParent();
        }
        
        this->root_->SetColor(kBlack);
    }
    
    void DeleteByKey(const K& key) {
        TreeNode<K, V>* current_node = this->root_;
        
        while (current_node != nullptr && current_node->GetKey() != key) {
            if (key > current_node->GetKey()) {
                current_node = current_node->GetRight();
            } else {
                current_node = current_node->GetLeft();
            }
        }
        
        if (current_node == nullptr) return;
        
        if (current_node->GetLeft() == nullptr && current_node->GetRight() == nullptr) {
            if (current_node->GetParent() == nullptr) {
                delete current_node;
                this->root_ = nullptr;
            } else {
                if (current_node->GetKey() > current_node->GetParent()->GetKey()) {
                    current_node->GetParent()->SetRight(nullptr);
                } else {
                    current_node->GetParent()->SetLeft(nullptr);
                }
                delete current_node;
            }
        }
        
        if (current_node->GetLeft() != nullptr && current_node->GetRight() == nullptr) {
            if (current_node->GetKey() > current_node->GetParent()->GetKey()) {
                current_node->GetParent()->SetRight(current_node->GetLeft());
                current_node->GetLeft()->SetParent(current_node->GetParent());
            } else {
                current_node->GetParent()->SetLeft(current_node->GetLeft());
                current_node->GetLeft()->SetParent(current_node->GetParent());
            }
            delete current_node;
        }
        
        if (current_node->GetLeft() == nullptr && current_node->GetRight() != nullptr) {
            if (current_node->GetKey() > current_node->GetParent()->GetKey()) {
                current_node->GetParent()->SetRight(current_node->GetRight());
                current_node->GetRight()->SetParent(current_node->GetParent());
            } else {
                current_node->GetParent()->SetLeft(current_node->GetRight());
                current_node->GetRight()->SetParent(current_node->GetParent());
            }
            delete current_node;
        }
        
        if (current_node->GetLeft() != nullptr && current_node->GetRight() != nullptr) {
            TreeNode<K, V>* successor = current_node->GetRight();
            
            while (successor->GetLeft() != nullptr) {
                successor = current_node->GetLeft();
            }
            
            TreeNode<K, V>* temp = current_node;
            
            current_node->SetKey(successor->GetKey());
            current_node->SetValue(successor->GetValue());
            
            successor->SetKey(temp->GetKey());
            successor->SetValue(temp->GetValue());
            
            if (successor->GetLeft() == nullptr && successor->GetRight() == nullptr) {
                if (successor->GetKey() > successor->GetParent()->GetKey()) {
                    successor->GetParent()->SetRight(nullptr);
                } else {
                    successor->GetParent()->SetLeft(nullptr);
                }
                delete successor;
            }
            
            if (successor->GetLeft() != nullptr && successor->GetRight() == nullptr) {
                if (successor->GetKey() > successor->GetParent()->GetKey()) {
                    successor->GetParent()->SetRight(successor->GetLeft());
                    successor->GetLeft()->SetParent(successor->GetParent());
                } else {
                    successor->GetParent()->SetLeft(successor->GetLeft());
                    successor->GetLeft()->SetParent(successor->GetParent());
                }
                delete successor;
            }
            
            if (successor->GetLeft() == nullptr && successor->GetRight() != nullptr) {
                if (successor->GetKey() > successor->GetParent()->GetKey()) {
                    successor->GetParent()->SetRight(successor->GetRight());
                    successor->GetRight()->SetParent(successor->GetParent());
                } else {
                    successor->GetParent()->SetLeft(successor->GetRight());
                    successor->GetRight()->SetParent(successor->GetParent());
                }
                delete successor;
            }
        }
    };
    
    void FixColorsAfterDelete(TreeNode<K, V>* root) {
        if (root->GetColor() == kRed) return;
        
        while (root->GetColor() == kBlack && root->GetParent() != nullptr) {
            if (root->GetKey() < root->GetParent()->GetKey()) {
                
            }
        }
    }
    
    TreeNode<K, V>* Search(const K& key) {
        TreeNode<K, V>* current_node = this->root_;
        
        while (current_node != nullptr && current_node->GetKey() != key) {
            if (key < current_node->GetKey()) {
                current_node = current_node->GetLeft();
            } else if (key > current_node->GetKey()) {
                current_node = current_node->GetRight();
            }
        }
        
        return current_node;
    };
    
    void InOrderTraverse(void (*func) (K)) {
        this->BSTInOrderTraverse(this->root_, func);
        // TreeNode<K, V>* current_node = this->root_;
        
        // while (current_node->GetLeft() != nullptr) {
        //     current_node = current_node->GetLeft();
        // }
    }
    
    void PreOrderTraverse(void (*func) (K)) {
        this->BSTPreOrderTraverse(this->root_, func);
        // TreeNode<K, V>* current_node = this->root_;
        
        // while (current_node->GetLeft() != nullptr) {
        //     current_node = current_node->GetLeft();
        // }
    }

private:
    TreeNode<K, V>* root_;
    
    void BSTInOrderTraverse(TreeNode<K, V>* root, void (*func) (K)) {
        if (root == nullptr) {
            return;
        }
        
        BSTInOrderTraverse(root->GetLeft(), func);
        
        func(root->GetKey());
        
        BSTInOrderTraverse(root->GetRight(), func);
    }
    
    void BSTPreOrderTraverse(TreeNode<K, V>* root, void (*func) (K)) {
        if (root == nullptr) {
            return;
        }
        
        func(root->GetKey());
        
        BSTPreOrderTraverse(root->GetLeft(), func);
        BSTPreOrderTraverse(root->GetRight(), func);
    }
};

}