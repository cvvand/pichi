
/*
 * Benchmark test:
 *
 */

#include "tensor.h"
#include "contraction.h"
#include "diagrams.h"

#include <random>
#include <iostream>
#include <iomanip>
#include <chrono>

#define N 100
#define P 20
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
  cout << "#\tT/ms\tResult" << endl;
  cout << "------------------------" << endl;

}

void writeRun(int n, double etot, cdouble res) {
  cout << setprecision(3) << n << "\t" << 1000*etot << "\t\t" << res << endl;
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

    Tensor* a = new Tensor(2, SIZE);
    Tensor* b = new Tensor(2, SIZE);
    Tensor* c = new Tensor(2, SIZE);
    Tensor* d = new Tensor(2, SIZE);

    fill2(*a);
    fill2(*b);
    fill2(*c);
    fill2(*d);

    DiagramNode dna, dnb, dnc, dnd;
    dna.t = a; dna.idx = "ab";
    dnb.t = b; dnb.idx = "bc";
    dnc.t = c; dnc.idx = "cd";
    dnd.t = d; dnd.idx = "ad";

    vector<DiagramNode> nodes;
    nodes.push_back(dna);
    nodes.push_back(dnb);
    nodes.push_back(dnc);
    nodes.push_back(dnd);

    auto start = chrono::steady_clock::now();

    cdouble r = diagram3(nodes);

    auto end = chrono::steady_clock::now();

    double et_tot = chrono::duration_cast<
        chrono::duration<double>>(end-start).count();
    if (i >= 0) {
      times.push_back(et_tot);
      writeRun(i, et_tot, r);
    }

    delete a; delete b; delete c; delete d;

  }

  report(times);

  return 0;
}

