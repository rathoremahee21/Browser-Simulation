#ifndef RENDERING_LAYER_H
#define RENDERING_LAYER_H
#include <windows.h>
#include<iostream>
#include<vector>
#include<ctime>
#include<stack>
using namespace std;


const int WIDTH = 60;

string line(){
    return "+" + string(WIDTH-2,'-') + "+";
}

void topBar(string url){
    cout<<line()<<"\n";
    string s=" URL: "+url;
    cout<<"|"<<s<<string(WIDTH-s.length()-2,' ')<<"|\n";
    cout<<line()<<"\n";
}

void card(string title, vector<string> body){
    cout<<line()<<"\n";
    cout<<"| "<<title<<string(WIDTH-title.length()-3,' ')<<"|\n";
    cout<<line()<<"\n";

    for(auto &l:body){
        string t=" "+l;
        if(t.length()>WIDTH-2) t=t.substr(0,WIDTH-5)+"...";
        cout<<"|"<<t<<string(WIDTH-t.length()-2,' ')<<"|\n";
    }
    cout<<line()<<"\n\n";
}



class HistoryManager{
    struct Node{
        string url;
        Node* prev;
        Node* next;
        Node(string u):url(u),prev(NULL),next(NULL){}
    };

    Node* head=NULL;
    Node* cur=NULL;

public:
    void visit(string u){
        Node* n=new Node(u);

        if(!head){ head=cur=n; return; }

        if(cur->next){
            Node* t=cur->next;
            while(t){
                Node* d=t;
                t=t->next;
                delete d;
            }
        }

        cur->next=n;
        n->prev=cur;
        cur=n;
    }

    string back(){
        if(!cur||!cur->prev) return "";
        cur=cur->prev;
        return cur->url;
    }

    string forward(){
        if(!cur||!cur->next) return "";
        cur=cur->next;
        return cur->url;
    }

    void show(){
        cout<<"\n--- SESSION HISTORY ---\n";
        Node* t=head;
        while(t){
            cout<<"  "<<t->url<<"\n";
            t=t->next;
        }
        cout<<"-----------------------\n";
    }
};


struct DOMNode{
    string tag;
    vector<DOMNode*> children;
};

class DOMParser{
public:
    DOMNode* parse(string content){
        DOMNode* root=new DOMNode{"html"};
        DOMNode* body=new DOMNode{"body"};
        root->children.push_back(body);

        body->children.push_back(new DOMNode{content});
        return root;
    }
};

class HTMLRenderer{
public:
    void showDOM(DOMNode* root){
        stack<pair<DOMNode*,int>> st;
        st.push({root,0});

        while(!st.empty()){
            auto p = st.top();
            DOMNode* node = p.first;
            int depth = p.second;
            st.pop();

            for(int i=0;i<depth;i++) cout<<"  ";
            cout<<node->tag<<"\n";

            for(int i=node->children.size()-1;i>=0;i--)
                st.push({node->children[i],depth+1});
        }
    }
};


class WebPage{
public:
    virtual void render(string url,string content)=0;
    virtual ~WebPage(){}
};


class GenericPage: public WebPage{
public:
    void render(string url,string content){
        topBar(url);

        card("Generic Page",{
            "Dynamically rendered using DOM",
            "Unknown website fallback",
            content
        });
    }
};


class NewsPage: public WebPage{
public:
    void render(string url,string content){
        topBar(url);

        card("Global Economy",{
            "Markets rising globally",
            "Tech driving growth"
        });

        card("AI Revolution",{
            "Automation increasing"
        });
    }
};

class ShopPage: public WebPage{
public:
    void render(string url,string content){
        topBar(url);
        card("Electronics",{"Phones","Laptops"});
    }
};

class SportsPage: public WebPage{
public:
    void render(string url,string content){
        topBar(url);
        card("Sports",{"Football","Cricket"});
    }
};

class QuotesPage: public WebPage{
public:
    void render(string url,string content){
        topBar(url);
        card("Quotes",{"Stay hungry stay foolish","Early bird catches the worm","One man's trash is another man's treasure"});
    }
};

class CalendarPage: public WebPage{
public:
    void render(string url,string content){
        time_t now=time(0);
        topBar(url);
        card("Time",{ctime(&now)});
    }
};

class GamePage: public WebPage{
public:
    void render(string url,string content){
        char b[3][3]={{'1','2','3'},{'4','5','6'},{'7','8','9'}};

        while(true){
            system("cls");
            topBar(url);

            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++) cout<<b[i][j]<<" ";
                cout<<"\n";
            }

            cout<<"\nEnter 0 to exit: ";
            int m; cin>>m;

            if(m==0){
                cout<<"\nExiting game...\n";
                Sleep(2000); system("cls");
                return;
            }
        }
    }
};

#endif
