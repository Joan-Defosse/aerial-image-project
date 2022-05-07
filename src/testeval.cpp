#include <chrono>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "../head/Analyst.h"

using namespace std;

int main(void)
{
  srand(time(nullptr));

  try
  {
    int pixelSize = 10;

    vector<string> filenames = { "images/amazonie_0", "images/amazonie_1", "images/amazonie_2", "images/amazonie_3", "images/amazonie_4" };

    for (auto name : filenames)
    {
      auto start = chrono::system_clock::now();

      Image img = Image::readAIP(name);
      Analyst analyst(img);

      cout << analyst.nbZones()
           << ", "
           << analyst.nbZonesOfColor(Color::Green)
           << ", "
           << analyst.nbPixelsOfColor(Color::Green)
           << endl;

      auto end = chrono::system_clock::now();

      chrono::duration<double> elapsed_seconds = end-start;
      cout << "elapsed time: " << elapsed_seconds.count() << "s" << endl;
    }
  }
  catch(exception e)
  {
    cerr << e.what() << endl;
  }

  return 0;
}
