#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TApplication.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

// Constants
const double g = 32.17;    // Gravity (ft/s^2)
const double B = 0.00041;  // Drag coefficient
const double S = 4.1e-4;   // Spin coefficient

// Magnus force
void MagnusForce(double vx, double vy, double vz, double omega[3], double &fx, double &fy, double &fz) {
    fx = S * (omega[1] * vz - omega[2] * vy);
    fy = S * (omega[2] * vx - omega[0] * vz);
    fz = S * (omega[0] * vy - omega[1] * vx);
}

// RK4 solver for vector system
void RK4Step(void (*f)(double, const vector<double>&, vector<double>&, double*),
             vector<double> &y, double t, double h, double omega[3]) {
    size_t n = y.size();
    vector<double> k1(n), k2(n), k3(n), k4(n), ytemp(n);

    f(t, y, k1, omega);
    for (size_t i=0;i<n;i++) ytemp[i] = y[i] + 0.5*h*k1[i];
    f(t+h/2.0, ytemp, k2, omega);
    for (size_t i=0;i<n;i++) ytemp[i] = y[i] + 0.5*h*k2[i];
    f(t+h/2.0, ytemp, k3, omega);
    for (size_t i=0;i<n;i++) ytemp[i] = y[i] + h*k3[i];
    f(t+h, ytemp, k4, omega);
    for (size_t i=0;i<n;i++) y[i] += (h/6.0)*(k1[i]+2*k2[i]+2*k3[i]+k4[i]);
}

// Equations of motion
void Equations(double t, const vector<double> &y, vector<double> &dydt, double* omega) {
    double vx = y[1], vy = y[3], vz = y[5];
    double v = sqrt(vx*vx + vy*vy + vz*vz);

    double fx, fy, fz;
    MagnusForce(vx, vy, vz, omega, fx, fy, fz);

    dydt[0] = vx;
    dydt[1] = -B*v*vx + fx;
    dydt[2] = vy;
    dydt[3] = -B*v*vy - g + fy;
    dydt[4] = vz;
    dydt[5] = -B*v*vz + fz;
}

// Set up initial conditions
void SetupPitch(const string &pitch, vector<double> &y0, double omega[3]) {
    double v0, theta, phi, spin;
    if (pitch=="slider") {v0=85*1.467; theta=M_PI/180; phi=0; spin=1800;}
    else if (pitch=="curveball") {v0=85*1.467; theta=M_PI/180; phi=45*M_PI/180; spin=1800;}
    else if (pitch=="screwball") {v0=85*1.467; theta=M_PI/180; phi=135*M_PI/180; spin=1800;}
    else if (pitch=="fastball") {v0=95*1.467; theta=M_PI/180; phi=225*M_PI/180; spin=1800;}
    else {cerr<<"Unknown pitch type"<<endl; exit(1);}

    y0 = {0, v0*cos(theta), 0, 0, 0, v0*sin(theta)};
    omega[0] = spin*cos(phi);
    omega[1] = spin*sin(phi);
    omega[2] = 0;
}

int main(int argc, char** argv) {
    TApplication theApp("App",&argc,argv);

    vector<string> pitches = {"slider","curveball","screwball","fastball"};
    for (const string &pitch : pitches) {
        vector<double> y0(6);
        double omega[3];
        SetupPitch(pitch,y0,omega);

        // RK4 integration
        double h = 1e-4;
        double tmax = (pitch=="fastball")?0.46:0.52;
        vector<vector<double>> trajectory;
        double t=0;
        while (t<tmax) {
            trajectory.push_back(y0);
            vector<double> dydt(6);
            Equations(t,y0,dydt,omega);
            RK4Step(Equations, y0, t, h, omega);
            t+=h;
        }

        // Extract trajectory
        vector<double> x, y, z;
        for (auto &s:trajectory) {x.push_back(s[0]); y.push_back(s[2]); z.push_back(s[4]);}

        // Print coordinates at plate (x ~ 60.6 ft)
        for (auto &s:trajectory) {
            if (s[0] >= 60.6) {
                cout << pitch << " final: x=" << s[0] << ", y=" << s[2] << ", z=" << s[4] << endl;
                break;
            }
        }

        // Dynamic axis limits
        double yMin = *min_element(y.begin(),y.end());
        double yMax = *max_element(y.begin(),y.end());
        double zMin = *min_element(z.begin(),z.end());
        double zMax = *max_element(z.begin(),z.end());
        double yPad = 0.1*(yMax-yMin); double zPad = 0.1*(zMax-zMin);
        yMin-=yPad; yMax+=yPad; zMin-=zPad; zMax+=zPad;

        // Plot
        TCanvas *c = new TCanvas(pitch.c_str(),pitch.c_str(),800,600);
        TGraph *gY = new TGraph(x.size(), &x[0], &y[0]);
        TGraph *gZ = new TGraph(x.size(), &x[0], &z[0]);
        gY->SetLineColor(kBlack);
        gZ->SetLineColor(kBlue); gZ->SetLineStyle(2);
        gY->SetTitle((pitch+" Trajectory;X (ft);Y/Z (ft)").c_str());
        gY->GetYaxis()->SetRangeUser(min(yMin,zMin),max(yMax,zMax));
        gY->Draw("AL"); gZ->Draw("L same");
        c->SaveAs((pitch+".png").c_str());
    }

    theApp.Run();
    return 0;
}

