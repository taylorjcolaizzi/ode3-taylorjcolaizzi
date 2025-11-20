#!/usr/bin/env python
"""
plot_terminal_velocity_speed.py

Plot projectile speed |v| vs time and mark both numerical and analytical terminal velocities.
"""

import ROOT as r
from math import sqrt

# --- open ROOT file ---
tf = r.TFile("RKnDemo.root")  # use the file created by your vterm demo
tg_vx = tf.Get("xy1")         # vx vs t
tg_vy = tf.Get("xy3")         # vy vs t

N = tg_vx.GetN()
time = [tg_vx.GetX()[i] for i in range(N)]
vx = [tg_vx.GetY()[i] for i in range(N)]
vy = [tg_vy.GetY()[i] for i in range(N)]

# --- compute speed magnitude ---
speed = [sqrt(vx[i]**2 + vy[i]**2) for i in range(N)]

# --- analytical terminal velocity ---
m = 10.0        # default mass in kg
g = 9.81        # gravity
k = 0.1         # air resistance coefficient
vt_analytic = sqrt(m*g/k)

# --- numerical terminal velocity (average last 10% of points) ---
start = int(0.9*N)
vt_numerical = sum(speed[start:N]) / (N - start)

# --- make TGraph for speed ---
tg_speed = r.TGraph(N)
for i in range(N):
    tg_speed.SetPoint(i, time[i], speed[i])

# --- canvas ---
c = r.TCanvas("c","Projectile Speed Terminal Velocity",800,600)
tg_speed.SetLineColor(r.kBlue)
tg_speed.SetLineWidth(2)
tg_speed.SetTitle("Projectile speed |v| vs time; t [s]; |v| [m/s]")
tg_speed.Draw("AL")

# --- horizontal lines for vt ---
line_analytical = r.TLine(time[0], vt_analytic, time[-1], vt_analytic)
line_analytical.SetLineColor(r.kRed)
line_analytical.SetLineStyle(2)
line_analytical.SetLineWidth(2)
line_analytical.Draw("same")

line_numerical = r.TLine(time[0], vt_numerical, time[-1], vt_numerical)
line_numerical.SetLineColor(r.kGreen+2)
line_numerical.SetLineStyle(3)
line_numerical.SetLineWidth(2)
line_numerical.Draw("same")

# --- legend ---
leg = r.TLegend(0.6,0.7,0.88,0.88)
leg.AddEntry(tg_speed,"|v| vs t","l")
leg.AddEntry(line_analytical,"Analytical v_t","l")
leg.AddEntry(line_numerical,"Numerical v_t","l")
leg.Draw()

# --- annotate values ---
txt = r.TLatex()
txt.SetTextSize(0.035)
txt.DrawLatexNDC(0.15,0.85,f"v_t numerical = {vt_numerical:.3f} m/s")
txt.DrawLatexNDC(0.15,0.80,f"v_t analytical = {vt_analytic:.3f} m/s")

# --- save plot ---
c.Update()
c.Print("projectile_speed_terminal_velocity.png")
print("Plot saved as projectile_speed_terminal_velocity.png")

