#include <stdio.h>

using namespace std;

string probeGpu() {
  return execCommand("nvidia-smi --id=0 --query-gpu=temperature.gpu --format=csv");
}

vector<double> parseGpuTemp(const string &gpu_temp) {
  vector<string> lines = split(gpu_temp.c_str(), '\n');
  vector<double> temps;
  if (lines.size() == 2) {
    double temp = stod(lines.at(1));
    temps.push_back(temp);
  }
  return temps;
}

vector<double> getGpuTemp() {
  string gpu_temp = probeGpu();
  return parseGpuTemp(gpu_temp);
}

class GPU
{
private:
  vector<double> temps;
 
public:
  GPU()
  {
  }

  void update()
  {
    temps = parseGpuTemp(probeGpu());
  }

  vector<double> getTemps() {
    return temps;
  }
};

