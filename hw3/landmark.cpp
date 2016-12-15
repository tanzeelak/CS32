//
//  main.cpp
//  cs32hw3
//
//  Created by Tanzeela Khan on 2/4/16.
//  Copyright © 2016 me. All rights reserved.
//
//PROBLEM 1

#include <iostream>
#include <string>
using namespace std;


class Landmark
{
public:
    Landmark(string name)
    {
        thename = name;
    }
    string color() const{
        return thecolor;
    }
    virtual void b() const = 0;
    void setColor(string color)
    {
        thecolor = color;
    }
    string icon() const
    {
        return theicon;
    }
    void setIcon(string icon)
    {
        theicon = icon;
    }
    string name() const
    {
        return thename;
    }
    virtual~Landmark()
    {
        
    }
    
private:
    string thecolor;
    string theicon;
    string thename;
};




class Hotel: public Landmark
{
public:
    Hotel(string a):Landmark(a)
    {
        setIcon("bed");
        setColor("yellow");
    }
    virtual void b()const{
        
    }
    virtual ~Hotel()
    {
        cout << "Destroying the hotel " << name() << "." << endl;
    }
    
};
class Restaurant: public Landmark
{
public:
    Restaurant(string name, int sC):Landmark(name)
    {
        seatCap = sC;
        if (seatCap < 40)
        {
            setIcon("small knife/fork");
        }
        else
        {
            setIcon("large knife/fork");
        }
        setColor("yellow");
    }
    virtual void b()const{
        
    }
    virtual~Restaurant()
    {
        cout << "Destroying the restaurant " << name() << "." << endl;
    }
    
    
private:
    int seatCap;
};
class Hospital: public Landmark
{
public:
    Hospital(string name):Landmark(name)
    {
        setIcon("H");
        setColor("blue");
    }
    virtual void b()const{
        
    }
    virtual~Hospital()
    {
        cout << "Destroying the hospital " << name() << "." << endl;
    }
    
};


void display(const Landmark* lm)
{
    cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
    << lm->name() << "." << endl;
}