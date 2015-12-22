#include "linkstate.h"
#include "context.h"
#include <set>

LinkState::LinkState(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

LinkState::LinkState(const LinkState & rhs) :
    Node(rhs)
{
    *this = rhs;
}

LinkState & LinkState::operator=(const LinkState & rhs) {
    Node::operator=(rhs);
    return *this;
}

LinkState::~LinkState() {}


/** Write the following functions.  They currently have dummy implementations **/
void LinkState::LinkHasBeenUpdated(Link* l) {
    cerr << *this << ": Link Update: " << *l << endl;
    int src = l->GetSrc();
    int dst = l->GetDest();
    int cost = l->GetLatency();
    routing_table.topo[src][dst].cost = cost;
    int age = routing_table.topo[src][dst].age++;
    SendToNeighbors(new RoutingMessage(age, src, dst, cost));
}

void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
    int age = m->age;
    int src = m->src;
    int dst = m->dst;
    int cost = m->cost; 
    if(routing_table.topo[src][dst].cost == -1 || routing_table.topo[src][dst].age < age)
    {
        routing_table.topo[src][dst].age = age;
        routing_table.topo[src][dst].cost = cost;
        routing_table.linkUpdated = true; 
        SendToNeighbors(m);
    }
}

void LinkState::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node* LinkState::GetNextHop(Node *destination) { 
    if(routing_table.linkUpdated) 
    {
        Int2Int dist, previous;
        TopoMatrix topo = routing_table.topo;
        TopoMatrix :: size_type size = topo.size();
        for(TopoMatrix :: size_type i=0; i<size; ++i) 
        {
            dist[i] = 2147483647;
            previous[i] = -1;
        }
        
        set < int > checked, queue;
        Mint2TL neighbors;
        dist[number] = 0;
        queue.insert(number);
        while(!queue.empty()) 
        {
            int min = 2147483647;
            int smallest = -1;  
            for(TopoMatrix :: size_type i = 0; i < size; ++i){
                if(queue.count(i) > 0 && checked.count(i) < 1 && dist[i] < min)
                {
                    min = dist[i];
                    smallest = i;
                }
            }
            queue.erase(smallest);
            checked.insert(smallest); 
            neighbors = topo[smallest];
            for(Mint2TL_Iter iter=neighbors.begin(); iter!=neighbors.end(); ++iter)
            {
                if(checked.count(iter->first) >= 1) 
                    continue;
                int v = iter->first;
                int costNow = dist[v], costThen = dist[smallest] + topo[smallest][v].cost;
                if(costThen < costNow)
                {
                    dist[v] = costThen;
                    previous[v] = smallest;
                    if(checked.count(v) < 1)
                        queue.insert(v);
                }
            }
        }
        Int2Int hop_temp;
        for(Int2IntIter iter = previous.begin(); iter != previous.end(); iter++) 
        {
            int before = iter->second, current = iter->first, currentDump = current;
            while(before != number) 
            {
                current = before;
                before = previous[before];
            }
            hop_temp[currentDump] = current;
        }
        routing_table.hops = hop_temp;
        routing_table.linkUpdated = false;
    }
    Node *node = new Node(routing_table.hops[destination->GetNumber()], NULL, 0, 0);
    node = context->FindMatchingNode(const_cast<Node *>(node));
    return node;
}

Table* LinkState::GetRoutingTable() {
    return new Table(routing_table);
}

ostream & LinkState::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
