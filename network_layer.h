#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include<iostream>
#include<map>
#include<queue>
using namespace std;

struct Page{
    string content;
};

class PageDatabase{
    map<string,Page> pages;

public:
    PageDatabase(){
        pages["www.home.com"] = {"Home Page"};
        pages["www.shop.com"] = {"Shop Page"};
        pages["www.news.com"] = {"News Page"};
        pages["www.sports.com"] = {"Sports Page"};
        pages["www.quotes.com"] = {"Quotes Page"};
        pages["www.calendar.com"] = {"Calendar Page"};
        pages["www.game.com"] = {"Game Page"};
    }

    bool exists(string url){
        return pages.find(url)!=pages.end();}

    Page getPage(string url){
        return pages[url];}

    void showAll(){
        cout<<"\n DATABASE \n";
        cout<<"========================================\n";
        for(auto &p:pages)
            cout<<p.first<<endl;
    }
};

class Cache{
    map<string, Page> storage;

public:
    bool exists(string url){
        return storage.find(url)!=storage.end();}

    void put(string url, Page p){
        storage[url] = p;}

    Page get(string url){
        return storage[url];}
};

class RequestPipeline{
    Cache &cache;
    PageDatabase &db;
    queue<string> requestQueue;

public:
    RequestPipeline(Cache &c, PageDatabase &d): cache(c), db(d) {}

    Page process(string url){
        requestQueue.push(url);
        string u = requestQueue.front();
        requestQueue.pop();

        if(!db.exists(u)){
            return {"Dynamic Content for "+u};}
        if(cache.exists(u)){
            return cache.get(u);}

        Page p = db.getPage(u);
        cache.put(u,p);

        return p;
    }
};

#endif
