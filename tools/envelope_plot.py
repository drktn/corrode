#!/usr/bin/env python3
"""Visualize Corrode envelope shapes.

Usage:
    python envelope_plot.py [input_file]
"""

import sys
import numpy as np
import matplotlib.pyplot as plt


def plot_envelope(filename: str = "envelope_data.csv"):
    data = np.loadtxt(filename, delimiter=",", skiprows=1)
    time_ms = data[:, 0]
    amplitude = data[:, 1]

    fig, ax = plt.subplots(figsize=(10, 4))
    ax.plot(time_ms, amplitude, linewidth=1.5)
    ax.set_xlabel("Time (ms)")
    ax.set_ylabel("Amplitude")
    ax.set_title("Corrode — Envelope Shape")
    ax.set_ylim(-0.05, 1.1)
    ax.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig("envelope_plot.png", dpi=150)
    plt.show()


if __name__ == "__main__":
    filename = sys.argv[1] if len(sys.argv) > 1 else "envelope_data.csv"
    plot_envelope(filename)
