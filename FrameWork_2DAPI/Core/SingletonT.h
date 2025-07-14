#pragma once
template <typename T>
class SingletonT
{
protected:
    SingletonT() = default;
    virtual ~SingletonT() = default;

    SingletonT(const SingletonT&) = delete;
    SingletonT& operator=(const SingletonT&) = delete;

private:
    static T* instance;

public:
    static T* GetI()
    {
        if (!instance)
        {
            instance = new T();
            static struct Deleter {
                ~Deleter() { delete instance; instance = nullptr; }
            } deleter;
        }
        return instance;
    }
    static T& Instance()
    {
        if (!instance)
        {
            instance = new T();
            static struct Deleter {
                ~Deleter() { delete instance; instance = nullptr; }
            } deleter;
        }
        return *instance;
    }

	virtual void DestroyManager()
	{
        if (instance) {
            delete instance;
            instance = nullptr;
        }
	}

};

template <typename T>
T* SingletonT<T>::instance = nullptr;
