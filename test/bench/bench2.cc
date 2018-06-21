#include <armadillo>
#include <chrono>
#include <iomanip>

#define N 64
#define P 5
#define R 10000
#define S 0.04

using namespace arma;
using namespace std;


double mean(vector<double> x) {
  double sum = 0.0;
  for (double xx : x)
    sum += xx;
  return sum/x.size();
}

double stdev(vector<double> x) {
  double m = mean(x);
  double sum = 0.0;
  for (double xx : x)
    sum += (xx - m)*(xx - m);
  return sqrt(sum/x.size());
}

int main() {

  cout << endl;

  vector<double> times;



  for (int i = -P; i < R; ++i) {

    sp_mat x = sprandu(N,N,S);
    sp_mat y = sprandu(N,N,S);

    auto start = chrono::steady_clock::now();

    sp_mat z = x*y;

    auto end = chrono::steady_clock::now();

    double et_tot = chrono::duration_cast<
        chrono::duration<double>>(end-start).count();
    if (i >= 0) {
      times.push_back(et_tot);
    }

  }

  double m = mean(times)*1000;
  double s = stdev(times)*1000;

  cout << setprecision(3) << m << " +/- " << s << endl;


  return 0;
}

