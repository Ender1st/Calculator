#include"config.h"
#include<vector>
#include<cmath>
#include<sstream>
#include<string>
#include<iostream>
using namespace std;
namespace nval {
    bool isSign = 0;
    int num = 1, fa = 2, lson = 4, rson = 8;
    double power = 0.5, value = 0.25;
    char sign = 64;
}
Tree::Node::Node() {}
Tree::Node::Node(char sign, int num, int root, int preFa) {
    power = 1;
    this->sign = sign;
    isSign = true;
    this->num = num;
    if(sign == '+' || sign == '-') {
        fa = 0;
        lson = root;
    }
    else {
        fa = preFa;
        lson = num - 1;
    }
}
Tree::Node::Node(double value, int num) {
    power = 1;
    this->num = num;
    this->value = value;
    fa = num - 1;
    isSign = false;
}
void Tree::Node::change_val(const int& val, const int& opt) {
    if(opt == 2) fa = val;
    if(opt == 4) lson = val;
    if(opt == 8) rson = val;
}
void Tree::Node::change_val(const double& val, const double& opt) {
    if(opt == 0.5) power = val;
}
int Tree::Node::get_val(const int& opt) {
    if(opt == 1) return num;
    if(opt == 2) return fa;
    if(opt == 4) return lson;
    if(opt == 8) return rson;
    return 0;
}
double Tree::Node::get_val(const double& opt) {
    if(opt == 0.5) return power;
    if(opt == 0.25) return value;
    return 0;
}
char Tree::Node::get_val(const char& opt) {
    if(opt == 64) return sign;
    return 0;
}
bool Tree::Node::get_val(const bool& opt) {
    if(opt == 0) return isSign;
    return 0;
}
void Tree::add_node(char sign) {
    int pre = cnt - 1, preFa = ptrNode[pre]->get_val(nval::fa);
    ptrNode.push_back(new Node(sign, cnt, root, preFa));
    if(sign == '+' || sign == '-') {
        ptrNode[root]->change_val(cnt, nval::fa);
        root = cnt;
    }
    else {
        ptrNode[preFa]->change_val(cnt, nval::rson);
        ptrNode[pre]->change_val(cnt, nval::fa);
    }
    cnt++;
}
void Tree::add_node(double value) {
    if(cnt) {
        int pre = cnt - 1;
        ptrNode[pre]->change_val(cnt, nval::rson);
    }
    ptrNode.push_back(new Node(value, cnt));
    cnt++;
}
double Tree::dfs(int num) {
    Node& now = *(ptrNode[num]);
    int lson = now.get_val(nval::lson), rson = now.get_val(nval::rson);
    if(!now.get_val(nval::isSign)) return now.get_val(nval::value);
    double lval = dfs(lson);
    double rval = dfs(rson);
    lval = pow(lval, ptrNode[lson]->get_val(nval::power));
    rval = pow(rval, ptrNode[rson]->get_val(nval::power));
    if(err) return 0;
    char sign = now.get_val(nval::sign);
    if(sign == '/' && abs(rval) < 1e-5) {
        err = 1;
        return 0;
    }
    if(sign == '+') return lval + rval;
    if(sign == '-') return lval - rval;
    if(sign == '*') return lval * rval;
    if(sign == '/') return lval / rval;
    return 0;
}
Tree::Tree() {}
Tree::Tree(string str) {
    cnt = 0;
    root = 0;
    err = 0;
    stringstream form(str);
    string s;
    while(form >> s) {
        if(s == "^") {
            form >> s;
            ptrNode.back()->change_val(stod(s), nval::power);
        }
        else if(s == "+" || s == "-" || s == "*" || s == "/") {
            add_node(s[0]);
        }
        else{
            add_node(stod(s));
        }
    }
}
double Tree::get_value() {
    return dfs(root);
}
Tree::~Tree() {
    for(int i = 0; i < cnt; i++) {
        delete ptrNode[i];
        ptrNode.pop_back();
    }
}