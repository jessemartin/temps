#include <stdio.h>
#include <string>

using namespace std;

string execCommand(const string &command) {
  char buffer[128];
  string result = "";
  FILE *pipe = popen(command.c_str(), "r");
  if (!pipe) {
    printf("popen() failed!");
  }
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
