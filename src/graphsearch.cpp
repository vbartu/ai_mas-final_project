#include <cstdio>
#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_set>
#include <limits.h>

#include "graphsearch.h"
#include "frontier.h"

using namespace std;

vector<CAction> search(AgentState* initial_state, int limit) {
	if (limit < 0) limit = INT_MAX;
	int iterations = 0;
	FrontierBestFS frontier;
	frontier.add(initial_state);
	unordered_set<AgentState*, HashHelper, EqualHelper> explored;

	while (true) {
		if(frontier.size() == 0 || iterations > limit) {
			vector<CAction> action_vector;
			cerr << "Frontier empty error" << endl;
			return action_vector;
		}

		// Get next node to be explored from the frontier
		AgentState* state = frontier.pop();

		iterations += 1;
		if (iterations % 10000 == 0) {
			fprintf(stderr, "Explored: %d, Frontier: %d\n", explored.size(), frontier.size());
			cerr << state->repr();
		}


		// If this state is a goal state, solition is found
		// Return the path (list of actions) followed to get to this state
		if (state->is_goal_state()) {
			vector<CAction> plan = state->extract_plan();
			return plan;
		
		}


		// If not, add the node to the explored set, get its successors
		// and if they are not alreay explored or in the frontier,
		// add them to the frontier
		explored.insert(state);
		vector<AgentState*> expanded_states = state->get_expanded_states();

		for (AgentState* new_state : expanded_states) {
			if (!frontier.contains(new_state) && !explored.count(new_state)) {
				frontier.add(new_state);
			}
		}
	}
}

vector<vector<CAction>> conflict_search(ConflictState* initial_state) {
	int iterations = 0;
	ConflictFrontierBestFS frontier;
	frontier.add(initial_state);
	unordered_set<ConflictState*, ConflictHashHelper, ConflictEqualHelper> explored;

	while (true) {
		if(frontier.size() == 0) {
			vector<vector<CAction>> action_vector;
			cerr << "Frontier empty error" << endl;
			return action_vector;
		}

		// Get next node to be explored from the frontier
		ConflictState* state = frontier.pop();

		iterations += 1;
		if (iterations % 10000 == 0) {
			// fprintf(stderr, "Explored: %d, Frontier: %d\n", explored.size(), frontier.size());
		}


		// If this state is a goal state, solition is found
		// Return the path (list of actions) followed to get to this state
		if (state->is_goal_state()) {
			vector<vector<CAction>> plan = state->extract_plan();
			return plan;
		}


		// If not, add the node to the explored set, get its successors
		// and if they are not alreay explored or in the frontier,
		// add them to the frontier
		explored.insert(state);
		vector<ConflictState*> expanded_states = state->get_expanded_states();

		for (ConflictState* new_state : expanded_states) {
			if (!frontier.contains(new_state) && !explored.count(new_state)) {
				frontier.add(new_state);
			}
		}
	}
}
