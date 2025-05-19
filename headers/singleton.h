#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
public:
    Singleton(const Singleton& ) = delete;
    Singleton& operator = (const Singleton&) = delete;
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }
protected:
    Singleton() = default;
    ~Singleton() = default;
};

#endif // SINGLETON_H
