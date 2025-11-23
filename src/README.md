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

This indicates that the baseball must be thrown at approximately 48.08 m/s to reach the strike zone under the specified conditions.

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
