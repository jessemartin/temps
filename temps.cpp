// #include "smc.c"
// #include "smc.h"
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

void render(vector<double> cpu, vector<double> gpu, vector<double> drive)
{
  printf("   CPU   | GPU | Drive\n");
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

int main()
{
  char buffer[128];
  string result = "";
  FILE *pipe = popen("ps ax", "r");
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
  vector<double> cpu = {45.2, 36.1, 93.0, 100.0};
  vector<double> gpu = {29.0};
  vector<double> drive = {26.2};
  render(cpu, gpu, drive);
  return 0;
}
