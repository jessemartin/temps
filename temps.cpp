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
#include <map>
#include "center.cpp"
#include "cpu.cpp"
#include "gpu.cpp"

using namespace std;

int col_width = 5;

bool supportsColors() {
  return true;
}

map<int,int> colorTemps = {
  { 1, 34 },//blue         34         44
  { 2, 36 },//cyan         36         46
  { 3, 32 },//green        32         42
  { 4, 33 },//yellow       33         43
  { 5, 31 },//red          31         41
  { 6, 35 },//magenta      35         45
  { 7, 37 },//white        37         47
  { 8, 37 },//white        37         47
  { 9, 37 },//white        37         47
  { 10, 37 }//white        37         47
};

void printColoredTempBar(const int &width, const int &step, ostringstream &rendered) {
  int color = colorTemps[step];
  string colorString = "\033[1;" + to_string(color) + "m";
  rendered << colorString << setw(width);
  rendered << centered("==");
  rendered << "\033[0m";
};

void render(CPU cpu, GPU gpu, const int &height)
{
  ostringstream rendered;
  int title_height = 1;
  int footer_height = 1;
  int graph_height = height - title_height - footer_height;

  int section_padding_right = 1;

  string cpu_header = "CPU";
  vector<double> cpu_temps = cpu.getTemps();
  int cpu_section_width = col_width * cpu_temps.size() + section_padding_right;

  string gpu_header = "GPU";
  vector<double> gpu_temps = gpu.getTemps();
  int gpu_section_width = col_width * gpu_temps.size() + section_padding_right;

  // Header
  if (cpu_temps.size()) {
    rendered << "|" << setw(cpu_section_width) << centered(cpu_header);
  }
  if (gpu_temps.size()) {
    rendered << "|" << setw(gpu_section_width) << centered(gpu_header);
  }
  rendered << "|\n";

  // Bar graphs
  for (int i = graph_height; i > 0; i--)
  {
    if (cpu_temps.size()) {
      rendered << "| ";
    }
    // CPU temp
    for (int j = 0; j < cpu_temps.size(); j++)
    {
      if (cpu_temps[j] >= i * 10)
      {
        printColoredTempBar(col_width, i, rendered);
      }
      else
      {
        rendered << setw(col_width) << centered("..");
      }
    }
    if (gpu_temps.size()) {
      rendered << "|";
    }
    // GPU temp
    for (int j = 0; j < gpu_temps.size(); j++)
    {
      if (gpu_temps[j] >= i * 10)
      {
        printColoredTempBar(col_width + 1, i, rendered);
      }
      else
      {
        rendered << setw(col_width + 1) << centered("..");
      }
    }
    rendered << "|\n";
  }

  // Footer
  if (cpu_temps.size()) {
    rendered << "| ";
  }
  // CPU temp
  for (int i = 0; i < cpu_temps.size(); i++) {
    ostringstream temp;
    temp << cpu_temps.at(i);
    rendered << setw(col_width) << centered(temp.str());
  }
  if (gpu_temps.size()) {
    rendered << "|";
  }
  // GPU temp
  for (int i = 0; i < gpu_temps.size(); i++) {
    ostringstream temp;
    temp << gpu_temps.at(i);
    rendered << setw(col_width + 1) << centered(temp.str());
  }
  rendered << "|\n";

  cout << rendered.str();
}

void clearScreen(const int &height) {
  for (int i = 0; i < height; i++) {
    cout << "\033[A\033[2K";
  }
}

int main()
{
  int height = 12;

  CPU cpu {};
  GPU gpu {};
  while(true) {
    cpu.update();
    gpu.update();
    render(cpu, gpu, height);
    this_thread::sleep_for(chrono::seconds(1));
    clearScreen(height);
  }
  return 0;
}
