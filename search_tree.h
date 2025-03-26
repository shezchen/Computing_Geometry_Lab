//
// Created by aoyeningluosi on 24-10-29.
//

#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H
#include <algorithm>
#include <vector>
#include <bits/ios_base.h>
#include <bits/ranges_algo.h>

#endif //SEARCH_TREE_H

struct node {
    int val;
    struct node *left;
    struct node *right;
    node() : val(0), left(nullptr), right(nullptr) {}
    explicit node(int x) : val(x), left(nullptr), right(nullptr) {}
};

class search_tree {
public:
    void insert(int val);
    struct node* root;
    search_tree() {
        root = nullptr;
    }
    void search(int min,int max,std::vector<int>& result);
    static int test(int times) {
        search_tree my_tree;
        for(int i=0;i<times;i++) {
            int a;
            std::cin >> a;
            my_tree.insert(a);
        }
        std::vector<int> result;
        my_tree.search(5,15,result);
        for(int num:result) {
            std::cout << num << " ";
        }
        return 0;
    }
private:
    void search_node(int min,int max,std::vector<int>& result, const struct node* cur);
};

inline void search_tree::search(int min, int max, std::vector<int> &result) {
    search_node(min,max,result,root);
    std::sort(result.begin(),result.end());
}

inline void search_tree::search_node(int min, int max, std::vector<int> &result, const struct node* cur) {
    if(cur == nullptr) {return;}
    if(cur->val>=min && cur->val<=max) {
        result.push_back(cur->val);
        search_node(min,max,result,cur->left);
        search_node(min,max,result,cur->right);
    }else if(cur->val<min) {
        search_node(min,max,result,cur->right);
    }
    else {
        search_node(min,max,result,cur->left);
    }
}
inline void search_tree::insert(const int val) {
    //insert a val
    if(root == nullptr) {
        root = new node(val);
    }else {
        node* cur = root;
        node* prev = nullptr;
        enum state{L,R};
        state direct = L;
        while(cur != nullptr) {
            if(val==cur->val) {return;}
            if(val < cur->val) {
                prev = cur;
                cur = cur->left;
                direct = L;
            }else {
                prev = cur;
                cur = cur->right;
                direct = R;
            }
        }
        if(prev == nullptr) {return;}
        if(direct==L) {
            prev->left = new node(val);
        }else {
            prev->right = new node(val);
        }
    }
}

