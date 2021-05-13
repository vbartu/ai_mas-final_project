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
	MSG_TYPE_CHECK_AGAIN,
} msg_type_t;


typedef enum conflict_type_t {
	CONFLICT_TYPE_TWO_AGENTS,
	CONFLICT_TYPE_BOX_IN_THE_WAY,
} conflict_type_t;

typedef struct msg_info_conflict_t {
	conflict_type_t conflict_type;
	goal_type_t goal_type;
	//int sequence_number;
	vector<CAction> next_actions;
} msg_info_conflict_t;

typedef struct conflict_agents_resolved_t {
	int skip;
	vector<CAction> new_actions;
} conflict_agents_resolved_t;

typedef union msg_params_t {
	//CAction next_action;
	//msg_info_conflict_t conflict;
} msg_params_t;

typedef struct msg_t {
	msg_type_t type;
	int agent_id;
	CAction next_action;
	msg_info_conflict_t conflict;
	conflict_agents_resolved_t conflict_resolved;
} msg_t;

void send_msg_to_agent(int dst_agent_id, msg_t msg);
void broadcast_msg(msg_t msg);
bool get_msg(int agent_id, msg_t* msg);


#endif // __COMMUNICATION_H
