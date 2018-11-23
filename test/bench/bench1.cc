
/*
 * Benchmark test:
 *
 */

#include "pichi/pichi.h"

#include <random>
#include <iostream>
#include <iomanip>
#include <chrono>

#define N 10
#define P 3
#define SIZE 16

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

    //Tensor a(2, SIZE);
    //Tensor b(2, SIZE);
    //Tensor c(3, SIZE);
    //Tensor d(3, SIZE);
    //Tensor e(4, SIZE);
    //Tensor f(4, SIZE);

    //fill2(a);
    //fill2(b);
    //fill3(c);
    //fill3(d);
    //fill4(e);
    //fill4(f);


    vector<Tensor> tensors(3);
    tensors[0].resize(2,SIZE);
    tensors[1].resize(2,SIZE);
    tensors[2].resize(4,SIZE);
    fill2(tensors[0]);
    fill2(tensors[1]);
    fill4(tensors[2]);

    Graph g("0ab1cd2abcd");

    auto start = chrono::steady_clock::now();

    //f.setStorage({2,3,1,0});
    //e.setStorage({2,3,1,0});

    Tensor res;
    contract(g,tensors,res);
    cdouble r[1];
    res.getSlice({0}, r);

    auto end = chrono::steady_clock::now();

    if (i == -P) {
      // Check correctness
      Tensor temp1,temp2;
      contract(tensors[0],tensors[2],{{0,0},{1,1}},temp1);
      contract(temp1,tensors[1],{{0,0},{1,1}},temp2);
      cdouble rtemp[1];
      temp2.getSlice({0},rtemp);

      cout << setprecision(17) << "Error = " << abs(r[0]-rtemp[0]) << endl;
    }

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

