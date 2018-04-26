
/*
 * Benchmark test:
 *
 */

#include "tensor.h"
#include "contraction.h"

#include <random>
#include <iostream>
#include <iomanip>
#include <chrono>

#define N 10
#define P 10
#define SIZE 128

using namespace pichi;
using namespace std;

mt19937 gen;

cdouble rc() {
  uniform_real_distribution<> dist(-1,1);
  double r = dist(gen);
  double c = dist(gen);
  return cdouble(r,c);
}

void fill2(Tensor& t) {
  int s = t.size();
  cdouble slice[s*s];
  for (int i = 0; i < s*s; ++i)
    slice[i] = rc();
  t.setSlice({-1,-1},slice);
}

void fill3(Tensor& t) {
  int s = t.size();
  cdouble slice[s*s];
  for (int j = 0; j < s; ++j) {
    for (int i = 0; i < s * s; ++i)
      slice[i] = rc();
    t.setSlice({-1, -1, j}, slice);
  }
}

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

void header() {
  cout << "Launching PICHI benchmark test 2" << endl << endl;
  cout << "   Contraction pattern: A_abc B_abc" << endl;
  cout << "   Running the test " << N << " times with tensors of size " <<
       SIZE << endl;
  cout << "   Warm-up runs without measuring: " << P << endl << endl;

  cout << "---------------------------------- " << endl << endl;
  cout << "#\tTime/sec" << endl;
  cout << "------------" << endl;

}

void writeRun(int n, double etot) {
  cout << setprecision(3) << n << "\t" << etot << endl;
}

void report(vector<double> times) {
  cout << endl << "---- Report: ------------------" << endl << endl;

  cout << setprecision(3) << "Time : (" << mean(times) << " +/- " <<
       stdev(times) << ") sec." << endl;
}

int main() {

  gen.seed(time(NULL));

  header();

  vector<double> times;

  for (int i = -P; i < N; ++i) {

    // Contract random tensors of pattern A_abc B_cd C_dbe D_ae
    Tensor a(3, SIZE);
    Tensor c(3, SIZE);

    fill3(a);
    fill3(c);

    Contraction<char> con;
    con.addTensor('A', a);
    con.addTensor('C', c);

    auto start = chrono::steady_clock::now();

    con.contract('A', 'C', {{0, 0},{1,1},{2,2}});
    auto end = chrono::steady_clock::now();

    double et_tot = chrono::duration_cast<
        chrono::duration<double>>(end-start).count();
    if (i >= 0) {
      times.push_back(et_tot);
      writeRun(i, et_tot);
    }

  }

  report(times);

  return 0;
}

