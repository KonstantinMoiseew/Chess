#include "subject.h"
#include <algorithm>
Subject::Subject()
{
    ArrayList = new std::vector<Observer *>[32];
}

void Subject::registerObserver(Observer * o)
{
        ArrayList->push_back(o);
}

void Subject::removeObserver(Observer *o)
{

}

void Subject::NotifyObserver()
{


    for(auto it=ArrayList->begin(); it!=ArrayList->end(); ++it)
    {

    }

}
