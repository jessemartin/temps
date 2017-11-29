// #include "smc.c"
// #include "smc.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <regex>
#include <string>

using namespace std;

vector<double> cpu;
vector<double> gpu;
vector<double> drive;

void render(vector<double> cpu, vector<double> gpu, vector<double> drive)
{
  cout << "   CPU   | GPU | Drive\n";
  for (int i = 10; i > 0; i--)
  {
    string row = " ";
    for (int j = 0; j < cpu.size(); j++)
    {
      if (cpu[j] >= i * 10)
      {
        row += "- ";
      }
      else
      {
        row += "  ";
      }
    }
    printf("%s|\n", row.c_str());
  }
}

string probe_cpu() {
  char buffer[128];
  string result = "";
  FILE *pipe = popen("sensors", "r");
  if (!pipe)
    printf("popen() failed!");
  try
  {
    while (!feof(pipe))
    {
      if (fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
  }
  catch (...)
  {
    pclose(pipe);
    throw;
  }
  pclose(pipe);
  return result;
}

vector<string> split(const string &s, char delim) {
  stringstream ss(s);
  string item;
  vector<string> tokens;
  while (getline(ss, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}

vector<double> parse_cpu_temp(string cpu_temp) {
  vector<string> lines = split(cpu_temp.c_str(), '\n');
  vector<double> temps;
  regex core_temp ("Core[^+|-]*[+|-]([^°]*).*");
  smatch matches;
  for (int i = 0; i < lines.size(); i++) {
    regex_match(lines[i], matches, core_temp);
    if (matches.size() == 2) {
      double temp = stod(matches.str(1));
      temps.push_back(temp);
    }
  }
  return temps;
}

vector<double> get_cpu_temp() {
  string cpu_temp = probe_cpu();
  return parse_cpu_temp(cpu_temp);
}

void update_cpu_temp() {
  cpu = get_cpu_temp();
}

int main()
{
  update_cpu_temp();

  gpu = {29.0};
  drive = {26.2};
  render(cpu, gpu, drive);
  return 0;
}
