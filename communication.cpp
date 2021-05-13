#include <queue>
#include <assert.h>

#include "communication.h"

static queue<msg_t> msg_queue[10];
static pthread_mutex_t msg_queue_mtx;

void send_msg_to_agent(int dst_agent_id, msg_t msg)
{
	assert(!pthread_mutex_lock(&msg_queue_mtx));
	msg_queue[dst_agent_id].push(msg);
	assert(!pthread_mutex_unlock(&msg_queue_mtx));
}

void broadcast_msg(msg_t msg)
{
	assert(!pthread_mutex_lock(&msg_queue_mtx));
	for (int i; i < n_agents; i++) {
		if (msg.agent_id == i)
			continue;
		msg_queue[i].push(msg);
	}
	assert(!pthread_mutex_unlock(&msg_queue_mtx));
}

bool get_msg(int agent_id, msg_t* msg)
{
	bool success = true;
	assert(!pthread_mutex_lock(&msg_queue_mtx));
	if (msg_queue[agent_id].empty()) {
		success = false;
	} else {
		*msg = msg_queue[agent_id].front();
		msg_queue[agent_id].pop();
	}
	assert(!pthread_mutex_unlock(&msg_queue_mtx));
	return success;
}
