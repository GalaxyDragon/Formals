#include <iostream>
#include <stack>
#include <string>
#include "gtest/gtest.h"
#include <deque>
using std::string;
using std::stack;
using std::cin;
using std::pair;
using std::cout;
using std::deque;
struct Node{
    virtual bool string_in(string& s)=0;
    virtual ~Node(){};
};
class SymbolNode: public Node {
 public:
    explicit SymbolNode(char c): symb(c){}
    bool string_in(string &s) override {
        if(s.size()!=1){
            return false;
        }
        return s[0]==symb;
    }
 private:
    char symb;

};
class StarNode: public Node {
 public:
    StarNode(Node* n):understar(n){}
    ~StarNode(){delete understar;}
    bool string_in(string &s) override {
        if(s.empty()){
            return true;
        }
        if(s.size()==1){
            return understar->string_in(s);
        }
        bool ans = false;
        for(int i=1; i<s.size()+1;++i){
            string s1 = s.substr(0,i);
            string s2 = s.substr(i,s.size());
            ans = ans||(understar->string_in(s1)&&string_in(s2));
            if(ans){
                return ans;
            }
        }
        return ans;
    }
 private:
    Node* understar;
};
class PlusNode: public Node {
 public:
    PlusNode(Node* n1, Node* n2): left_p(n1),right_p(n2){}
    bool string_in(string &s) override {
        return left_p->string_in(s) || right_p->string_in(s);
    }
    ~PlusNode(){delete left_p;delete right_p;}
 private:
    Node* left_p;
    Node* right_p;
};
class ConcNode: public Node {
 public:
    ConcNode(Node* n1, Node* n2): left(n1),right(n2){}
    ~ConcNode(){delete left; delete right;}
    bool string_in(string &s) override {
        bool ans = false;
        for(int i=0; i<s.size()+1;++i){
            string s1 = s.substr(0,i);
            string s2 = s.substr(i,s.size());
            ans = ans||(left->string_in(s1)&&right->string_in(s2));
            if(ans){
                return ans;
            }
        }
        return ans;
    }
 private:
    Node* left;
    Node* right;
};
class Regexp{
 public:
    Regexp(string& s){
        int cnt = 0;
        for(char c:s){
            if(c=='a'||c=='b'||c=='c'){
               cnt++;
            }
            if(c=='.' || c=='+'){
                cnt--;
            }
            if(cnt<=0){
                return ;
            }
        }
        if(cnt!=1){
            return;
        }
        stack<Node*> body;
        for(char c:s){
            if(c=='a'||c=='b'||c=='c'){
                SymbolNode* a = new SymbolNode(c);
                body.push(dynamic_cast<Node*>(a));
            }
            if(c=='*'){
                auto contained = body.top();
                body.pop();
                StarNode* total = new StarNode(contained);
                body.push(dynamic_cast<Node*>(total));
            }
            if(c=='+'){
                auto left = body.top();
                body.pop();
                auto right = body.top();
                body.pop();
                PlusNode* total = new PlusNode(right,left);
                body.push(dynamic_cast<Node*>(total));

            }
            if(c=='.'){
                auto left = body.top();
                body.pop();
                auto right = body.top();
                body.pop();
                ConcNode* total = new ConcNode(right,left);
                body.push(dynamic_cast<Node*>(total));
            }
        }
        initial_node = body.top();
    }
    bool is_correct(){
        return initial_node!= nullptr;
    }
    bool s(string& ss){
        return initial_node->string_in(ss);
    }
    int longest_sub(string &s){
        int ans = -1;
        for(int i=0; i<s.size()+1;++i){
            for(int j=0;j<s.size()-i+1;++j){
                string s1 = s.substr(i,j);
                if(initial_node->string_in(s1)) {
                    ans = std::max(ans, j);
                }
                }
        }
        return ans;
    }
    ~Regexp(){delete initial_node;}
 private:
    Node* initial_node= nullptr;

};
int main() {
    string s;
    cin>>s;
    Regexp x(s);
    string u;
    cin>>u;
    if(!x.is_correct()){
        cout<<"ERROR";
        return 0;
    }
    int ans = x.longest_sub(u);
    (ans==-1)?cout<<"INF":cout<<ans;
    return 0;

 }
