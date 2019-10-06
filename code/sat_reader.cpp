#include "common.h"


int main(int argc, char const *argv[]){
  string input_file,output_file,sat_check,int_data;
  GRAPH v;
  int g_max,g_dash_max;
  input_file = argv[1];
  output_file = argv[2];
  ifstream file(input_file);
  ifstream file2("info");
  ofstream outfile;

  file2 >> int_data;
  g_max = stoi(int_data);
  file2 >>int_data;
  g_dash_max = stoi(int_data);
  file2.close();
  outfile.open(output_file);
  file >> sat_check;
  if (sat_check=="SAT"){
    file >> int_data;
    int ct = 1;
    while(int_data != "0"){
      int temp = stoi(int_data);
      int j = abs(temp%g_dash_max); //remove the abs function and add the temp>0 condition
      int i = abs(temp/g_dash_max)+1;
      i= (j==0)?i-1:i;
      j=(j==0)?g_dash_max:j;
      // if (temp>0){
          v.push_back(make_tuple(i,j));
      // }

      // v.push_back(temp);
      // ct++;
      file >> int_data;
    }
    for (int k =0; k< v.size(); k++){
      outfile <<get<0>(v[k])<<" "<<get<1>(v[k])<<"\n";
      // cout << get<0>(v[k])<<" "<< get<1>(v[k])<<endl;
    }
  }else{
    outfile << "0";
  }

  outfile.close();
  return 0;
}
