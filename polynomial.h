//
// Created by aoyeningluosi on 24-11-17.
//

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <algorithm>
#include <map>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>



#endif //POLYNOMIAL_H
//TO BE CONTINUED...
template<class T>
class term {
public:
    T num;
    std::vector<std::pair<char,T>> letters;//the term without num
    term()=default;
    term(T num, std::vector<std::pair<char,T>> letters):num(num),letters(letters) {}
    void write(T num_w,const std::vector<std::pair<char,T>>& letters_w) {
        letters=letters_w;
        num=num_w;
    }
    void write(T num_w){num=num_w;}
    void write(std::vector<std::pair<char,T>> letters_w) {letters=letters_w;}
    void multiply(const char letter,const T ts) {
        //letters.push_back(std::make_pair(letter,ts));
        auto itr=std::find_if(letters.begin(),letters.end(),[letter](const std::pair<char,T>& p) {if(p.first==letter)return true;});
        if(itr==letters.end()) {
            /*did not find it*/
            letters.emplace_back(letter,ts);
        }
        else
            itr->second+=ts;
    }
    term operator+(const term& t) const {
        std::sort(letters.begin(),letters.end(),[](auto p1,auto p2) {return p1.first<p2.first;});

    }
    void del(char letter) {

    }
};


class Polynomial {
    public:
};