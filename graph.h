//
// Created by aoyeningluosi on 24-11-17.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <cfloat>
#include <numeric>
#include <set>
#include <valarray>
#include <vector>
#include <bits/fs_fwd.h>

#include "line_segment_tree.h"

#endif //GRAPH_H

namespace Graph {
    class point {
    public:
        float x,y;
        point()=default;
        point(const float _x, const float _y) :x(_x),y(_y){}
        bool operator==(const point &other) const {return x==other.x && y==other.y;}
    };
    struct line_vec {
        point p{0,0},q{0,0};
        float x=0,y=0;
        //notice:p is the left point,p to q
        line_vec()=default;
        line_vec(const point& p, const point& q) :p(p),q(q),x(q.x-p.x),y(q.y-p.y){}
        line_vec(float p_x,float p_y,float q_x,float q_y) :p(p_x,p_y),q(q_x,q_y),x(q.x-p.x),y(q.y-p.y){}
        line_vec(const line_vec &other)= default;
        static auto cmp(const line_vec& l1,const line_vec& l2) {
            //a1b2 - a2b1
            return l1.x*l2.y-l1.y*l2.x;
        }
        static auto cross(const line_vec& a, const line_vec& b) {
            const line_vec al(a.p,b.q),ar(a.p,b.p),bl(b.p,a.p),br(b.p,a.q);
            return ((cmp(a,al)>=0 && cmp(a,ar)<=0) || (cmp(a,al)<=0 && cmp(a,ar)>=0))&& ((cmp(b,bl)>=0 && cmp(b,br)<=0) || (cmp(b,bl)<=0 && cmp(b,br)>=0));
        }
        static auto cross_point(const line_vec& a, const line_vec& b) {
            if(!cross(a,b))
                return point(FLT_MIN,FLT_MIN);
            const float m1=(a.q.y-a.p.y)/(a.q.x-a.p.x),m2=(b.q.y-b.p.y)/(b.q.x-b.p.x);
            const float b1=a.p.y-m1*a.p.x,b2=b.p.y-m2*b.p.x;
            return point((b2-b1)/(m1-m2),(m1*b2-m2*b1)/(m1-m2));
        }
    };
    struct light {
        point init_point,advance_point;
        light(const point& p,const point& q):init_point(p),advance_point(q){}
    };
    class graph {
        enum class state{non,fin};//when this graph is ok,state==fin
        point center_point{FLT_MIN,FLT_MIN};
        float circle_r=0;
        std::vector<line_vec> lines;
    public:
        [[nodiscard]] auto get_center()const{return std::make_pair(center_point,circle_r);}
        state graph_state= state::non;
        graph()=default;
        explicit graph(const std::vector<line_vec>& _lines) : lines(_lines){}

        void assign_center() {
            center_point=point{0,0};
            std::accumulate(lines.begin(),lines.end(),center_point,[](const point& center,const line_vec & add)
                ->point{return point{center.x+add.p.x,center.y+add.p.y};});
            center_point.x/=static_cast<float>(lines.size());center_point.y/=static_cast<float>(lines.size());
            for(auto &l:lines)
                circle_r=sqrt(std::pow(l.p.x-center_point.x,2)
                    +std::pow(l.p.y-center_point.y,2)) > circle_r?static_cast<float>(sqrt(std::pow(l.p.x-center_point.x,2)
                        +std::pow(l.p.y-center_point.y,2))):circle_r;
        }

        void check_finish() {
            if(lines.size()<2){return;}
            if((lines.end()-1)->q==lines.begin()->p){graph_state=state::fin;assign_center();}
        }

        void add_line(const point& p, const point& q) {
            if(graph_state==state::fin)
                return;
            if(lines.empty())
                lines.emplace_back(p,q);
            else if(const point& last_point=(lines.end()-1)->q; last_point.x==p.x && last_point.y==p.y) {
                lines.emplace_back(p,q);
                check_finish();
            }
        }
        bool light_if_cross(const light& l) {
            for(const auto &line:lines) {
                auto a1=l.init_point.y-l.advance_point.y
                ,b1=l.advance_point.x-l.init_point.x
                ,d1=l.init_point.x*(l.init_point.y-l.advance_point.y)+l.init_point.y*(l.advance_point.x-l.init_point.x);
                auto a2=line.p.y-line.q.y
                ,b2=line.q.x-line.p.x
                ,d2=line.p.x*(line.p.y-line.q.y)+line.p.y*(line.q.x-line.p.x);
                auto x=(b2*d1-b1*d2)/(a1*b2-a2*b1),y=(a1*d2-a2*d1)/(a1*b2-a2*b1);
                auto light_alpha=(x-l.init_point.x)/(l.advance_point.x-l.init_point.x),vec_line_alpha=(x-line.p.x)/(line.q.x-line.p.x);
                if(light_alpha>=0 && vec_line_alpha>=0 && vec_line_alpha<=1)
                    return true;
            }
            return false;
        }

        std::vector<point> light_cross_point(const light& l) {
            std::vector<point> res;
            for(const auto &line:lines) {
                auto a1=l.init_point.y-l.advance_point.y
                ,b1=l.advance_point.x-l.init_point.x
                ,d1=l.init_point.x*(l.init_point.y-l.advance_point.y)+l.init_point.y*(l.advance_point.x-l.init_point.x);
                auto a2=line.p.y-line.q.y
                ,b2=line.q.x-line.p.x
                ,d2=line.p.x*(line.p.y-line.q.y)+line.p.y*(line.q.x-line.p.x);
                auto x=(b2*d1-b1*d2)/(a1*b2-a2*b1),y=(a1*d2-a2*d1)/(a1*b2-a2*b1);
                auto light_alpha=(x-l.init_point.x)/(l.advance_point.x-l.init_point.x),vec_line_alpha=(x-line.p.x)/(line.q.x-line.p.x);
                if(light_alpha>=0 && vec_line_alpha>=0 && vec_line_alpha<=1)
                    res.emplace_back(x,y);
            }
            return res;
        }
    };
class block_split_tree_node {
public:
    float split{0};
    std::shared_ptr<block_split_tree_node> parent;
    std::shared_ptr<block_split_tree_node> left,right;
    std::vector<graph> graph_on_split;
    block_split_tree_node()=default;
    block_split_tree_node(const std::shared_ptr<block_split_tree_node>& _parent,
        std::vector<graph> graphs) :parent(_parent){
        if(graphs.empty()){left=right=nullptr;return;}
        if(graphs.size()==1){
            graph_on_split.push_back(graphs[0]);
            left=right=nullptr;split=graphs[0].get_center().first.y;
            return;
        }
        std::ranges::sort(graphs,[](const graph& a,const graph& b)
            {return a.get_center().first.y<b.get_center().first.y;});
        split=graphs[graphs.size()/2].get_center().first.y;
        graphs.erase(graphs.begin()+graphs.size()/2);
        std::vector<graph> left_pass,right_pass;
        for(auto& p:graphs) {
            if(p.get_center().first.y-p.get_center().second<=split && p.get_center().first.y+p.get_center().second>=split) {
                graph_on_split.push_back(p);
                continue;
            }
            if(p.get_center().first.y+p.get_center().second<=split) {
                left_pass.push_back(p);
                continue;
            }
            right_pass.push_back(p);
        }
        left=std::make_shared<block_split_tree_node>(std::shared_ptr<block_split_tree_node>(this),left_pass);
        right=std::make_shared<block_split_tree_node>(std::shared_ptr<block_split_tree_node>(this),right_pass);
    }
};
class block_split_tree {
    public:
    std::shared_ptr<block_split_tree_node> root;
    explicit block_split_tree(const std::vector<graph> &graphs) {
        root=std::make_shared<block_split_tree_node>(nullptr,graphs);
    }
};
    class ground_block {
        float left=0,right=0;
        std::vector<graph> graphs;
        std::shared_ptr<block_split_tree> ptr;
    public:
        std::shared_ptr<block_split_tree> get_ptr(){return ptr;}
        ground_block(const float& _left,const float& _right):left(_left),right(_right){}
        [[nodiscard]] bool contain(const std::pair<point,float>& p) const {
            return !(p.first.x+p.second<left || p.first.x-p.second>right);//contain is true
        }
        void attend(const graph& g){graphs.push_back(g);}
        void cut() {
            ptr=std::make_shared<block_split_tree>(graphs);
        }
        std::vector<graph> light_find(light _light,const std::shared_ptr<block_split_tree>& pointer) {
            //first light_find
            std::vector<graph> result;
            if(pointer==nullptr) return std::vector<graph>{0};
            auto& target_vec=pointer->root->graph_on_split;
            for(auto& g:target_vec) {
                //in this target vector ,find those graph who can cross with light
                if(g.light_if_cross(_light))
                    result.push_back(g);
            }
            light_find(_light,pointer->root->left,result);
            light_find(_light,pointer->root->right,result);
            return result;
        }
        void light_find(light _light,const std::shared_ptr<block_split_tree_node>& pointer,std::vector<graph>& result) {
            if(pointer==nullptr) return;
            for(auto& target_vec=pointer->graph_on_split; auto& g:target_vec) {
                //in this target vector ,find those graph who can cross with light
                if(g.light_if_cross(_light))
                    result.push_back(g);
            }
            light_find(_light,pointer->left,result);
            light_find(_light,pointer->right,result);
        }
    };
    class ground {
        int graph_number=0,block_number=0;
        float length=0;
        point left_lower_corner{FLT_MAX,FLT_MAX},right_upper_corner{FLT_MIN,FLT_MIN};
        std::vector<float> split;
        std::vector<ground_block> blocks;
    public:
        ground()=default;
        std::vector<std::pair<point,float>> point_pairs;
        void init_ground(const std::vector<graph>& graph_set) {
            std::ranges::for_each(graph_set,[this](const graph& g){point_pairs.push_back(g.get_center());++graph_number;});
            const auto x_minmax=std::minmax(point_pairs.begin(),point_pairs.end(),
                [](const auto& a,const auto&b){return a.first.x<b.first.x;});
            const auto y_minmax=std::minmax(point_pairs.begin(),point_pairs.end(),
                [](const auto& a,const auto&b){return a.first.y<b.first.y;});
            left_lower_corner=point{x_minmax.first->first.x,y_minmax.first->first.y};
            right_upper_corner=point{x_minmax.second->first.x,y_minmax.second->first.y};
            length=right_upper_corner.x-left_lower_corner.x;
            block_number=graph_number/5;
            const float step=length/static_cast<float>(block_number);
            for(int i=0;i<=block_number;i++)
                split.push_back(left_lower_corner.x+step*static_cast<float>(i));
            for(int i=0;i<block_number;i++)blocks.emplace_back(split[i],split[i+1]);
            for(const auto& g:graph_set) {
                const auto target=std::ranges::find_if(blocks,
                    [g](const auto& blk){return blk.contain(g.get_center());});
                target->attend(g);
            }
            std::ranges::for_each(blocks,[](const auto&block){block.cut();});
        }
        void light_find(const light& target_light,std::vector<graph>& result) {
            auto target=std::find_if(blocks.begin(),blocks.end(),
                [target_light](const ground_block& _b){return _b.contain(std::make_pair(target_light.init_point,0));});
            std::for_each(target,blocks.end(),[&target_light,&result](ground_block& b)
                {auto arr=b.light_find(target_light,b.get_ptr());std::ranges::copy(arr,std::back_inserter(result));});
        }
    };
}
