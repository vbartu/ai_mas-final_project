#ifndef __BDI_AGENT_H
#define __BDI_AGENT_H


typedef struct goal_t {
	int type; // 0 is for going to a box, 1 for carrying that box to its goal, 2 for going to agent goal,
	int row;
	int col;
}

class BdiAgent {
	public:
		int agent_id;

		BdiAgent();

		void run();

}

#endif // __BDI_AGENT_H
