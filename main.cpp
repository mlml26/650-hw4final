#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include "exerciser.h"

using namespace std;
using namespace pqxx;
void execute(connection *C, string a) {
  /* Create a transactional object. */
  work W(*C);
  /* Execute SQL query */
  W.exec( a );
  W.commit();
}
void drop(connection *C) {
    string a = "drop table if exists PLAYER CASCADE;";
    execute(C, a);
    string b = "drop table if exists TEAM CASCADE;";
    execute(C, b);
    string c = "drop table if exists STATE CASCADE;";
    execute(C, c);
    string d = "drop table if exists COLOR CASCADE;";
    execute(C, d);
}
void create(connection *C, string File) {
  string lines;
  string s;
  ifstream ifs(File.c_str());
  if(ifs.fail()) {
    cerr << "File to open SQL file." << endl;
    exit(EXIT_FAILURE);
  }
  while(getline(ifs, s)){
    lines += s;
  }
  ifs.close();
  execute(C, lines);
}
void addColor(connection * C, string File) {
  string s, name;
  int color_id;
  ifstream ifs(File.c_str());
  if(ifs.fail()) {
    cerr << "File to open SQL file." << endl;
    exit(EXIT_FAILURE);
  }
  while(getline(ifs, s)){
    stringstream cin(s);
    cin >> color_id;
    cin >> name;
    add_color(C, name);
  }
  ifs.close();
}

void addState(connection * C, string File) {
  string s, name;
  int state_id;
  ifstream ifs(File.c_str());
  if(ifs.fail()) {
    cerr << "File to open SQL file." << endl;
    exit(EXIT_FAILURE);
  }
  while(getline(ifs, s)){
    stringstream cin(s);
    cin >> state_id;
    cin >> name;
    add_state(C, name);
  }
  ifs.close();
}

void addTeam(connection * C, string File) {
  string s, name;//TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES
  int team_id, state_id, color_id, wins, losses;
  ifstream ifs(File.c_str());
  if(ifs.fail()) {
    cerr << "File to open SQL file." << endl;
    exit(EXIT_FAILURE);
  }
  while(getline(ifs, s)){
    stringstream cin(s);
    cin >> team_id;//primary key
    cin >> name;
    cin >> state_id;
    cin >> color_id;
    cin >> wins;
    cin >> losses;
    //cout << name << ", " <<state_id << ", " << color_id << ", " << wins << ", " << losses << endl;
    add_team(C, name, state_id, color_id, wins, losses);
  }
  ifs.close();
}

void addPlayer(connection * C, string File) {
  string s, first_name, last_name;//TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES
  int player_id, team_id, jersey_num, mpg, ppg, rpg, apg;
  double spg, bpg;
  ifstream ifs(File.c_str());
  if(ifs.fail()) {
    cerr << "File to open SQL file." << endl;
    exit(EXIT_FAILURE);
  }
  while(getline(ifs, s)){
    stringstream cin(s);
    cin >> player_id;//primary key
    cin >> team_id;
    cin >> jersey_num;
    cin >> first_name;
    cin >> last_name;
    cin >> mpg;
    cin >> ppg;
    cin >> rpg;
    cin >> apg;	
    cin >> spg;
    cin >> bpg;	
    //    cout << "spg="<< spg << endl;
    //cout << "bpg="<< bpg << endl;
    add_player(C, team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
  }
  ifs.close();
}

int main (int argc, char *argv[]) 
{
  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      //cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }
  //1. drop(if needed)4 table
  drop(C);
  create(C,"createOne.sql");
  addColor(C,"color.txt");
  addState(C, "state.txt");
  addTeam(C, "team.txt");
  addPlayer(C, "player.txt");
  
  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  exercise(C);
  //Close database connection
  C->disconnect();

  return 0;
}


