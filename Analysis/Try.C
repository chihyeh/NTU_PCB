#include <iostream>
#include <fstream>

using namespace std;
float calcMHWScore(vector<float> scores)
{
  size_t size = scores.size();

  if (size == 0)
  {
    return 0;  // Undefined, really.
  }
  else
  {
    sort(scores.begin(), scores.end());
    if (size % 2 == 0)
    {
      return (scores[size / 2 - 1] + scores[size / 2]) / 2;
    }
    else 
    {
      return scores[size / 2];
    }
  }
}

int Try(){
	vector<float> Try={1,2,3,3};
    cout << "Size:" << Try.size() << endl;
	cout << calcMHWScore(Try) << endl;
	return 0;
}

