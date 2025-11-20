#!/usr/bin/env python
"""
combine_pngs_to_pdf_ordered_fixed_size.py

Combine PNGs into a single PDF in a specific order with all pages the same size.
"""

from PIL import Image

# --- specify PNG files in desired order ---
png_files = [
    "Projectile.png",
    "vterm_plots.png",                     
    "energy_analysis_combined.png",       
    "projectile_speed_terminal_velocity.png",  
    "vt_vs_mass.png",                      
    "accuracy_report.png"           
]

# --- open first image to get reference size ---
first_img = Image.open(png_files[0]).convert("RGB")
width, height = first_img.size

# --- open and resize all images to the same size ---
images = [first_img]  # first image already loaded
for f in png_files[1:]:
    img = Image.open(f).convert("RGB")
    img_resized = img.resize((width, height))
    images.append(img_resized)

# --- save as single PDF ---
images[0].save("vterm.pdf", save_all=True, append_images=images[1:])

print(f"Combined {len(images)} PNGs into vterm.pdf with uniform page size ({width}x{height})")

