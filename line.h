//
// Created by aoyeningluosi on 24-11-5.
//

#ifndef LINE_H
#define LINE_H
#include <algorithm>
#include <vector>

class point {
public:
    float x,y;
    explicit point(float x=0, float y=0):x(x),y(y){}
    float operator *(point& b) const {
        //叉积
        return x*b.y-y*b.x;
    }
};
class vec_line {
public:
    float ax,ay,bx,by;//a vector from a to b

    vec_line(float ax=0, float ay=0, float bx=0, float by=0):ax(ax),ay(ay),bx(bx),by(by){}
    vec_line(point a,point b) {
        ax=a.x;
        bx=b.x;
        ay=a.y;
        by=b.y;
    }
    float operator *(vec_line& b) {
        point temp_a(bx-ax,by-ay);
        point temp_b(b.bx-b.ax,b.by-b.ay);
        return temp_a*temp_b;
    }
};
bool if_cross(vec_line& a,vec_line& b) {
    vec_line afirst_bfirst(a.ax,a.ay,b.ax,b.ay);
    vec_line afirst_blast(a.ax,a.ay,b.bx,b.by);
    vec_line bfirst_afirst(b.ax,b.ay,a.ax,a.ay);
    vec_line bfirst_alast(b.ax,b.ay,a.bx,a.by);
    if((a*afirst_bfirst) * (a*afirst_blast)<0
        &&
        (b*bfirst_afirst) * (b*bfirst_alast)<0) return true;
    else if((a*afirst_bfirst) * (a*afirst_blast)==0 || b*bfirst_afirst*(b*bfirst_alast)==0) return true;
    else return false;
}



class line_segment;
class line_point {
    //the start point or thr end point of one line
public:
    float x,y;
    line_segment *p;//point to its line_segment
    int state;//0 is the start point,1 is the end point
    explicit line_point(float x=0, float y=0,int st=0,line_segment& line):x(x),y(y) {
        if(st==0) st=0;
        else st=1;

        p=&line;
    }
};

class line_segment {
public:
    line_point a,b;//a is for left point,b is for right
    line_segment(line_point a,line_point b):a(a),b(b){}
};

class point_set {
public:
    std::vector<line_segment> lines;
    std::vector<line_point> points;
    point_set()= default;
    void insert(line_segment l) {
        lines.push_back(l);
        points.push_back(l.a);
        points.push_back(l.b);
    }
    void self_sort() {
        sort(points.begin(),points.end(),[](line_point& a,line_point& b)->bool {
            if(a.x<b.x) return true;
            else if(a.x>b.x) return false;
            else if(a.state<b.state) return true;
            else if(a.state>b.state) return false;
            else if(a.y<b.y) return true;
            else if(a.y>b.y) return false;
            else return true;
        });
    }
    bool answer() const {

    }
};


#endif //LINE_H
