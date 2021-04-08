

// start_time = time.perf_counter()

// def search(initial_state, frontier):

//     output_fixed_solution = False

//         # Part 2:
//         # Now try to implement the Graph-Search algorithm from R&N figure 3.7
//         # In the case of "failure to find a solution" you should return None.
//         # Some useful methods on the state class which you will need to use are:
//         # state.is_goal_state() - Returns true if the state is a goal state.
//         # state.extract_plan() - Returns the list of actions used to reach this state.
//         # state.get_expanded_states() - Returns a list containing the states reachable from the current state.
//         # You should also take a look at frontier.py to see which methods the Frontier interface exposes

//         iterations = 0

//         frontier.add(initial_state)
//         explored = set()

//         while True:

//             # Return error is frontier is empty
//             if not frontier.size():
//                 return None

//             # Get next node to be explored from the frontier
//             leaf_node = frontier.pop()

//             iterations += 1
//             if iterations % 1000 == 0:
//                 print_search_status(explored, frontier, leaf_node, True)

//             if memory.get_usage() > memory.max_usage:
//                 print_bar()
//                 print('Maximum memory usage exceeded.', file=sys.stderr, flush=True)
//                 print_search_status(explored, frontier, leaf_node)
//                 return None



//             # If this state is a goal state, solition is found
//             # Return the path (list of actions) followed to get to this state
//             if leaf_node.is_goal_state():
//                 print_bar()
//                 print_search_status(explored, frontier, leaf_node)
//                 print(f"{leaf_node}", file=sys.stderr, flush=True)
//                 return leaf_node.extract_plan()

//             # If not, add the node to the explored set, get its successors
//             # and if they are not alreay explored or in the frontier,
//             # add them to the frontier
//             explored.add(leaf_node)
//             for new_node in leaf_node.get_expanded_states():
//                 if not frontier.contains(new_node) and new_node not in explored:
//                     frontier.add(new_node)


// def print_search_status(explored, frontier, state, same_line=False):
//     elapsed_time = time.perf_counter() - start_time
//     if isinstance(frontier, FrontierBestFirst):
//         print(f"E: {len(explored)}, "
//             + f"G: {len(explored)+frontier.size()}, "
//             + f"T: {elapsed_time:3.3f} s, "
//             + f"M: {memory.get_usage():.2f} MB, "
//             + f"D: {state.g}, "
//             + f"H: {frontier.last_f}",
//             end="\r" if same_line else "\n",
//             file=sys.stderr,
//             flush=True
//         )
//     else:
//         print(f"E: {len(explored)}, "
//             + f"G: {len(explored)+frontier.size()}, "
//             + f"T: {elapsed_time:3.3f} s, "
//             + f"M: {memory.get_usage():.2f} MB, "
//             + f"D: {state.g}",
//             end="\r" if same_line else "\n",
//             file=sys.stderr,
//             flush=True
//         )

// def print_bar():
//     print("=================================================",
//         file=sys.stderr,
//         flush=True
//     )
