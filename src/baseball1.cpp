///
/// Starter template for first baseball problem
/// Solve for the initial speed of the pitch given the initial parameters
/// xend : distance to home plate [18.5] m
/// z0 : height of release of ball [1.4] m
/// theta0 : angle of release above horizontal [1] degree
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

struct Params {
  double g;   // acceleration [m/s^2]
  double m;   // mass of object [kg], nb proj. In vacuum funcs do not depend on the mass
  double d;   // m diameter of ball
  double b;   // b,c params for air resistance
  double c;
};

// Here, we need to insert the functions that describe our projectile motion.
// Use ri, rj, rk to define directions (x, y, z).

/// \brief Change in position along \f$\hat i\f$ axis
/// \param[in] x independent variable
/// \param[in] y dependent variables
double f_ri(double x, const vector<double> &y, void *params=0){ 
  (void) x;   // prevent unused variable warning
  return y[1];
}

/// \brief Change in velocity along  \f$\hat i\f$ axis
/// \param[in] x independent variable
/// \param[in] y dependent variables
double f_vi(double x, const vector<double> &y, void *params=0){ 
  (void) x;
  Params *p = (Params*)params;
  double v = sqrt(y[1]*y[1] + y[3]*y[3]);
  return -p->air_k * v * y[1] / p->m;
}

/// \brief Change in position along \f$\hat j\f$ axis
/// \param[in] x independent variable
/// \param[in] y dependent variables
///
/// Air resistance model: F= \f$k v^2\f$
///
double f_rj(double x, const vector<double> &y, void *params=0){  
  (void) x;   // prevent unused variable warning
  return y[3];
}

/// Change in velocity along  \f$\hat j\f$ axis
/// \param[in] x independent variable
/// \param[in] y dependent variables
double f_vj(double x, const vector<double> &y, void *params=0){  
  (void) x;
  Params *p = (Params*)params;
  return -p->air_k * sqrt(y[1]*y[1] + y[3]*y[3]) * y[3] / p->m - p->g;
}

/// \brief Stopping condition
/// \param[in] x independent variable
/// \param[in] y dependent variables
///
/// Returns 0(1) to flag continuation(termination) of calculation 
double f_stop(double x, const vector<double> &y, void *params=0){
  (void) x;
  if (y[2]<0) return 1;  // stop calulation if the current step takes height to negative value
  return 0;  // continue calculation
}

int main(int argc, char **argv){

  // examples of parameters
  Params pars;
  pars.g=9.81;
  pars.m=0.145;    
  pars.d=0.0075;   
  pars.b=1.6e-4;  
  pars.c=0.25;
  void *p_par = (void*) &pars;

  double xend=18.5;       // meters to plate
  double z0=1.4;             // height of release [m]
  double theta0=1;         // angle of velocity at release (degrees)
                                      // convert to radians before using!
  bool showPlot=false;    // keep this flag false by default
  
  // allow changing the parameters from the command line
  int c;
  while ((c = getopt (argc, argv, "x:z:t:p")) != -1)
    switch (c) {
    case 'x':
      xend = atof(optarg);
      break;
    case 'z':
      z0 = atof(optarg);
      break;
    case 't':
      theta0 = atof(optarg);
      break;
    case 'p':
      showPlot=true;
      break;
    case '?':
      fprintf (stderr, "Unknown option `%c'.\n", optopt);
    }
  TApplication theApp("App", &argc, argv); // init ROOT App for displays


  double vPitch = 0;   // m/s of pitch needed to land in strike zone at 0.9 meters
  // write code to solve for vPitch here

  // ******************************************************************************
  // ** this block is useful for supporting both high and std resolution screens **
  UInt_t dh = gClient->GetDisplayHeight()/2;   // fix plot to 1/2 screen height  
  //UInt_t dw = gClient->GetDisplayWidth();
  UInt_t dw = 1.1*dh;
  // ******************************************************************************

  // --- set up RK4 solver ---
  vector<pfunc_t> v_fun(4); // ODEs for motion
  v_fun[0] = f_ri; // x
  v_fun[1] = f_vi; // v_x
  v_fun[2] = f_rj; // y
  v_fun[3] = f_vj; // v_y

  vector<double> y(4); // initial conditions
  y[0] = 0;
  y[1] = v0 * cos(theta*M_PI/180.0);
  y[2] = 0;
  y[3] = v0 * sin(theta*M_PI/180.0);

  cout << "Simulating projectile fall with air resistance...\n";

  auto tgN = RK4SolveN(v_fun, y, nsteps, x, xmax, p_par, f_stop);

  // do the plotting
  TCanvas *c2 = new TCanvas("c2","ODE solutions 2",dw,dh);
  tgN[2].Draw("al*");
  c2->Draw();

  cout << "Final velocity = " << sqrt(y[1]*y[1]+y[3]*y[3]) << endl;
  
  // save our graphs
  TFile *tf=new TFile("RKnDemo.root","recreate");
  for (unsigned i=0; i<v_fun.size(); i++){
    tgN[i].Write();
  }
  tf->Close();

  // do not change these lines
  printf("********************************\n");
  printf("(xend,z0,theta0) = (%lf,%lf,%lf)\n",xend,z0,theta0);
  printf("v_pitch = %lf m/s\n",vPitch);
  printf("********************************\n");

  if (showPlot){
    cout << "Press ^c to exit" << endl;
    theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
    theApp.Run();
  }
  
  return 0;
}
