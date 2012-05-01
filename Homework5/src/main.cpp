/*
 * main.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 04/02/2012
 */
 
#include <iostream>
#include "homework5.h"

int main(int argc, char *argv[])
{
    Homework5Application *app = new Homework5Application(argc, argv);
    app->run();
    
    delete app;
    return 0;
}
