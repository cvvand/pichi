
/*
 * Benchmark test:
 *
 */

#include "pichi/pichi.h"

#include <random>
#include <iostream>
#include <iomanip>
#include <chrono>

#define N 100
#define P 5
#define SIZE 64

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
  int s = t.getSize();
  cdouble slice[s*s];
  for (int i = 0; i < s*s; ++i)
    slice[i] = rc();
  t.setSlice({-1,-1},slice);
}

void fill3(Tensor& t) {
  int s = t.getSize();
  cdouble slice[s*s];
  for (int j = 0; j < s; ++j) {
    for (int i = 0; i < s * s; ++i)
      slice[i] = rc();
    t.setSlice({-1, -1, j}, slice);
  }
}

void fill4(Tensor& t) {
  int s = t.getSize();
  cdouble slice[s*s];
  for (int k = 0; k < s; ++k) {
    for (int j = 0; j < s; ++j) {
      for (int i = 0; i < s * s; ++i)
        slice[i] = rc();
      t.setSlice({-1, -1, j, k}, slice);
    }
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
  cout << "Launching PICHI benchmark test 1" << endl << endl;
  cout << "Computing the contraction A_ab B_ac" << endl;
  cout << "   Running the test " << N << " times with tensors of size " <<
       SIZE << endl;
  cout << "   Warm-up runs without measuring: " << P << endl << endl;

  cout << "---------------------------------- " << endl << endl;
  cout << "#\tT/ms" << endl;
  cout << "------------------------" << endl;

}

void writeRun(int n, double etot) {
  cout << setprecision(3) << n << "\t" << 1000*etot << endl;
}

void report(vector<double> times) {
  cout << endl << "---- Report: ------------------" << endl << endl;

  cout << setprecision(3) << "Time : (" << 1000*mean(times) << " +/- " <<
          setprecision(2) <<1000*stdev(times) << ") ms." << endl;
}

int main() {

  gen.seed(time(NULL));

  header();

  vector<double> times;

  for (int i = -P; i < N; ++i) {

    //Tensor a(3, SIZE);
    //Tensor b(3, SIZE);
    Tensor c(2, SIZE);
    Tensor d(2, SIZE);
    //Tensor e(4, SIZE);
    //Tensor f(4, SIZE);

    //fill3(a);
    //fill3(b);
    fill2(c);
    fill2(d);
    //fill4(e);
    //fill4(f);

    auto start = chrono::steady_clock::now();

    //f.setStorage({2,3,1,0});
    //e.setStorage({2,3,1,0});

    Tensor t1 = contract(c,d, {{0,0},{1,1}});
    //Tensor t2 = contract(t1,b, {{0,0},{1,1},{2,2}});
    //Tensor t3 = contract(t2,d, {{0,1},{1,0}});
    //cdouble r[1];
    //t2.getSlice({0}, r);

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

