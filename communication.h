#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "global.h"
#include "action.h"
#include "bdi_agent.h"

typedef enum msg_type_t {
	MSG_TYPE_NEXT_ACTION,
	MSG_TYPE_STEP_FINISHED,
	MSG_TYPE_CONFLICT_AGENTS,
	MSG_TYPE_CONFLICT_AGENTS_RESOLVED,
	MSG_TYPE_CONFLICT_BOX,
	MSG_TYPE_HELP_ACCEPTED_CONFLICT_BOX,
	MSG_TYPE_HELP_REJECTED_CONFLICT_BOX,
	MSG_TYPE_MOVING_BOX_FROM_PATH,
	MSG_TYPE_WAITING_FOR_YOUR_HELP,
	// MSG_TYPE_IN_CHARGE_REQUEST_HELP,
	// MSG_TYPE_CONFLICT_BOX_RESOLVED,
	MSG_TYPE_CHECK_AGAIN,
} msg_type_t;

typedef struct conflict_agents_t {
	vector<CAction> next_actions;
} conflict_agents_t;

typedef struct conflict_agents_resolved_t {
	int skip;
	vector<CAction> new_actions;
} conflict_agents_resolved_t;

typedef struct conflict_box_t {
	coordinates_t box_pos;
	vector<CAction> next_actions;
} conflict_box_t;

typedef struct help_conflict_box_t {
	coordinates_t adj_pos;
	coordinates_t box_pos;
	coordinates_t goal_box_pos;
} help_conflict_box_t;

typedef struct msg_t {
	msg_type_t type;
	int agent_id;
	int time;
	CAction next_action;
	conflict_agents_t conflict;
	conflict_agents_resolved_t conflict_resolved;
	conflict_box_t conflict_box;
	help_conflict_box_t help_conflict_box;
} msg_t;

void send_msg_to_agent(int time, int dst_agent_id, msg_t msg);
void send_msg_to_agents(int time, vector<int> dst_agent_ids, msg_t msg);
void broadcast_msg(int time, msg_t msg);
bool get_msg(int time, int agent_id, msg_t* msg);


#endif // __COMMUNICATION_H
