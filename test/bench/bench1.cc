
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
#define P 3
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
  cout << "Launching PICHI benchmark test 1" << endl << endl;
  cout << "Computing the diagram A_abc B_abd C_ce D_de" << endl;
  cout << "   Running the test " << N << " times with tensors of size " <<
       SIZE << endl;
  cout << "   Warm-up runs without measuring: " << P << endl << endl;

  cout << "---------------------------------- " << endl << endl;
  cout << "#\tTime/sec\tResult" << endl;
  cout << "------------------------" << endl;

}

void writeRun(int n, double etot, cdouble res) {
  cout << setprecision(3) << n << "\t" << etot << "\t" << res << endl;
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

    Tensor a(3, SIZE);
    Tensor b(3, SIZE);
    Tensor c(2, SIZE);
    Tensor d(2, SIZE);

    fill3(a);
    fill3(b);
    fill2(c);
    fill2(d);

    Contraction con;
    con.addTensor(0, a); // A_abc
    con.addTensor(1, b); // B_abd
    con.addTensor(2, c); // C_ce
    con.addTensor(3, d); // D_de

    auto start = chrono::steady_clock::now();

    con.contract(2,3, {{1, 1}},4); // E_cd
    con.contract(0,4, {{2, 0}},5); // F_abd
    cdouble r = con.contract(5,1,{{0,0},{1,1},{2,2}});


    auto end = chrono::steady_clock::now();

    double et_tot = chrono::duration_cast<
        chrono::duration<double>>(end-start).count();
    if (i >= 0) {
      times.push_back(et_tot);
      writeRun(i, et_tot, r);
    }

  }

  report(times);

  return 0;
}

