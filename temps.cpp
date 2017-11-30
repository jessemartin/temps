#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <math.h>
#include "center.cpp"

using namespace std;

vector<double> cpu;
vector<double> gpu;
vector<double> drive;

int col_width = 5;

void render(const vector<double> &cpu, const vector<double> &gpu, const vector<double> &drive, const int &height)
{
  ostringstream rendered;

  string cpu_header = "CPU Temps";
  int section_padding_right = 1;
  int section_width = col_width * cpu.size() + section_padding_right;

  rendered << "|" << setw(section_width) << centered(cpu_header) << "|\n";
  for (int i = 10; i > 0; i--)
  {
    rendered << "| ";
    for (int j = 0; j < cpu.size(); j++)
    {
      if (cpu[j] >= i * 10)
      {
        rendered << setw(col_width) << centered("==");
      }
      else
      {
        rendered << setw(col_width) << centered("..");
      }
    }
    rendered << "|\n";
  }

  rendered << "| ";
  for (int i = 0; i < cpu.size(); i++) {
    ostringstream temp;
    temp << cpu.at(i);
    rendered << setw(col_width) << centered(temp.str());
  }
  rendered << "|\n";

  cout << rendered.str();
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

vector<double> parse_cpu_temp(const string &cpu_temp) {
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

vector<double> get_cpu_temp() {
  string cpu_temp = probe_cpu();
  return parse_cpu_temp(cpu_temp);
}

void update_cpu_temp() {
  cpu = get_cpu_temp();
}

void clear_screen(const int &height) {
  for (int i = 0; i < height; i++) {
    cout << "\033[A\033[2K";
  }
}

int main()
{
  int height = 12;
  while(true) {
    update_cpu_temp();
    gpu = {29.0};
    drive = {26.2};
    render(cpu, gpu, drive, height);
    this_thread::sleep_for(chrono::seconds(1));
    clear_screen(height);
  }
  return 0;
}
