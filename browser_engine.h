#ifndef BROWSER_ENGINE_H
#define BROWSER_ENGINE_H

#include<windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include "network_layer.h"
#include "rendering_layer.h"

using namespace std;

class User{
public:
    string email;
    string getUsername(){
        return email.substr(0,email.find('@'));
    }
};

class UserManager{
public:
    bool validEmail(string e){
        return (e.find('@')!=string::npos && e.find(".com")!=string::npos && e.find(' ')==string::npos);
    }

    void signup(string e,string p){
        ofstream f("users.bin",ios::app);
        f<<e<<" "<<p<<"\n";
    }

    bool login(string e,string p){
        ifstream f("users.bin");
        string a,b;
        while(f>>a>>b)
            if(a==e && b==p)
                return true;
        return false;
    }

    void showUsers(){
        ifstream f("users.bin");
        string a,b;
        cout<<"\n USERS:\n";
        while(f>>a>>b) cout<<a<<endl;
    }
};

//=================================================================
template<typename T>
class Tab{
    string URL;

public:
    Tab(string u){ URL=u; }

    string getURL(){ return URL; }

    void setURL(string u){ URL = u; }

    string getTitle(){
        int start = URL.find("www.");
        int end = URL.find(".");
        if(start!=string::npos && end!=string::npos)
            return URL.substr(start+4, end-(start+4));
        return URL;
    }
};

// =============================================================
class Browser{

    struct ThemeSet{
        string primary, secondary, accent, faint;
    } theme;

    void setTheme(int t){
        switch(t){
        case 1: theme={"\033[1;37m","\033[0;37m","\033[1;36m","\033[2m"}; break;
        case 2: theme={"\033[1;37m","\033[0;37m","\033[1;30m","\033[2m"}; break;
        case 3: theme={"\033[1;32m","\033[0;32m","\033[1;30m","\033[2;32m"}; break;
        case 4: theme={"\033[1;31m","\033[0;31m","\033[1;37m","\033[2;31m"}; break;
        case 5: theme={"\033[1;34m","\033[0;34m","\033[1;36m","\033[2;34m"}; break;
        }
    }

    string reset(){ return "\033[0m"; }
    void clearScreen(){ system("cls"); }

    HistoryManager history;
    queue<string> notifications;

    struct Node{
        int id;
        Tab<int> tab;
        Node* next;
        Node* prev;
        Node(string u,int i):id(i),tab(u),next(this),prev(this){}
    };

    Node* active=nullptr;
    Node* head=nullptr;
    int tabCount=0;

    Cache cache;
    PageDatabase db;
    RequestPipeline pipeline;
    HTMLRenderer renderer;
    DOMParser parser;

    UserManager um;
    User user;

    bool guest=false;
    int visits=0;

private:

    void traverseDOM(DOMNode* root){
        if(!root) return;
        stack<DOMNode*> st;
        st.push(root);

        while(!st.empty()){
            DOMNode* node = st.top();
            st.pop();
            for(auto c : node->children)
                st.push(c);
        }
    }

    void showQueue(){
        while(!notifications.empty()){
            cout << "\033[2m" << notifications.front() << "\033[0m\n";
            notifications.pop();
        }
    }

    WebPage* createPage(string url){
        if(url=="www.news.com") return new NewsPage();
        if(url=="www.shop.com") return new ShopPage();
        if(url=="www.sports.com") return new SportsPage();
        if(url=="www.quotes.com") return new QuotesPage();
        if(url=="www.calendar.com") return new CalendarPage();
        if(url=="www.game.com") return new GamePage();
        return nullptr; // important, Mahee will elaborate in viva
    }

public:
    Browser():pipeline(cache, db){ setTheme(1); }

    void line(){ cout<<theme.secondary<<"----------------------------------------\n"<<reset(); }
    void doubleline(){ cout<<theme.primary<<"========================================\n"<<reset(); }

    void tabBar(){
        if(!active) return;
        Node* t = head;

        cout<<theme.secondary<<"\n";
        for(int i=0;i<tabCount;i++){
            string title = t->tab.getTitle();
            if(t==active)
                cout<<theme.accent<<" [ "<<title<<" ] "<<theme.secondary;
            else
                cout<<"   "<<title<<"   ";
            cout<<"|";
            t = t->next;
        }
        cout<<"\n"<<reset();
    }

// =============================================================
    void start(){
        while(true){
            clearScreen();
            doubleline();
            cout<<theme.accent<<"\tCONSOLE BROWSER\n"<<reset();
            doubleline();

            cout<<"1.Sign Up\n2.Login\n3.Guest\n4.Inspect\n0.Exit\n";
            int ch; cin>>ch;

            if(ch==0) return;
            if(ch==1) signup();
            if(ch==2) login();
            if(ch==3){ guest=true; home(); }
            if(ch==4) inspect();
        }
    }

    void signup(){
        string e,p;
        cout<<"Email: "; cin>>e;
        if(!um.validEmail(e)){ cout<<"Invalid\n"; Sleep(2000); return; }

        cout<<"Password: "; cin>>p;
        if(p.size()<8){ cout<<"Weak\n"; Sleep(2000); return; }

        um.signup(e,p);
        user.email=e;
        guest=false;
        home();
    }

    void login(){
        string e,p;
        cout<<"Email: "; cin>>e;
        cout<<"Password: "; cin>>p;

        if(um.login(e,p)){
            user.email=e;
            home();
        } else{
            cout<<"Login failed\n";
            Sleep(2000);
        }
    }

 //=============================================================
    void newTab(){
        Node* n = new Node("www.home.com", tabCount+1);

        if(!head){
            head = active = n;
        } else{
            Node* last = head->prev;
            last->next = n;
            n->prev = last;
            n->next = head;
            head->prev = n;
            active = n;
        }
        tabCount++;
    }

    void closeTab(){
        if(tabCount==1) return;
        clearScreen();
        Node* t = active;
        if(t==head) head=head->next;

        t->prev->next = t->next;
        t->next->prev = t->prev;

        active = t->next;
        delete t;
        tabCount--;
    }

    bool switchTab(){
        if(tabCount==1){
            cout<<"Only one tab\n";
            Sleep(2000);
            return false;
        }
        cout<<"1.Prev 2.Next\n";
        int ch; cin>>ch;

        if(ch==1) active=active->prev;
        if(ch==2) active=active->next;
        return true;
    }

    void home(){
        clearScreen();
        if(!active) newTab();

        while(true){
            cout<<theme.primary<<(guest?"WELCOME GUEST\n":"WELCOME "+user.getUsername()+"\n");

            tabBar();
            doubleline();

            string url = active->tab.getURL();

            if(url=="www.home.com"){
                cout<<"1.Enter URL \n2.Shortcuts \n3.New Tab \n4.Switch Tab \n5.Settings \n0.Exit\n";
                int ch; cin>>ch;

                if(ch==0) break;
                if(ch==1) enterURL();
                if(ch==2) shortcuts();
                if(ch==3) {clearScreen(); newTab();}
                if(ch==4 && switchTab())
                    openURL(active->tab.getURL(),false);
                if(ch==5) settings();
            }
            else{
                cout<<"1.Back 2.Forward 3. New URL 4.NewTab 5.Switch 6.Close 7.Print 8.Inspect 0.Home\n";
                int ch; cin>>ch;

                if(ch==0){ active->tab.setURL("www.home.com"); clearScreen(); continue; }
                if(ch==1) back();
                if(ch==2) forward();
                if(ch==3) enterURL();
                if(ch==4) {clearScreen(); newTab();}
                if(ch==5 && switchTab()) openURL(active->tab.getURL(),false);
                if(ch==6) closeTab();
                if(ch==7) print();
                if(ch==8) inspectPage();
            }
        }
    }

    void openURL(string u,bool addHistory=true){

        if(!(u.find("www.")==0 &&
            (u.find(".com")!=string::npos ||
             u.find(".org")!=string::npos ||
             u.find(".in")!=string::npos))){
            cout<<"[Error] Invalid URL\n";
            Sleep(3000); clearScreen();
            active->tab.setURL("www.home.com");
            return;
        }
        clearScreen();
        notifications.push("[Processing Request]");
        notifications.push("[Network Fetch]");
        notifications.push("[Rendering Page]");
        showQueue();

        Page p = pipeline.process(u);
        DOMNode* dom = parser.parse(p.content);
        traverseDOM(dom);

        active->tab.setURL(u);
        visits++;

        if(addHistory && u!="www.home.com")
            history.visit(u);

        doubleline();

WebPage* page = createPage(u);

if(page){

    page->render(u, p.content);
    delete page;
}
else{
    topBar(u);

    vector<string> lines;
    lines.push_back("Dynamic DOM Rendered Page");
    lines.push_back("Valid URL. No predefined template found, hence directing to fallback");
    lines.push_back("Content: " + p.content);

    card("Generic Page (DOM)", lines);
}
    }

    void enterURL(){
        string u; cout<<"Enter URL: "; cin>>u;
        openURL(u,true);
    }

    void shortcuts(){
        cout<<"1.home 2.shop 3.news 4.sports 5.game\n";
        int ch; cin>>ch;
        clearScreen();
        if(ch==1) openURL("www.home.com");
        if(ch==2) openURL("www.shop.com");
        if(ch==3) openURL("www.news.com");
        if(ch==4) openURL("www.sports.com");
        if(ch==5) openURL("www.game.com");
    }

    void back(){
        string u=history.back();
        if(u==""){ cout<<"No back\n"; Sleep(1500); clearScreen();return; }
        openURL(u,false);
    }

    void forward(){
        string u=history.forward();
        if(u==""){ cout<<"No forward\n"; Sleep(1500);clearScreen(); return; }
        openURL(u,false);
    }

    void inspectPage(){
        Page p = pipeline.process(active->tab.getURL());
        DOMNode* d = parser.parse(p.content);
        renderer.showDOM(d);
        cout<<"\nPress x to exit...";
        string t; cin>>t; clearScreen();
    }

    void print(){
        ofstream f("page.txt");
        Page p = pipeline.process(active->tab.getURL());
        f<<p.content;
        cout<<"Printed\n";
        Sleep(2000); clearScreen();
    }

    void settings(){
        cout<<"1.History \t2.Theme \t0.Back\n";
        int ch; cin>>ch;

        if(ch==1){
            history.show();
            string t; cin>>t;
        }
        if(ch==2){
            int t; cin>>t;
            setTheme(t);
        }
    }

    void inspect(){
        um.showUsers();
        cout<<"\nVisits: "<<visits<<endl;
        db.showAll();
        Sleep(5000);
    }
};

#endif
