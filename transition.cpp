#include "transition.h"
#include <QtGlobal>
#include <QElapsedTimer>

//transition
Transition::Transition()
{
    transitionTimer = nullptr;
}
Transition::~Transition()
{
    delete transitionTimer;
}
float Transition::GetRate(){
    return 0;
}
void Transition::Start(int duration)
{
    transitionDuration = duration;
    if(transitionTimer == nullptr) transitionTimer = new QElapsedTimer();
    transitionTimer->restart();
}
void Transition::Stop()
{
    delete transitionTimer;
    transitionTimer = nullptr;
}
float Transition::Elapsed(){
    if(transitionDuration == 0 || transitionTimer == nullptr) return 1;
    return qMin(transitionTimer->elapsed() / 1000.f, (float)transitionDuration) / transitionDuration;
}

//constant
float Constant::GetRate(){
    return 1;
}

float Linear::GetRate(){
    return Elapsed();
}
