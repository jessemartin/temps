#include <stdio.h>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include "command.cpp"

using namespace std;

string probeCpu() {
  return execCommand("sensors");
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

vector<double> parseCpuTemp(const string &cpu_temp) {
  vector<string> lines = split(cpu_temp.c_str(), '\n');
  vector<double> temps;
  regex core_temp ("Core[^+|-]*([^°]*).*");
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

vector<double> getCpuTemp() {
}

class CPU
{
private:
  vector<double> temps;

public:
  CPU()
  {
  }

  void update()
  {
    temps = parseCpuTemp(probeCpu());
  }

  vector<double> getTemps() {
    return temps;
  }
};

