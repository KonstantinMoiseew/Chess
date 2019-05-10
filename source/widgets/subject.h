#pragma once
#include <vector>
class Observer;


class Subject
{
public:
    void registerObserver(Observer *o);
    void removeObserver(Observer *o);
    void NotifyObserver();
    Subject();

private:
    std::vector<Observer *>  *ArrayList;

};
