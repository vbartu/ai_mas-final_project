

void Communication::update_box_position(char box, int row, int col)
{
	// TODO: thread lock
	coordinates_t coord = {row, col};
	map[coord] = box;
}


void Communication::update_position(char agent_id, int row, int col)
{
	// TODO: thread lock
	coordinates_t coord = {row, col};
	map[coord] = agent_id;
}


map<coordinates_t, char> Communication::get_positions()
{
	return level_map;
}

void agent() {
	map = get_positions();
	

