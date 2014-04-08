#ifndef APP_H
#define APP_H

class OgreTest;

class App {
public:
    bool init();
    void run();
#ifdef __APPLE__
    void render();
#endif
    
private:
    OgreTest *test_;
};

#endif
