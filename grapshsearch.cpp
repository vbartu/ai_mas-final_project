#include <cstdio>
#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_set>
#include "fake_set.h"

#include "state.h"
#include "frontier.h"
#include "graphsearch.h"

using namespace std;

class HashHelper {
public:
	int operator()(const State* state) const {
		return state->hashCode();
	}
};

vector<vector<Action>> search(State* initial_state, Frontier &frontier) {
	int iterations = 0;
	frontier.add(initial_state);
	//unordered_set<State*, HashHelper> explored;
	FakeSet explored;

	while (true) {
		// TODO: Return error is frontier is empty
		if(frontier.size() == 0) {
			vector<vector<Action>> action_vector;
			return action_vector;
		}

		// Get next node to be explored from the frontier
		State* state = frontier.pop();
		//cerr << "New node: " << state << endl;
		//cerr << state->repr();

		iterations += 1;
		if (iterations % 100 == 0) {
			cout << "#" << "Explored: " << explored.size() << endl;
			cout << "#" << "Frontier: " << frontier.size() << endl;
		}


		// If this state is a goal state, solition is found
		// Return the path (list of actions) followed to get to this state
		if (state->is_goal_state()) {
			cerr << "Finished!" << endl;
			return state->extract_plan();
		}


		// If not, add the node to the explored set, get its successors
		// and if they are not alreay explored or in the frontier,
		// add them to the frontier
		explored.insert(state);
		vector<State*> expanded_states = state->get_expanded_states();

		for (State* new_state: expanded_states) {
			if (!frontier.contains(new_state) && !explored.count(new_state)) {
				frontier.add(new_state);
			}
		}
	}
}
