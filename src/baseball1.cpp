///
/// baseball1.cpp
/// Compute the required pitch speed to reach home plate in the strike zone
/// Saves trajectory plot as PNG
///

#include "RKn.hpp"
#include "TROOT.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <vector>

using namespace std;

struct Params {
    double g;  // gravity
    double m;  // mass
    double d;  // diameter
    double b;  // linear drag coefficient
    double c;  // quadratic drag coefficient
};

// --- Derivative functions for RK4 solver ---
// y = {x, y_dummy, z, vx, vy_dummy, vz}

double fx(double t, const vector<double> &y, void *p) { return y[3]; }
double fy(double t, const vector<double> &y, void *p) { return 0.0; }
double fz(double t, const vector<double> &y, void *p) { return y[5]; }

double fvx(double t, const vector<double> &y, void *p) {
    Params *pars = (Params*) p;
    double vx = y[3], vz = y[5];
    double v = sqrt(vx*vx + vz*vz);
    if(v < 1e-12) v = 1e-12;
    double D = pars->b*v + pars->c*v*v;
    return -D*vx/v / pars->m;
}

double fvy(double t, const vector<double> &y, void *p) { return 0.0; }

double fvz(double t, const vector<double> &y, void *p) {
    Params *pars = (Params*) p;
    double vx = y[3], vz = y[5];
    double v = sqrt(vx*vx + vz*vz);
    if(v < 1e-12) v = 1e-12;
    double D = pars->b*v + pars->c*v*v;
    return -D*vz/v / pars->m - pars->g;
}

int main(int argc, char **argv) {

    // --- Problem parameters ---
    Params pars;
    pars.g = 9.81;
    pars.m = 0.145;
    pars.d = 0.075;
    pars.b = 1.6e-4 * pars.d; // needed to multiply by d
    pars.c = 0.25 * pars.d * pars.d; // needed to multiply by d^2

    void *p_par = (void*)&pars;

    double xend = 18.5;
    double z0 = 1.4;
    double theta0 = 1.0;
    bool savePlot = false;

    int c;
    while ((c = getopt(argc, argv, "x:z:t:p")) != -1)
        switch(c) {
            case 'x': xend = atof(optarg); break;
            case 'z': z0 = atof(optarg); break;
            case 't': theta0 = atof(optarg); break;
            case 'p': savePlot = true; break;
        }

    TApplication theApp("App", &argc, argv);

    double theta = theta0 * M_PI / 180.0;
    vector<pfunc_t> fnlist = {fx, fy, fz, fvx, fvy, fvz};

    vector<double> traj_x, traj_z;

    // --- simulate function ---
    auto simulate = [&](double v0, bool storePoints=false) {
        vector<double> y(6);
        y[0] = 0.0; y[1] = 0.0; y[2] = z0;
        y[3] = v0 * cos(theta); y[4] = 0.0; y[5] = v0 * sin(theta);

        double t = 0.0;
        double dt = 0.0005;

        if(storePoints){
            traj_x.clear();
            traj_z.clear();
            traj_x.push_back(y[0]);
            traj_z.push_back(y[2]);
        }

        while(y[0] < xend && y[2] > 0.0){
            y = RK4StepN(fnlist, y, t, dt, p_par);
            t += dt;
            if(storePoints){
                traj_x.push_back(y[0]);
                traj_z.push_back(y[2]);
            }
        }
        return y[2];
    };

    // --- Bisection method ---
    double targetZ = 0.9;
    double low = 25.0, high = 60.0;

    for(int i = 0; i < 50; i++){
        double mid = 0.5*(low+high);
        double zfinal = simulate(mid);
        if(zfinal > targetZ) high = mid;
        else low = mid;
    }

    double vPitch = 0.5*(low+high);

    printf("********************************\n");
    printf("(xend,z0,theta0) = (%lf,%lf,%lf)\n", xend, z0, theta0);
    printf("v_pitch = %lf m/s\n", vPitch);
    printf("********************************\n");

    // --- Save plot ---
    if(savePlot){
        simulate(vPitch, true);

        TCanvas *c1 = new TCanvas("c1","Baseball Trajectory",800,600);
        TGraph *gr = new TGraph(traj_x.size(), traj_x.data(), traj_z.data());
        gr->SetTitle("Baseball Trajectory; x [m]; z [m]");
        gr->SetLineWidth(2);
        gr->Draw("AL");

        TGraph *strike = new TGraph(2, (double[]){0.0, xend}, (double[]){0.9,0.9});
        strike->SetLineColor(kRed);
        strike->SetLineStyle(2);
        strike->Draw("L");

        TGraph *plate = new TGraph(1, (double[]){xend}, (double[]){0.9});
        plate->SetMarkerStyle(20);
        plate->SetMarkerColor(kBlue);
        plate->Draw("P");

        c1->SaveAs("trajectory.png");
        cout << "Trajectory plot saved as trajectory.png" << endl;
    }

    return 0;
}

