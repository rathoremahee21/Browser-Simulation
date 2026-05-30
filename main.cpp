
#include<iostream>
#include<windows.h>
#include "browser_engine.h"
using namespace std;

void Intro(){
    cout<<"========================================\n";
    cout<<"       CONSOLE BROWSER SIMULATION\n";
    cout<<"========================================\n";
}

int main(){
    Intro();

    Browser browser;

    cout<<"Initializing modules...\n";
    cout<<"Network | Cache | Renderer | Parser\n\n";
    Sleep(3000);
    system("cls");

    browser.start();

    cout<<"\nThank you.\nExiting browser...\n";
    return 0;
}
