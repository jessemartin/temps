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

using namespace std;

vector<double> gpu;
vector<double> drive;

int col_width = 5;

void render(CPU cpu, const vector<double> &gpu, const vector<double> &drive, const int &height)
{
  ostringstream rendered;
  int title_height = 1;
  int footer_height = 1;
  int graph_height = height - title_height - footer_height;

  string cpu_header = "CPU Temps";
  int section_padding_right = 1;
  vector<double> cpu_temps = cpu.getTemps();
  int section_width = col_width * cpu_temps.size() + section_padding_right;

  rendered << "|" << setw(section_width) << centered(cpu_header) << "|\n";
  for (int i = graph_height; i > 0; i--)
  {
    rendered << "| ";
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
    rendered << "|\n";
  }

  rendered << "| ";
  for (int i = 0; i < cpu_temps.size(); i++) {
    ostringstream temp;
    temp << cpu_temps.at(i);
    rendered << setw(col_width) << centered(temp.str());
  }
  rendered << "|\n";

  cout << rendered.str();
}

string probeGpu() {
  return execCommand("nvidia-smi --id=0 --query-gpu=temperature.gpu --format=csv");
}

vector<double> parseGpuTemp(const string &gpu_temp) {
}

vector<double> getGpuTemp() {
  string gpu_temp = probeGpu();
  return parseGpuTemp(gpu_temp);
}

void updateGpuTemp() {
  gpu = getGpuTemp();
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
  // GPU gpu {};
  while(true) {
    cpu.update();
    // gpu.update();
    updateGpuTemp();
    drive = {26.2};
    render(cpu, gpu, drive, height);
    this_thread::sleep_for(chrono::seconds(1));
    clearScreen(height);
  }
  return 0;
}
