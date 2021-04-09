#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <bits/stdc++.h>

#include "action.h"

Action search(vector<Action> &initial_state,vector<vector<Action>> &frontier)
{
	cout<< "Search start" << endl;
    int iterations = 0;

    frontier.push_back(initial_state);
    set<vector<Action>> explored;

    // while(1)
    {
        // Return error is frontier is empty
        if(frontier.size() == 0)
            return 0;

        // Get next node to be explored from the frontier
        vector<Action> leaf_node = frontier.back();
        frontier.pop_back();
        iterations++;
        if(iterations % 1000 == 0)
        {
            // print_search_status(explored, frontier, leaf_node, True);
        }

        // If this state is a goal state, solition is found
        // Return the path (list of actions) followed to get to this state
        // if(leaf_node.is_goal_state())
        // {
        //     cout<< "goal state" << endl;
            // return leaf_node.extract_plan();
        // }

        // If not, add the node to the explored set, get its successors
        // and if they are not alreay explored or in the frontier,
        // add them to the frontier
        // explored.insert(leaf_node);
        // for(auto new_node: leaf_node.get_expanded_states())
        // {
        //     if(!count(frontier.begin(),frontier.end(),new_node) && !count(explored.begin(),explored.end(),new_node))
        //     {
        //         frontier.push_back(new_node);
        //     }
        // }
    }

    return true;
}

