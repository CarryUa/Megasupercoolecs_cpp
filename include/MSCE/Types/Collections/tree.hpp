#ifndef MSCE_TREE_H_
#define MSCE_TREE_H_
#include <vector>
namespace msce
{
    template <typename TD>
    struct NTreeNode
    {
    public:
        std::vector<NTreeNode<TD> *> children;
        NTreeNode<TD> *parent = nullptr;
        TD data;

        void add_child(NTreeNode<TD> *node);
        void add_child(TD data);
        bool is_leaf() const;
        bool is_root() const;
        NTreeNode<TD> *get_root();

        std::vector<TD> traverse_dfs();
        // std::vector<TD> traverse_hfs();

        NTreeNode(TD data, NTreeNode *parent = nullptr);

    private:
        void dfs_traversal_helper(NTreeNode<TD> *cur, std::vector<TD> &result);
        // void hfs_traversal_helper(NTree*Node<TD> *cur, std::vector<TD> &result);
    };
    template <typename TD>
    inline void NTreeNode<TD>::add_child(NTreeNode<TD> *node)
    {
        node->parent = this;
        this->children.push_back(node);
    }
    template <typename TD>
    inline void NTreeNode<TD>::add_child(TD data)
    {
        this->children.push_back(new NTreeNode<TD>(data, this));
    }
    template <typename TD>
    inline bool NTreeNode<TD>::is_leaf() const
    {
        return this->children.size() == 0;
    }
    template <typename TD>
    inline bool NTreeNode<TD>::is_root() const
    {
        return this->parent == nullptr;
    }
    template <typename TD>
    inline NTreeNode<TD> *NTreeNode<TD>::get_root()
    {
        if (this->parent == nullptr)
            return this;

        NTreeNode<TD> *cur = this->parent;
        while (cur->parent != nullptr)
            cur = cur->parent;

        return cur;
    }
    template <typename TD>
    inline std::vector<TD> NTreeNode<TD>::traverse_dfs()
    {
        std::vector<TD> res;
        this->dfs_traversal_helper(this, res);

        return res;
    }
    // template <typename TD>
    // inline std::vector<TD> NTreeNode<TD>::traverse_hfs()
    // {
    //     std::vector<TD> res;
    //     this->hfs_traversal_helper(this, res);

    //     return res;
    // }
    template <typename TD>
    inline NTreeNode<TD>::NTreeNode(TD data, NTreeNode *parent)
    {
        this->data = data;
        this->parent = parent;
    }
    template <typename TD>
    inline void NTreeNode<TD>::dfs_traversal_helper(NTreeNode<TD> *cur, std::vector<TD> &result)
    {
        if (!cur)
            return;

        for (auto *next : cur->children)
        {
            if (!next)
                continue;

            dfs_traversal_helper(next, result);
        }

        result.push_back(cur->data);
        return;
    }
    // template <typename TD>
    // inline void NTreeNode<TD>::hfs_traversal_helper(NTreeNode<TD> *cur, std::vector<TD> &result)
    // {
    //     if (!cur)
    //         return;
    // }
}

#endif // MSCE_TREE_H_