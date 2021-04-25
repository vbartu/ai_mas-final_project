#include "bdi_agent.h"
#include "action.h"
#include "communication.h"

goal_t BdiAgent::get_next_goal(umap_t believes) {
	goal_t goal;
	int row_delta[4] = {-1, 0, 0, 1}; // to find the adjacent cells
  int col_delta[4] = {0, -1, 1, 0};

	// iterate for each goal
	umap_t:iterator it = this->goals_map.begin();
	while (it != this->goals_map.end())
	{
		int goal_row = it->first.x;
		int goal_col = it->first.y;
		char type_goal = it->second;
		coordinates_t goal_pos(goal_row, goal_col);

		// check if in the believes there is a box with the same key(same pos)
		umap_t::iterator box_it = believes.find(goal_pos);
		if (box_it != believes.end && box_it->second == type_goal)
		{
			// box in the correct position
			continue;
		}

		else
		{
			// get the agent position
			umap_t::iterator b_it = believes.begin();
			int ag_row = b_it->first.x;
			int ag_col = b_it->first.y;
			char ag_type = b_it->second;
			b_it++;
			// start from the first box
			for (b_it; m_it != believes.end(); b_it++)
			{
				if (b_it->second == type_goal
				{// I already know that the correspective box is not in the correct position
					// check if the agent is in one of the four adjacent cell
					bool agent_adj = false;
					for (int i = 0; i < 4; i++)
					{
						int row_adj = ag_row + row_delta[i];
						int col_adj = ag_col + col_delta[i];
						// check if it is a wall ? maybe not necessary
						if (b_it->first.x == row_adj && b_it->first.y == col_adj)
						{
							// agent adjacent to the box
							goal.type = 1;
							goal.x = ..;
							goal.y = ..;
							agent_adj = true;
							break;
						}
					}
					if (!agent_adj)
					{
						// the next goal is to bring the agent to the box
						goal.type = 0;
						goal.x = b_it->first.x;
						goal.y = b_it->second.y;
					}
				}
			}
			if (goal.tyep == -1)
			{ // no box left
				// check if ther is an agent goal
				umap_t::iterator ag_it = believes.find(goal_pos);
				for (umap_t::iterator ag_it = goals_map.begin(); ag_it < goals_map.end(); ag_it++)
				{
					if (ag_it->second == ag_type && ag_it->first.x != ag_row && ag_it->first.y != first.y)
					{
						goal.type = 0;
						goal.x = ag_it->first.x;
						goal.y = ag_it->first.y;
					}
				}
			}
		}
	}
}


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
			believes = communication.get_positions();
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
