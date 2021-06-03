#ifndef TRANSITION_H
#define TRANSITION_H

#include <QElapsedTimer>

class Transition
{
public:
    Transition();
    ~Transition();
    virtual float GetRate();
    void Start(int duration);
    void Stop();
    float Elapsed();
protected:
    int transitionDuration;
    QElapsedTimer* transitionTimer;
};

class Constant : public Transition{
public:
    virtual float GetRate() override;
};

class Linear : public Transition{
public:
    virtual float GetRate() override;
};

#endif // TRANSITION_H
