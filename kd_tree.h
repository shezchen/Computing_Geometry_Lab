//
// Created by aoyeningluosi on 24-10-29.
//

#ifndef KD_TREE_H
#define KD_TREE_H
#include <bits/stl_pair.h>

#endif //KD_TREE_H


class KD_Tree {
public:
    struct Node ;
    Node *ptr=nullptr;
    struct Node {
        enum spilt_dim{X=0,Y=1};//split by line x=k or t=k
        int dim;
        int split;
        std::pair<int,int> point;// save only one point(x,y) or(-1,-1)
        Node* left;
        Node* right;
        Node(std::vector<std::pair<int,int>> points_vector, const int pre_dim) {
            dim = (pre_dim+1)/2;
            point={-1,-1};
            if(points_vector.empty()) {
                //null situation
                left = right = nullptr;
                split=0;
                point={-1,-1};
                return;
            }
            if(points_vector.size()==1) {
                point = points_vector[0];//save this point
                left = right = nullptr;
                split=0;
                return;
            }
            //have more than 2 points

            if(dim==0) {
                //split by line x=k
                std::ranges::sort(points_vector,[](const std::pair<int,int>& a,const std::pair<int,int>& b){return a.first<b.first;});
                //sort this vector,then get mid
                std::vector<std::pair<int,int>> split_points_l;//new vector for next construction
                std::vector<std::pair<int,int>> split_points_r;
                split=points_vector[points_vector.size()/2].first;
                for(int i=0;i<points_vector.size()/2;i++) {
                    split_points_l.push_back(points_vector[i]);
                }
                for(int i=(int)points_vector.size()/2;i<points_vector.size();i++) {
                    split_points_r.push_back(points_vector[i]);
                }
                point={0,0};
                left=new Node(split_points_l,dim);
                right=new Node(split_points_r,dim);
            }else {
                //split by line x=k
                std::ranges::sort(points_vector,[](const std::pair<int,int>& a,const std::pair<int,int>& b){return a.second<b.second;});
                //sort this vector,then get mid
                std::vector<std::pair<int,int>> split_points_l;//new vector for next construction
                std::vector<std::pair<int,int>> split_points_r;
                split=points_vector[points_vector.size()/2].second;
                for(int i=0;i<points_vector.size()/2;i++) {
                    split_points_l.push_back(points_vector[i]);
                }
                for(int i=static_cast<int>(points_vector.size())/2;i<points_vector.size();i++) {
                    split_points_r.push_back(points_vector[i]);
                }
                point={-1,-1};
                left=new Node(split_points_l,dim);
                right=new Node(split_points_r,dim);
            }
        }

    };
    void search(int,int,int,int,std::vector<std::pair<int,int>>& result,KD_Tree::Node*);
    explicit KD_Tree(const std::vector<std::pair<int,int>>& points_vector);

    void insert_point(std::pair<int,int> point);

    void search_point(int,int,int,int,std::vector<std::pair<int,int>>& result);
    static int test() {
        std::vector<std::pair<int,int>> v={std::make_pair(12,12),std::make_pair(5,6),std::make_pair(7,8)},result;
        KD_Tree mytree(v);
        mytree.search_point(2,6,5,8,result);
        std::cout<<"result: ";
        for(int i=0;i<result.size();i++) {
            std::cout<<result[i].first<<" "<<result[i].second<<std::endl;
        }
        return 0;
    }
};


inline KD_Tree::KD_Tree(const std::vector<std::pair<int, int>> &points_vector) {
    ptr = new Node(points_vector,1);//the first node is split by x=k
}

inline void KD_Tree::insert_point(std::pair<int,int> point) {
    //point.emplace_back(point);

}
inline void KD_Tree::search_point(int xa,int xb,int ya,int yb, std::vector<std::pair<int, int> > &result) {
if(ptr==nullptr) {return ;}
    KD_Tree::search(xa,xb,ya,yb,result,ptr);
}

inline void KD_Tree::search(int xa,int xb,int ya,int yb, std::vector<std::pair<int, int> > &result,KD_Tree::Node* ptr) {
    if(ptr==nullptr) {return ;}
    if(ptr->dim==0) {
        //spilt by line x=k
        if(ptr->point!=std::make_pair(-1,-1)) {
            if(ptr->point.first>=xa && ptr->point.first<=xb && ptr->point.second>=ya && ptr->point.second<=yb) {
                result.push_back(ptr->point);
            }
            return;//finish this search and return
        }
        if(xb<=ptr->split) {
            //search area at left side
            //so we just continue search left side
            search(xa,xb,ya,yb,result,ptr->left);
            return;
        }
        if(xa>=ptr->split) {
            //search right side
            search(xa,xb,ya,yb,result,ptr->right);
            return;
        }
        search(xa,xb,ya,yb,result,ptr->left);
        search(xa,xb,ya,yb,result,ptr->right);
        return;
    }else {
        //spilt by line y=k
        if(ptr->point!=std::make_pair(-1,-1)) {
            if(ptr->point.first>=xa && ptr->point.first<=xb && ptr->point.second>=ya && ptr->point.second<=yb) {
                result.push_back(ptr->point);
            }
            return;
        }
        if(yb<=ptr->split) {
            search(xa,xb,ya,yb,result,ptr->left);
            return;
        }
        if(ya>=ptr->split) {
            search(xa,xb,ya,yb,result,ptr->right);
            return;
        }
        search(xa,xb,ya,yb,result,ptr->left);
        search(xa,xb,ya,yb,result,ptr->right);
        return;
    }
}

