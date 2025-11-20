#!/usr/bin/env python
"""
energy_step_analysis_combined.py

Analyze energy conservation for projectile motion solved with RK4.
Generates a single PNG containing:
1) Max energy deviation vs step size plot
2) Energy Conservation Analysis text
"""

import ROOT as r
import glob
from math import sqrt
import matplotlib.pyplot as plt
import re

# --- define mass and gravity ---
m = 1.0
g = 9.81

# --- list ROOT files ---
files = glob.glob("vterm_nsteps*.root")

step_sizes = []
max_devs = []
nsteps_list = []

for f in files:
    tf = r.TFile(f)
    if tf.IsZombie():
        print(f"File {f} not found or corrupted, skipping")
        continue

    tg_x_vs_t  = tf.Get("xy0")
    tg_vx_vs_t = tf.Get("xy1")
    tg_y_vs_t  = tf.Get("xy2")
    tg_vy_vs_t = tf.Get("xy3")

    xval   = tg_x_vs_t.GetY()
    height = tg_y_vs_t.GetY()
    vx     = tg_vx_vs_t.GetY()
    vy     = tg_vy_vs_t.GetY()
    nvals  = tg_x_vs_t.GetN()

    # extract nsteps from filename
    m_steps = re.search(r"nsteps(\d+)", f)
    if m_steps:
        nsteps = int(m_steps.group(1))
    else:
        nsteps = nvals
    h = 20.0 / nsteps  # step size, xmax=20 s

    # compute max energy deviation
    e0 = 0
    deltaE = []
    for i in range(nvals):
        v2 = vx[i]**2 + vy[i]**2
        E = 0.5*m*v2 + m*g*height[i]
        if i == 0: e0 = E
        deltaE.append(E - e0)

    max_dev = max(abs(d) for d in deltaE)
    step_sizes.append(h)
    max_devs.append(max_dev)
    nsteps_list.append(nsteps)

# --- sort all data by step size ---
sorted_data = sorted(zip(step_sizes, max_devs, nsteps_list))
step_sizes_sorted, max_devs_sorted, nsteps_sorted = zip(*sorted_data)

# --- compute summary statistics ---
best_h = step_sizes_sorted[-1]   # smallest step size
best_dev = max_devs_sorted[-1]
worst_h = step_sizes_sorted[0]   # largest step size
worst_dev = max_devs_sorted[0]

# --- create a figure with two subplots ---
fig, (ax_plot, ax_text) = plt.subplots(2, 1, figsize=(10,12), gridspec_kw={'height_ratios':[3,1]})

# --- Subplot 1: log-log plot ---
ax_plot.loglog(step_sizes_sorted, max_devs_sorted, 'o-', markersize=8)
ax_plot.set_xlabel("RK4 step size h [s]")
ax_plot.set_ylabel("Max |E - E0| [J]")
ax_plot.set_title("Energy Conservation vs RK4 Step Size")
ax_plot.grid(True, which="both", ls="--")

# annotate points with nsteps
for h, dev, n in zip(step_sizes_sorted, max_devs_sorted, nsteps_sorted):
    ax_plot.text(h*1.05, dev*1.05, str(n), fontsize=9)

# --- Subplot 2: textual analysis ---
textstr = f"""--- Energy Conservation Analysis ---
Step sizes ranged from {worst_h:.4f}s to {best_h:.4f}s
Maximum energy deviation with largest step: {worst_dev:.6f} J
Maximum energy deviation with smallest step: {best_dev:.6f} J
Observation:
- Energy is very well conserved for small step sizes.
- Larger step sizes produce larger numerical deviations.
- Reducing RK4 step size improves accuracy, consistent with 4th-order convergence.
"""
ax_text.text(0.05, 0.95, textstr, transform=ax_text.transAxes,
             fontsize=12, verticalalignment='top')
ax_text.axis('off')  # hide axes for text

plt.tight_layout()
plt.savefig("energy_analysis_combined.png", dpi=300)
print("Combined plot saved as energy_analysis_combined.png")
plt.show()

