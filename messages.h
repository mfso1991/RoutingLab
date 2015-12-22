#ifndef _messages
#define _messages

#include <iostream>
#include "node.h"
#include "link.h"

struct RoutingMessage {
    RoutingMessage();
    RoutingMessage(const RoutingMessage &rhs);
    RoutingMessage &operator=(const RoutingMessage &rhs);

    ostream & Print(ostream &os) const;

    // Anything else you need

    #if defined(LINKSTATE)
    RoutingMessage(int a, int s, int d, int c);
    int age;
    int src;
    int dst;
    int cost;
    #endif

    #if defined(DISTANCEVECTOR)
    RoutingMessage(int s, map < int , TopoLink >);
    int src;
    map < int, TopoLink> distVctr;
    #endif
};

inline ostream & operator<<(ostream &os, const RoutingMessage & m) { return m.Print(os);}

#endif
