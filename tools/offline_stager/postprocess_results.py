import matplotlib.pyplot as plt
import pandas as pd
import sys
import numpy as np

directory = sys.argv[1]


plt.rcParams['image.cmap'] = 'gnuplot'

eeg = pd.read_csv(directory + '/eeg.csv', header=None)
ir = pd.read_csv(directory + '/ir.csv', header=None)

staging = pd.read_csv( directory + '/staging.csv', header=None)
staging.loc[staging[0] == -1, 0] = np.NaN
staging = staging[~pd.isnull(staging[0])]

fig, axarr = plt.subplots(3,1, sharex=True, figsize=(30,15))
fig.suptitle(directory + ' staging plot', fontsize=30)

print(staging.columns)
axarr[0].plot(staging.index, staging[0], 'k', linewidth=5)
axarr[0].grid()
axarr[0].set_ylim(-1,5)
axarr[0].set_title('Staging')

axarr[1].pcolormesh(eeg.T, vmax=10**6, vmin=10**5)
axarr[1].set_ylim(0, 1200)
axarr[1].set_title('EEG Spectrum')

axarr[2].pcolormesh(ir.T, vmin=10**4, vmax=3 * 10**5)
axarr[2].set_ylim(0, 200)
axarr[2].set_title('IR Spectrum')
axarr[2].set_xlabel('epoch')

plt.savefig(directory + '/plot.png')
