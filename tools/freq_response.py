#!/usr/bin/env python3
"""Plot filter frequency response from Corrode DSP test output.

Usage:
    python freq_response.py [input_file]

Reads a CSV file with columns: frequency_hz, magnitude_db
Produces a frequency response plot.
"""

import sys
import numpy as np
import matplotlib.pyplot as plt


def plot_response(filename: str = "filter_response.csv"):
    data = np.loadtxt(filename, delimiter=",", skiprows=1)
    freq = data[:, 0]
    mag = data[:, 1]

    fig, ax = plt.subplots(figsize=(10, 6))
    ax.semilogx(freq, mag, linewidth=1.5)
    ax.set_xlabel("Frequency (Hz)")
    ax.set_ylabel("Magnitude (dB)")
    ax.set_title("Corrode — TeeBeeFilter Frequency Response")
    ax.set_xlim(20, 20000)
    ax.set_ylim(-60, 10)
    ax.grid(True, which="both", alpha=0.3)
    ax.axhline(y=-3, color="r", linestyle="--", alpha=0.5, label="-3 dB")
    ax.legend()

    plt.tight_layout()
    plt.savefig("freq_response.png", dpi=150)
    plt.show()
    print(f"Saved to freq_response.png")


if __name__ == "__main__":
    filename = sys.argv[1] if len(sys.argv) > 1 else "filter_response.csv"
    plot_response(filename)
