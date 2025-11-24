///
/// Starter template for second baseball problem
///
///  Do not change the interface for running the program
///  Fill in the value of vPitch in the print statement with your solution
///  at the end of main()
///

#include "RKn.hpp"
#include "TROOT.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TFile.h"
#include "TStyle.h"
#include "TGClient.h"
#include "TF1.h"
#include "TCanvas.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

// COPY-PASTED CODE FROM baseball1.cpp original branch part_1_b_malinda from which this branch was created.

// parameters
struct Params {
  double g;  // gravity
  double m;  // mass
  double d;  // diameter
  double b;  // linear drag coefficient
  double c;  // quadratic drag coefficient
};

// y={x,y,z,v_x,v_y,v_z}
// this is position (3), velocity (3)

// position functions
// Fitzpatrick 3.44
// dx/dt = v_x
double fx(double t, const vector<double> &y, void *p) { return y[3]; }
// Fitzpatrick 3.45
// dy/dt = v_y
double fy(double t, const vector<double> &y, void *p) { return y[4]; }
// Fitzpatrick 3.46
// dz/dt = v_z
double fz(double t, const vector<double> &y, void *p) { return y[5]; }

// velocity functions
// Fitzpatrick 3.47
// (w stands for omega). w = w(0,sin(phi),cos(phi))
// dv_x/dt = -F(v)vv_x + Bw(v_x*sin(phi) - v_y*cos(phi))
double fvx(double t, const vector<double> &y, void *p) {
  Params *pars = (Params*) p;
  double vx = y[3], vz = y[5];
  double v = sqrt(vx*vx + vz*vz);
  if(v < 1e-12) v = 1e-12;
  double D = pars->b*v + pars->c*v*v;
  return -D*vx/v / pars->m;
}
// Fitzpatrick 3.48
// dv_y/dt = -F(v)vv_y + Bwv_x*cos(phi)
double fvy(double t, const vector<double> &y, void *p) { return 0.0; }
// Fitzpatrick 3.49
// dv_z/dt = -g -F(v)vv_z - Bwv_x*sin(phi)
double fvz(double t, const vector<double> &y, void *p) {
  Params *pars = (Params*) p;
  double vx = y[3], vz = y[5];
  double v = sqrt(vx*vx + vz*vz);
  if(v < 1e-12) v = 1e-12;
  double D = pars->b*v + pars->c*v*v;
  return -D*vz/v / pars->m - pars->g;
}

int main(int argc, char **argv){

  // IGNORE THESE COMMENTS BELOW SINCE THEY'RE OUT OF DATE BASED ON HOW MALINDA
  // WROTE THE y VECTOR
  // we have 6 initial conditions for this problem
  // y[0] = y[2] = y[4] = 0;  // init x,y,z
  // y[1] = v0*cos(theta0);   // vx  "x is line towards the plate
  // y[3] = 0;                // vy  "y" is measured as left/right divergence from line to plate
  // y[5] = v0*sin(theta0);   // vz  "z" is vertival measure
  vector<double> y0(6);

  // boundary conditions at t = 0
  y0[0] = 0; // Fitzpatrick 3.50
  y0[1] = 0; // Fitzpatrick 3.51
  y0[2] = 0; // Fitzpatrick 3.52
  y0[3] = vPitch * cos(theta0); // Fitzpatrick 3.53
  y0[4] = 0; // Fitzpatrick 3.54
  y0[5] = vPitch * sin(theta0); // Fitzpatrick 3.55

  bool showPlot=false;
  // pitches
  // slider ip=0
  // curve ip=1
  // screwball ip=2
  // fast ip=3
  int ip=1;    // default pitch
  int c;
  while ((c = getopt (argc, argv, "p:n")) != -1)
    switch (c) {
    case 'p':
      ip = atoi(optarg);
      break;
    case 'n':
      showPlot=false;
      break;
    }

  TString title;
  if (ip==0){
    cout << "Setting up initial conditions for slider" << endl;
    //SetupSlider(y0);
  }
  else if (ip==1){
    cout << "Setting up initial conditions for curveball" << endl;
    //SetupCurve(y0);
  }
  else if (ip==2){
    cout << "Setting up initial conditions for screwball" << endl;
    //SetupScrewball(y0);
  }
  else {
    cout << "Setting up initial conditions for fastball" << endl;
    //SetupFastball(y0);
  }

  TApplication theApp("App", &argc, argv); // init ROOT App for displays

  double xend=60.5;   // feet
  double yend=0;    // tbd
  double zend=0;    // tbd
  double vxend=0;
  double vyend=0;
  double vzend=0;

  // write code here


  // to compare to the plots in Fitzpatrick, output your results in **feet**
  // do not change these lines
  printf("********************************\n");
  printf("Coordinates when x=60 feet\n");
  printf("(x,y,x) = (%lf,%lf,%lf)\n",xend,yend,zend);
  printf("(vx,vy,vz) = (%lf,%lf,%lf)\n",vxend,vyend,vzend);
  printf("********************************\n");

  // plot the trajectory.  See Fitzpatrick for plot details
  if (showPlot){
    cout << "Press ^c to exit" << endl;
    theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
    theApp.Run();
  }
  
  return 0;
}

