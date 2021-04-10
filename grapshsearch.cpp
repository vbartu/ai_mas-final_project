#include <cstdio>
#include <iostream>
#include <chrono>
#include <vector>

#include "state.h"
#include "frontier.h"
#include "graphsearch.h"

using namespace std;

auto start_time = chrono::system_clock::now();

    void print_search_status(vector<State> explored, Frontier &frontier, State state, bool same_line = false) {
      auto end = chrono::system_clock::now();
      //long elapsed_time = start_time - end;

      //if (isinstance(frontier, FrontierBestFirst)) {
      //  cout << "E: " << explored.size();
      //  cout << "G: " << explored.size() + frontier.size();
      //  cout << "T: " << elapsed_time << "s";
      //  // cout << "M: " << memory.get_usage;
      //  cout << "D: " << state.Mg;
      //  cout << "H: " << frontier.last_f;
      //  if (same_line) {
      //    char end = '\r';
      //  }
      //  else {
      //    char end = '\n';
      //  }
      //  // file = sys.stderr;
      //  // flush=true;
      //}

      //else {
        cout << "E: " << explored.size();
        cout << "G: " << explored.size() + frontier.size();
        //out << "T: " << elapsed_time << "s";
        // cout << "M: " << memory.get_usage;
        cout << "D: " << state.Mg;
        if (same_line) {
          char end = '\r';
        }
        else {
          char end = '\n';
        }
        // file = sys.stderr;
        // flush=true;
      }
	//}


    void print_bar() {
      cout << "=================================================" << endl;
          // file=sys.stderr,
          // flush=True
    }


    vector<vector<Action>> search(State initial_state, Frontier &frontier) {
        bool output_fixed_solution = false;
        if (output_fixed_solution) {
          //return Action[] = {MoveS, MoveN, MoveN};
		  vector<vector<Action>> action_vector;
		  return action_vector;
        }

        else {
          int iterations = 0;

          frontier.add(initial_state);
          vector<State> explored;

          while (true) {

            // Return error is frontier is empty
            if(frontier.size() == 0) {
				vector<vector<Action>> action_vector;
				return action_vector;
            }

            // Get next node to be explored from the frontier
            State leaf_node = frontier.pop();

            iterations += 1;
            //Print a status message every 10000 iteration
            if (iterations % 10000 == 0) {
              // print_search_status(explored, frontier, state, same_line=False):
            }

            // if memory.

            // If this state is a goal state, solition is found
            // Return the path (list of actions) followed to get to this state
            if (leaf_node.is_goal_state()) {
              print_bar();
              //print_search_status();
              return leaf_node.extract_plan();
            }

            // If not, add the node to the explored set, get its successors
            // and if they are not alreay explored or in the frontier,
            // add them to the frontier
            bool found = false;
            for(int i = 0; i< explored.size(); i++){
              if (explored[i] == leaf_node) {
                found = true;
                break;
              }
            if(!found) {
              explored.push_back(leaf_node);
            }

            vector<State> new_nodes = leaf_node.get_expanded_states();
            for (State node: new_nodes) {
              bool found = false;
              for(int i = 0; i < explored.size(); i++){
                if (explored[i] == node) {
                  found = true;
                  break;
                }
              }
              if (!frontier.contains(node) &&  !found) {
                frontier.add(node);
              }
            }
          }
        }
      }
    }
