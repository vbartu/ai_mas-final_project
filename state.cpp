class State {
	public:
		static bool walls[][];
		static char goals[][];
		static int agent_colors[];
		static int box_colors[];

		int agents[][];
		char boxes[][];

		final State parent;
		//final Action[] joint_action;
		final int g; // depth

		State(int agents[][], char boxes[][], 
}
