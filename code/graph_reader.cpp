#include "common.h"


void print_graph(GRAPH g){
  cout<<"----Printing Graph----"<<endl;
  for (int i = 0;i<g.size();i++){
    cout<< get<0>(g[i])<<" "<<get<1>(g[i])<<endl;
  }
  cout<<"----------------------"<<endl;
}

void write_clause_in_file(string filename,EXPRESSION all_clauses, int num_clauses, int num_variables){
  ofstream outfile;
  outfile.open(filename);
  outfile << "p cnf "<<num_variables<<" "<<num_clauses<<'\n';
  for (int i = 0; i<num_clauses;i++){
    CLAUSE curr_clause = all_clauses[i];
    for (int j = 0; j<curr_clause.size();j++){
      outfile<<curr_clause[j]<<" ";
    }
    outfile<<"0\n";
  }
  outfile.close();
}


//Nij is mapping from i of G to j of G'
//Nij is written as X(i*m + j) where m is cardinality of G' and n is of G

int create_var_number(int i,int j, int g_dash_max){
  return ((i-1)*g_dash_max + j);
}

// EXPRESSION edge_clause(GRAPH g, GRAPH g_dash){
//
// }

EXPRESSION one_to_one_clause(int g_dash_max, int g_max){
  EXPRESSION one2one_clauses;
  for ( int i = 1; i<=g_max;i++){
    for (int j = 1; j<=g_dash_max;j++){

      int first_map = -1 *create_var_number(i,j,g_dash_max);

      if (i != g_max){  //-Nij V -Nkj
        for (int k =i+1; k<=g_max;k++){
          int second_map = -1 *create_var_number(k,j,g_dash_max);
          CLAUSE e = {first_map,second_map};
          one2one_clauses.push_back(e);
        }
      }

      if (j != g_dash_max){  // -Nij V -Nil
        for(int l = j+1;l<=g_dash_max;l++){
          int third_map = -1 * create_var_number(i,l,g_dash_max);
          CLAUSE f = {first_map,third_map};
          one2one_clauses.push_back(f);
        }
      }
    }
  }
  return one2one_clauses;
}


int main(int argc, char const *argv[]) {

  string s1,s2,input_file,output_file;
  bool GRAPH_FLAG=true;
  GRAPH g,g_dash; //true edges of G and G'
  GRAPH false_g, false_g_dash; //false edges of G and G'
  int g_dash_max,g_max; //Max node value in each of the graph.
  int num_clauses,num_variables;

  input_file = argv[1];
  output_file = argv[2];

  ifstream file(input_file);
  int max_node=0;

  while (file >> s1){
    file >> s2;
    if (s1 == "0" && s2 == "0"){
        g_dash_max = max_node;
        GRAPH_FLAG=false;
        max_node = 0;
        continue;
    }
    if (GRAPH_FLAG){
      g_dash.push_back(make_tuple(stoi(s1),(stoi(s2)))); //g' > g
    }else{
      g.push_back(make_tuple(stoi(s1),(stoi(s2))));
    }
    max_node = max(stoi(s1),max_node);
    max_node = max(stoi(s2),max_node);
  }
  g_max = max_node;
  ofstream infofile;
  infofile.open("info");
  infofile << g_max<<" "<<g_dash_max<<"\n";
  print_graph(g);
  cout<<"Graph G Max node is: "<<g_max<<endl;
  print_graph(g_dash);
  cout<<"Graph G' Max node is: "<<g_dash_max<<endl;
  EXPRESSION one2one_clauses = one_to_one_clause(g_dash_max,g_max);
  num_clauses = one2one_clauses.size();
  num_variables = g_max * g_dash_max;
  write_clause_in_file(output_file,one2one_clauses,num_clauses,num_variables);


  return 0;
}
