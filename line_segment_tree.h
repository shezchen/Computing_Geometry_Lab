#ifndef LINE_SEGMENT_TREE_H
#define LINE_SEGMENT_TREE_H
#include <algorithm>
#include <cfloat>
#include <memory>
#include <ranges>
#include <vector>

#endif //LINE_SEGMENT_TREE_H
//这是文章第一部分线段树的代码，我已经测试过了！
namespace line_segment_tree {
    class point {
    public:
        float x,y;
        point()=default;
        point(const float _x, const float _y) :x(_x),y(_y){}
    };

    class line {
    public:
        point p,q;
        //notice:p is the left point
        line(const point& _p,const point& _q){
            if(_p.x>_q.x) {p=_q;q=_p;}else{p=_p;q=_q;}
        }
    };
    class node {
    public:
        float split,min,max;
        std::vector<line> lines;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
        std::shared_ptr<node> pred;
        node():split(0),min(FLT_MIN),max(FLT_MAX),left(nullptr),right(nullptr),pred(nullptr){}
        node(float _min,float _max):split(0),min(_min),max(_max),left(nullptr),right(nullptr),pred(nullptr){}
    };
    class segment_tree {
    public:
        std::shared_ptr<node> root;
        void build_tree(const std::vector<line>& lines,const std::vector<point>& points,float min,float max);
        static auto give_min(const float tar,float& min){min=min<=tar?min:tar;}
        static auto give_max(const float tar,float& max){max=max>=tar?max:tar;}
        explicit segment_tree(const std::vector<line>& lines) {
            //give lines to make this tree
            std::vector<point> points;
            float min=FLT_MAX,max=FLT_MIN;
            for(const auto& line : lines) {
                give_min(line.p.x,min);
                give_max(line.p.x,max);
                give_min(line.q.x,min);
                give_max(line.q.x,max);
                points.push_back(line.p);
                points.push_back(line.q);
            }
            root = std::make_shared<node>(min,max);
            std::ranges::sort(points,[](const point& a,const point& b)->bool{return a.x<b.x;});
            build_tree(root,lines,points,min,max);
        }
        static bool range_search(const std::shared_ptr<node>& p_node,const line& _line) {
            if(p_node==nullptr){return true;}
            return _line.p.x<=p_node->min && _line.q.x>=p_node->max;
        }
        static void build_tree(const std::shared_ptr<node>& p_node,const std::vector<line>& lines,const std::vector<point>& points,
                            const float min,const float max) {
            //[min,max) is the range,p_node is the ptr points to the obj.
            //vector lines and points is which to be built into the tree.
            if(points.empty()) return;
            std::vector<line> newlines_l,newlines_r;
            std::vector<point> newpoints_l,newpoints_r;
            p_node->split=points[points.size()/2].x;
            for(auto& point : points) {
                if(point.x<p_node->split) newpoints_l.push_back(point);
                if(point.x>p_node->split) newpoints_r.push_back(point);
                //this make the point vector for left and right children
            }
            for(auto& line : lines) {
                if(range_search(p_node,line) && !range_search(p_node->pred,line)) {
                    p_node->lines.push_back(line);
                    continue;
                }
                if(line.p.x<p_node->split) newlines_l.push_back(line);
                else newlines_r.push_back(line);
            }
            p_node->left=std::make_shared<node>(min,p_node->split);
            p_node->left->pred=p_node;
            build_tree(p_node->left,newlines_l,newpoints_l,min,p_node->split);
            p_node->right=std::make_shared<node>(p_node->split,max);
            p_node->right->pred=p_node;
            build_tree(p_node->right,newlines_r,newpoints_r,p_node->split,max);
        }
        void get_answer(const float t,std::vector<line>& answer) {
            answer.clear();
            if(t<root->min || t>root->max) return;
            if(root==nullptr) return;
            answer.insert(answer.end(),root->lines.begin(),root->lines.end());
            if(t<=root->split) get_answer(t,answer,root->left);
            else get_answer(t,answer,root->right);
        }
        void get_answer(const float t,std::vector<line>& answer,const std::shared_ptr<node>& p_node) {
            if(p_node==nullptr) return;
            answer.insert(answer.end(),p_node->lines.begin(),p_node->lines.end());
            if(t<p_node->split) {get_answer(t,answer,p_node->left);}
            else{get_answer(t,answer,p_node->right);}
        }
    };
}