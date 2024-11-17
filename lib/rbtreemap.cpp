#include "rbtree.h"

#include <functional>
#include <optional>

template<typename K, typename V>
class RBMap {
public:
    RBMap<K, V>() {}
    
    std::optional<std::reference_wrapper<V>> operator[](K key) {
        NSRBTree::TreeNode<K, V>* found_node = tree.Search(key);
        if (found_node == nullptr) {
            return std::nullopt;
        }
        return found_node->GetValue();
    }
    
    void Add(const K& key, const V& value) {
        tree.Push(key, value);
    }
    
    void IterateKeys(void (*func) (K)) {
        tree.InOrderTraverse(func);
    }
private:
    NSRBTree::RBTree<K, V> tree;
};
