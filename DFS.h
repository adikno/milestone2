//
// Created by michal on 1/9/19.
//

#ifndef UNTITLED14_DFS_H
#define UNTITLED14_DFS_H


#include "Searcher.h"

template <class T>
class DFS : public Searcher<T> {
private:
    int evaluated;
    double pathCost;

public:
    DFS(){
        evaluated =0;
    }
    vector<State<T>*> search(Searchable<T>* searchable) override {
        searchable->setCurrVisited();
        vector<State<T>*> trace;
        this->helpSearch(searchable, searchable->getInitialState(), trace);
        return trace;
    }

    int helpSearch(Searchable<T>* searchable, State<T>* curr, vector<State<T>*> &trace) {
        if(curr->equals(searchable->getGoalState())){
            evaluated++;
            while (curr->getParent() != nullptr) {
                trace.push_back(curr);
                pathCost += curr->getCost();
                curr = curr->getParent();
            }
            pathCost += curr->getCost();
            trace.push_back(curr);
            vector<State<T>*> back;
            for (int i = trace.size() - 1; i >= 0 ; i--) {
                back.push_back(trace.at(i));
            }
            trace = back;
            return 1;
        }
        curr->setVisited();
        evaluated++;
        searchable->setCurr(curr);
        list<State<T>*> succerssors = searchable->getAllPossibleStates(curr,'b');
        for (State<T>* state : succerssors) {
            bool visited = state->getVisited();
            if (!visited) {
                state->setParent(curr);
                if (helpSearch(searchable, state, trace) == 1) {
                    return 1;
                }
            }
        }
    }

    int getNumberOfNodesEvaluated() override {
        return evaluated;
    }

    double getPathCost() {
        return pathCost;
    }
};





#endif //UNTITLED14_DFS_H
