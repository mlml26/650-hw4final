#include "query_funcs.h"
#include <iomanip>

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg){
  work W(*C);
  stringstream ss;
  ss << "INSERT INTO PLAYER(TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES (" <<  team_id << ", " << jersey_num << ", " << W.quote(first_name) << ", " << W.quote(last_name) << ", " << mpg << ", " << ppg << ", " << rpg << ", " << apg << ", " << spg << ", " << bpg << ");";
  W.exec( ss.str() );
  W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses) {  work W(*C);
  //TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES
  /*
  string a = "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES (" + W.quote(name) + ", " + to_string(state_id) + ", " + to_string(color_id) + ", " + to_string(wins) + ", " + to_string(losses) +");";
  cout << a << endl;
*/
  stringstream ss;
  ss << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES (" << W.quote(name) << ", " << state_id << ", " << color_id << ", " << wins << ", " << losses << ");";
  /* Execute SQL query */
  W.exec( ss.str() );
  W.commit();
}


void add_state(connection *C, string name)
{
  work W(*C);
  string a = "INSERT INTO STATE(NAME) VALUES(" + W.quote(name) + ");";
  /* Execute SQL query */
  W.exec( a );
  W.commit();
}


void add_color(connection *C, string name)
{
  work W(*C);
  string a = "INSERT INTO COLOR(NAME) VALUES(" + W.quote(name) + ");";
  /* Execute SQL query */
  W.exec( a );
  W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
  work W(*C);
  /* Create SQL statement */
  stringstream ss;
  ss <<  "SELECT * FROM PLAYER ";
  int flag = 0;
  if(use_mpg) {
    ss <<"WHERE (MPG BETWEEN " << min_mpg << "AND " << max_mpg << ") ";
    flag = 1;
  }
  if(use_ppg) {
    if(flag == 0) {ss << "WHERE "; flag = 1;}
    else ss << "AND ";
    ss << "(PPG BETWEEN " << min_ppg << " AND " << max_ppg << ") ";
    //flag = 1;
  }
  if(use_rpg) {
    if(flag == 0) {ss << "WHERE "; flag = 1;}
    else ss << "AND ";
    ss << "(RPG BETWEEN " << min_rpg << " AND " << max_rpg << ") ";
    //flag = 1;
  }
  if(use_apg) {
    if(flag == 0) {ss << "WHERE ";flag = 1;}
    else ss << "AND ";
    ss << "(APG BETWEEN " << min_apg << " AND " << max_apg << ") ";
    flag = 1;
  }
  if(use_spg) {
    if(flag == 0) {ss << "WHERE "; flag = 1;}
    else ss << "AND ";
    ss << "(SPG BETWEEN " << min_spg << " AND " << max_spg << ") ";
    flag = 1;
  }
  if(use_bpg) {
    if(flag == 0) {ss << "WHERE "; flag = 1;}
    else ss << "AND ";
    ss << "(APG BETWEEN " << min_bpg << " AND " << max_bpg << ") ";
    flag = 1;
  }
  ss<< ";";
  string a = ss.str();
  W.commit();	
	/* Create a non-transactional object. */
  nontransaction N(*C);
	/* Execute SQL query */
  result R( N.exec( a ));
	//list down all the record
  for(auto it = R.begin(); it != R.end(); ++it) {
    if(it == R.begin()) {
      cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"<<endl;
    }
    cout << it[0].as<int>() << " " << it[1].as<int>() << " " << it[2].as<int>() << " "  << it[3].as<string>() << " "  << it[4].as<string>() << " " << it[5].as<int>() << " " << it[6].as<int>() << " " << it[7].as<int>() << " " << it[8].as<int>() << " " << fixed << setprecision(1) << it[9].as<double>() << " " << it[10].as<double>()<< endl;
  }
}


//quary2(): show the name of each team with the indicated uniform color
void query2(connection *C, string team_color) {
  work W(*C);
  /* Create SQL statement */
  // stringstream ss;
  string s = "SELECT TEAM.NAME FROM TEAM, COLOR WHERE COLOR.NAME = " + W.quote(team_color) + " AND TEAM.COLOR_ID = COLOR.COLOR_ID;"; 
  W.commit();
  /* Create a non-transactional object. */
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec(s));
  //list down all the record
  for(auto it = R.begin(); it != R.end(); ++it) {
    if(it == R.begin()) {
      cout << "NAME"<<endl;
    }
    cout << it[0].as<string>() << endl;
  }
}


void query3(connection *C, string team_name)
{
  work W(*C);
  /* Create SQL statement */
  string a = "SELECT FIRST_NAME, LAST_NAME FROM PLAYER, TEAM WHERE TEAM.NAME = " + W.quote(team_name) + " AND PLAYER.TEAM_ID = TEAM.TEAM_ID;"; 
  W.commit();
  /* Create a non-transactional object. */
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( a ));
  //list down all the record
  for(auto it = R.begin(); it != R.end(); ++it) {
    if(it == R.begin()) {
      cout << "FIRST_NAME LAST_NAME"<<endl;
    }
    cout << it[0].as<string>() << " " << it[1].as<string>() << endl;
  }
}


void query4(connection *C, string team_state, string team_color)
{
  work W(*C);
  /* Create SQL statement */
  string a = "SELECT FIRST_NAME, LAST_NAME, UNIFORM_NUM FROM PLAYER, STATE, COLOR, TEAM WHERE STATE.NAME = " + W.quote(team_state) + " AND PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.STATE_ID = STATE.STATE_ID AND COLOR.NAME = " + W.quote(team_color) + " AND TEAM.COLOR_ID = COLOR.COLOR_ID;"; 
  W.commit();	
  /* Create a non-transactional object. */
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( a ));
  //list down all the record
  for(auto it = R.begin(); it != R.end(); ++it) {
    if(it == R.begin()) {
      cout << "UNIFORM_NUM FIRST_NAME LAST_NAME"<<endl;
    }
    cout << it[2].as<string>() << " " << it[1].as<string>() << " " << it[0].as<int>() << endl;
  }
}

void query5(connection *C, int num_wins)
{
  work W(*C);
  string wins = to_string(num_wins);
  /* Create SQL statement */
  string a = "SELECT FIRST_NAME, LAST_NAME, TEAM.NAME, WINS FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND WINS > " + wins + ";";
  W.commit();
  /* Create a non-transactional object. */
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( a ));
  //list down all the record                                                                          
  for(auto it = R.begin(); it != R.end(); ++it) {
    if(it == R.begin()) {
      cout << "FIRST_NAME LAST_NAME NAME WINS"<<endl;
    }
    cout << it[0].as<string>() << " " << it[1].as<string>() << " " << it[2].as<string>() << " " << it[3].as<int>() << endl;
  }
}
