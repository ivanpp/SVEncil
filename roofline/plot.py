import matplotlib.pyplot as plt
import numpy as np

# DDR Bandwidth (GB/s)
bw = 13

# Peak performance (GFlops/s)
peak_perf = 128

# Define operational intensity range (Flops/Byte)
ai = np.logspace(-2, 2, num=100)

# Define performance bounds
roofline = np.minimum(ai * bw, peak_perf)

# Define Performance bound
perf_bound = np.ones_like(ai) * peak_perf

# Plot the roofline
plt.plot(ai, perf_bound, label='Peak Performance', color='red')
plt.plot(ai, roofline, label='Roofline', color='blue')

# Mark the point in the roofline with AI = 0.44
ai_p = 0.44
perf = min(ai_p * bw, peak_perf)
plt.plot(ai_p, perf, 'ro', label='Stencil-3d on the Roofline', color='green')

# Add label to roofline
bw_label = 'DDR Bandwidth: {} GB/s'.format(bw)
plt.annotate(bw_label, xy=(ai[52], bw), xytext=(ai[53], bw*1.2),
             ha='left', va='bottom', fontsize='small',
             arrowprops=dict(arrowstyle='->'))

# Add label to perf_bound line
perf_bound_label = 'Peak Performance: {} GFlops/s'.format(peak_perf)
plt.annotate(perf_bound_label, xy=(ai[0], peak_perf), xytext=(ai[1], peak_perf*1.2),
             ha='left', va='bottom', fontsize='small',
             arrowprops=dict(arrowstyle='->'))

# Add labels and legend
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Arithmetic Intensity (Flops/Byte)')
plt.ylabel('Performance (GFlops/s)')
plt.title('Simplified (DRAM) Roofline')
plt.legend()

# Set the limits of the y-axis
plt.ylim(1, 200)

# Show the plot
plt.show()
