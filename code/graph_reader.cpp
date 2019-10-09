#include "common.h"

// int g_adj_mat[n][n] = {0};
// // int g_dash_mat[m][m] = {0};
// int **g_adj_mat;
// int **g_dash_adj_mat;

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

EXPRESSION edge_clause(EXPRESSION e,int g_dash_max,GRAPH g, GRAPH g_dash, GRAPH g_false_edges, GRAPH g_dash_false_edges){
  for (int i = 0; i< g.size(); i++){
    EDGE c1_e = g[i];
    for (int j = 0; j < g_dash_false_edges.size(); j++){
      EDGE c2_e = g_dash_false_edges[j];
      // cout << get<0>(c2_e)<<" "<<get<1>(c2_e)<<" ";
      CLAUSE c = {-1 * create_var_number(get<0>(c1_e),get<0>(c2_e),g_dash_max),-1 * create_var_number(get<1>(c1_e),get<1>(c2_e),g_dash_max)};
      e.push_back(c);
    }
  }
  for (int j = 0; j<g_dash.size();j++){
    EDGE c1_e = g_dash[j];
    for (int j = 0; j < g_false_edges.size(); j++){
      EDGE c2_e = g_false_edges[j];
      // cout <<
      // cout << get<0>(c2_e)<<" "<<get<1>(c2_e)<<endl;
      CLAUSE c = {-1 * create_var_number(get<0>(c2_e),get<0>(c1_e),g_dash_max),-1 * create_var_number(get<1>(c2_e),get<1>(c1_e),g_dash_max)};
      e.push_back(c);
    }
  }
  return e;
}

EXPRESSION total_function_clause(EXPRESSION e, int g_dash_max, int g_max){
  for (int i = 1; i<=g_max; i++){
    CLAUSE c;
    for (int j = 1; j<=g_dash_max; j++){
      int a = create_var_number(i,j,g_dash_max);
      c.push_back(a);
    }
    e.push_back(c);
  }
  return e;
}

void print_matrix(vector<vector<int> > g_dash_adj_mat){
  for (int i = 0; i< g_dash_adj_mat.size();i++){
    for (int j = 0; j< g_dash_adj_mat.size();j++){
      cout<< g_dash_adj_mat[i][j]<<" ";
    }
    cout<<endl;
  }
}

int main(int argc, char const *argv[]) {

  string s1,s2,input_file,output_file;
  bool GRAPH_FLAG=true;
  GRAPH g,g_dash; //true edges of G and G'
  GRAPH false_g, false_g_dash; //false edges of G and G'
  int g_dash_max,g_max; //Max node value in each of the graph.
  int num_clauses,num_variables;
  GRAPH g_false_edges;
  GRAPH g_dash_false_edges;
  vector<vector<int> > g_adj_mat,g_dash_adj_mat;


  input_file = argv[1];
  output_file = argv[2];
  cout<<output_file<<endl;

  ifstream file(input_file);
  int max_node=0;

  while (file >> s1){
    file >> s2;
    if (s1 == "0" && s2 == "0"){
        for (int i = 1; i < g_dash_adj_mat.size(); i++){
          for (int j = 1; j < g_dash_adj_mat.size(); j++){
            if ((i != j) && (g_dash_adj_mat[i][j] == 0)){
              g_dash_false_edges.push_back(make_tuple(i, j));
            }
          }
        }
        g_dash_adj_mat.clear();
        g_dash_max = max_node;
        GRAPH_FLAG=false;
        max_node = 0;
        continue;
    }
    max_node = max(stoi(s1),max_node);
    max_node = max(stoi(s2),max_node);
    if (GRAPH_FLAG){
      g_dash.push_back(make_tuple(stoi(s1),(stoi(s2)))); //g' > g
      if (g_dash_adj_mat.size()<=max_node){
        for(int i = 0; i<=max_node;i++){
          if (i<g_dash_adj_mat.size()){
            for (int j = g_dash_adj_mat.size();j<=max_node;j++){
              g_dash_adj_mat[i].push_back(0);
            }
          }else{
            vector<vector<int> > l(max_node+1 - g_dash_adj_mat.size(), vector<int>(max_node+1));
            g_dash_adj_mat.insert(g_dash_adj_mat.end(),l.begin(),l.end());
          }
        }
      }
      g_dash_adj_mat[stoi(s1)][stoi(s2)] = 1;
    }else{
      g.push_back(make_tuple(stoi(s1),(stoi(s2))));
      if (g_adj_mat.size()<=max_node){
        for(int i =0; i<=max_node;i++){
          if (i<g_adj_mat.size()){
            for (int j = g_adj_mat.size();j<=max_node;j++){
              g_adj_mat[i].push_back(0);
            }
          }else{
            vector<vector<int> > l(max_node+1 - g_adj_mat.size(), vector<int>(max_node+1));
            g_adj_mat.insert(g_adj_mat.end(),l.begin(),l.end());
          }
        }
      }
      g_adj_mat[stoi(s1)][stoi(s2)] = 1;
    }
  }
  for (int i = 1; i < g_adj_mat.size(); i++){
    for (int j = 1; j < g_adj_mat.size(); j++){
      if ((i != j) && (g_adj_mat[i][j] == 0)){
        g_false_edges.push_back(make_tuple(i, j));
      }
    }
  }
  print_matrix(g_adj_mat);
  g_adj_mat.clear();
  // print_matrix(g_dash_adj_mat);
  // cout<<"-x-x-x-x-x-x-"<<endl;


  g_max = max_node;
  ofstream infofile;
  infofile.open("info");
  infofile << g_max<<" "<<g_dash_max<<"\n";
  // print_graph(g);
  // cout<<"Graph G Max node is: "<<g_max<<endl;
  // print_graph(g_dash);
  // cout<<"Graph G' Max node is: "<<g_dash_max<<endl;
  EXPRESSION one2one_clauses = one_to_one_clause(g_dash_max,g_max);
  one2one_clauses = total_function_clause(one2one_clauses,g_dash_max,g_max);
  one2one_clauses  = edge_clause(one2one_clauses,g_dash_max,g,g_dash, g_false_edges, g_dash_false_edges);
  num_clauses = one2one_clauses.size();
  num_variables = g_max * g_dash_max;
  write_clause_in_file(output_file,one2one_clauses,num_clauses,num_variables);

  return 0;
}
