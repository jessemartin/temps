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
#include "cpu.cpp"
#include "gpu.cpp"

using namespace std;

int col_width = 5;

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
  rendered << "|" << setw(cpu_section_width) << centered(cpu_header);
  rendered << "|" << setw(gpu_section_width) << centered(gpu_header);
  rendered << "|\n";

  // Bar graphs
  for (int i = graph_height; i > 0; i--)
  {
    rendered << "| ";
    // CPU temp
    for (int j = 0; j < cpu_temps.size(); j++)
    {
      if (cpu_temps[j] >= i * 10)
      {
        rendered << setw(col_width) << centered("==");
      }
      else
      {
        rendered << setw(col_width) << centered("..");
      }
    }
    rendered << "|";
    // GPU temp
    for (int j = 0; j < gpu_temps.size(); j++)
    {
      if (gpu_temps[j] >= i * 10)
      {
        rendered << setw(col_width + 1) << centered("==");
      }
      else
      {
        rendered << setw(col_width + 1) << centered("..");
      }
    }
    rendered << "|\n";
  }

  // Footer
  rendered << "| ";
  // CPU temp
  for (int i = 0; i < cpu_temps.size(); i++) {
    ostringstream temp;
    temp << cpu_temps.at(i);
    rendered << setw(col_width) << centered(temp.str());
  }
  rendered << "|";
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
