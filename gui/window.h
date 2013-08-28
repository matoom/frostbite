#ifndef WINDOW_H
#define WINDOW_H

class Window {

public:
    virtual ~Window() {}
    virtual void setAppend(bool) = 0;
    virtual bool append() = 0;


};
#endif // WINDOW_H
