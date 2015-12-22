#ifndef _table
#define _table

#include <iostream>
#include <map>

using namespace std;

struct TopoLink {
    TopoLink(): cost(-1), age(0) {}

    TopoLink(const TopoLink & rhs) {
        *this = rhs;
    }

    TopoLink & operator=(const TopoLink & rhs) {
        this->cost = rhs.cost;
        this->age = rhs.age;
        return *this;
    }

    int cost;
    int age;
};

// Students should write this class
class Table {
    public:
        Table();
        Table(const Table &);
        Table & operator=(const Table &);

        ostream & Print(ostream &os) const;
        map < int, map < int, TopoLink > > topo;
        map < int, int > hops;
        // Anything else you need

        #if defined(LINKSTATE)
        bool linkUpdated;
        #endif

        #if defined(DISTANCEVECTOR)
        //map < int, int > hops;
        map < int , TopoLink> neighborLinks;
        map < int , TopoLink> distanceVector;
        #endif
};

typedef map < int, map < int, TopoLink > > TopoMatrix;
typedef TopoMatrix :: const_iterator topoIter;
typedef map < int, int > Int2Int;
typedef Int2Int :: const_iterator Int2IntIter;
typedef map < int, TopoLink > Mint2TL;
typedef Mint2TL :: const_iterator Mint2TL_Iter;

inline ostream & operator<<(ostream &os, const Table & t) { return t.Print(os);}
#endif
