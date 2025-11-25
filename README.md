# ode3

physx630 odelib
---

To build the ODE library and example programs, simply type `make` in this top level ode3 directory.

Description of example programs:<br>

**RKnTest**: Solves a single 1st order ODE using the single equation RK4 solver and the ODE array solver

**RKnStep**: A basic example of the ODE array solver is applied to projectile motion with a simple model of air resistance, force of air resistance = -kv^2<br>. At each step in the elapsed time and x,y positions are printed.<br>
Optional parameters [default values]<br>
* -v initial_velocity [100] m/s
* -t angle_thera [45] degrees
* -m mass_of_projectile [10] kg
* -k coefficient_of_air_resistance [0.1] kg/m


**RKnDemo**: Solves for projectile motion with a simple model of air resistance, force of air resistance = -kv^2<br>
This program includes graphical output.  Detailed output is saved in TGraph objects in RKnDemo.root.  The file **RKnPlotDemo.py** shows how to access date in the TGraphs and can be used to generate additional plots.<br>
Optional parameters [default values]<br>
* -v initial_velocity [100] m/s
* -t angle_thera [45] degrees
* -m mass_of_projectile [10] kg
* -k coefficient_of_air_resistance [0.1] kg/m

**baseball1**:  Starter template for first baseball problem

**baseball2**:  Starter template for second baseball problem

**baseball_drag.ipynb**: this notebook describes the drag force equations used in the text.

gsl starter code
---

The starter code here (projGSL.cpp) demonstrates very basic usage of the gsl for solving a problem of coupled differential equations. An 8th order R-K solver with fixed step size is used. You are encouraged to try other solvers as you explore the problem. See here for the gsl docs: https://www.gnu.org/software/gsl/doc/html/ode-initval.html

This example solves the 2D projectile motion problem with a simple model for air resistance. After each step, data are stored in ROOT TGraphs, which are then displayed at the conclusion of the calculation.

The gsl provides a number of ODE solvers and a variety of interfaces.  Some of the solvers (not R-K methods) use the Jacobian matrix, which gives the devivative of the function wrt the dependent parameters.  See the gsl examples for details.

Python starter code
---

Two examples are given for using ODE solvers from the scipy.integrate sub-package in Python. In these examples graphs are made using matplotlib.

    Solution (projScPY2.py[ipynb]) using a more modern interface scipy.integrate.solve_ivp. See also: https://docs.scipy.org/doc/scipy/reference/tutorial/integrate.html and https://www.programcreek.com/python/example/119375/scipy.integrate.solve_ivp

    Solution (projScPY.py[ipynb]) using an older interface scipy.integrate.odeint¶ (see comments here: https://docs.scipy.org/doc/scipy/reference/integrate.html).  I do not recommend using this interface any longer.

The notebook versions contain additional comments on using the integrators.

# Part A

Please see vterm.pdf for the responses to parts 1-5. Here's a brief overview of how to compile and run the code. The necessary files are the following:

vterm.cpp, vterm_vs_mass.cpp, vterm_energy.cpp, vterm_air.cpp, plot_energy_vs_steps.py, and finally vterm_plot.py

The easiest way to compile the c++ programs is to run "make" followed by the name of the executable. For example, to compile "vterm.cpp", the command would be "make vterm". All these programs are making each part of the included PDF.

# Part B

## Exercise 1

Malinda wrote this!

Team member names and computing IDs: Malinda Amarakoon (zjm4us) Taylor Colaizzi (zxk4gs)

# Baseball Pitch Simulation

## Problem Description

This program calculates the initial speed required for a baseball to reach home plate within the strike zone, taking into account **air resistance** modeled as:

\[
F(v) = b v + c v^2
\]

for a spherical projectile (no spin), where the drag always points opposite to the velocity.  

### Parameters

- Diameter: `d = 0.075 m`  
- Mass: `m = 0.145 kg`  
- Drag coefficients:  
  - `b = 1.6 × 10^-4 [N·s/m²] × d`  
  - `c = 0.25 [N·s²/m⁴] × d²`  

- Initial conditions:  
  - `x(0) = 0 m`  
  - `y(0) = 0 m`  
  - `z(0) = 1.4 m`  
  - `vx(0) = v0 * cosθ0`  
  - `vy(0) = 0`  
  - `vz(0) = v0 * sinθ0`  
  - Launch angle: `θ0 = 1°`  

- Target at home plate:  
  - `xend = 18.5 m`  
  - `yend = 0 m`  
  - `zend = 0.9 m`  

The program uses a **bisection method** combined with an **RK4 solver** to find the required initial speed.

---

## How to Compile and Run

Compile:

```bash
g++ -std=c++20 -O2 -I$(root-config --incdir) baseball1.cpp RKn.cpp `root-config --cflags --libs` -o baseball1

run
./baseball1

Output
********************************
(xend,z0,theta0) = (18.500000,1.400000,1.000000)
v_pitch = 48.084208 m/s
********************************

This indicates that the baseball must be thrown at approximately 48.08 m/s to reach the strike zone under the specified conditions. This is roughly equivalent to 107 mph. (Taylor added this last sentence!)

run
./baseball1 -p

Output
********************************
(xend,z0,theta0) = (18.500000,1.400000,1.000000)
v_pitch = 48.084208 m/s
********************************
Info in <TCanvas::Print>: png file trajectory.png has been created
Trajectory plot saved as trajectory.png



Files
baseball1.cpp – main program
RKn.hpp / RKn.cpp – Runge-Kutta solvers
trajectory.png
