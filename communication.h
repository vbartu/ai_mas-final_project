

typedef struct coordinates_t {
	int x;
	int y;
} coordinates_t;



class Communication {
	public:

		vector<map<coordinates_t, char>> level_map;

		Communication();


		void update_position(char agent_id, int row, int col);

		void update_box_postion(char box , int row, int col);

		map<coordinates_t, char> get_positions();

