#include "bdi_agent.h"
#include "action.h"


void BdiAgent::run()
{
	map<coordinates_t, char> believes;
	goal_t intention;
	
	while (true) {
		believes = communication.get_positions();
		intention = this->get_next_goal(believes);
		State* state = this->intention_to_state(intention, believes);
		vector<Action> plan = graphsearch(state);
		while (plan.size()) {
			Action next_action = plan.head();
			beleives = communication.get_positions();
			if (this->check_conflict(next_action)) {
				plan.pop_front();
				this->store_action(next_action);
			} else {
				// try to go around the conflict,
				// if impossible
				break;
			}
		}
	}
}
