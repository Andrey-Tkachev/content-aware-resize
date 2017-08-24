//
// Created by nikita on 05.07.17.
//

#ifndef CONTENTAWARERESIZE_SINGLETON_H
#define CONTENTAWARERESIZE_SINGLETON_H

template<typename T>
class Singleton {
public:
    static T &Instance() {
        static T instance;
        return instance;
    }

protected:
    Singleton() {};

    ~Singleton() {};

public:
    Singleton(Singleton const &) = delete;

    Singleton &operator=(Singleton const &) = delete;

};

#endif //CONTENTAWARERESIZE_SINGLETON_H
