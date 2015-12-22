#include "distancevector.h"
#include "context.h"

DistanceVector::DistanceVector(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

DistanceVector::DistanceVector(const DistanceVector & rhs) :
    Node(rhs)
{
    *this = rhs;
}

DistanceVector & DistanceVector::operator=(const DistanceVector & rhs) {
    Node::operator=(rhs);
    return *this;
}

DistanceVector::~DistanceVector() {}


/** Write the following functions.  They currently have dummy implementations **/
void DistanceVector::LinkHasBeenUpdated(Link* l) {
    cerr << *this << ": Link Update: " << *l << endl;

    int dst = l->GetDest();
    int latency = l->GetLatency();

    routing_table.neighborLinks[dst].cost = latency;
    routing_table.distanceVector[dst].cost = -1;
    routing_table.topo[dst][dst].cost = 0;
    //recalculate Distance Vector, if anything changed, broadcast
    //if anything updated, tell its neighbors
    if(updateDV())
        SendToNeighbors(new RoutingMessage(number, routing_table.distanceVector));
}

void DistanceVector::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
    //update distancevector form that src 
    routing_table.topo[m->src] = m->distVctr;
    //initialize what hasn't been initialized 
    for(Mint2TL_Iter mess_iter = m->distVctr.begin(); mess_iter != m->distVctr.end(); mess_iter++)
    {
        if(routing_table.distanceVector[mess_iter->first].cost == -1)
        {

        }
    }
    //recalculate cost and send to its neighbors
    if(updateDV())
         SendToNeighbors(new RoutingMessage(number, routing_table.distanceVector));
}

void DistanceVector::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

bool DistanceVector::updateDV()
{
    bool update = false;
    
    for(Mint2TL_Iter dv_iter = routing_table.distanceVector.begin(); dv_iter != routing_table.distanceVector.end(); dv_iter++)
    {
        int curr_node = dv_iter->first;
        //itself, should be 0
        if(curr_node == number)
        {
            routing_table.distanceVector[curr_node].cost = 0;
            continue;
        }

        int currMin = 2147483647;
        //int new_cost = 0;
        int nextHop = -1;
        //find shortest distance and update the position
        for(Mint2TL_Iter neighbor_iter = routing_table.neighborLinks.begin(); neighbor_iter != routing_table.neighborLinks.end(); neighbor_iter++)
        {
            int curr_neighbor = neighbor_iter->first;
            if(routing_table.neighborLinks[curr_neighbor].cost != -1 && routing_table.topo[curr_neighbor][curr_node].cost != -1)
            {
                int new_cost = routing_table.neighborLinks[curr_neighbor].cost + routing_table.topo[curr_neighbor][curr_node].cost;
                if(new_cost < currMin)
                {
                    currMin = new_cost;
                    nextHop = curr_neighbor;
                }
            }
        }
        //if changed, update
        if(currMin != 2147483647 && currMin != routing_table.distanceVector[curr_node].cost)
        {
            routing_table.distanceVector[curr_node].cost = currMin;
            routing_table.hops[curr_node] = nextHop;
            update = true;
        }
    }
    return update;
}

Node* DistanceVector::GetNextHop(Node *destination) { 
    //find matching nodes in its neighbors and return what we found 
    Node *node = new Node(routing_table.hops[destination->GetNumber()], NULL, 0, 0);
    node = context->FindMatchingNode(const_cast<Node *>(node));
    return node;
}

Table* DistanceVector::GetRoutingTable() {
    return new Table(routing_table);
}

ostream & DistanceVector::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
