import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

import sys
plt.rcParams['image.cmap'] = 'gnuplot'

spectrogram_file = sys.argv[1]
feature_file = sys.argv[2]

s = pd.read_csv(spectrogram_file, header=None)
s = s.apply(pd.to_numeric)
m = pd.read_csv(feature_file, header = None)

fig, axarr = plt.subplots(2,1, sharex=True, figsize=(18,15))
axarr[0].pcolormesh(s.T, vmin=0, vmax=np.max(np.max(s)) / 2)
axarr[0].set_title(spectrogram_file)

m5 = pd.read_csv(feature_file, header = None)
m5 = m5 * 60
axarr[1].plot(m5.rolling(1, center=True).median(), c='b', linewidth=1, label='original')
axarr[1].plot(m5.rolling(50, center=True).median(), c='k', linewidth=3, label='mean 50')
axarr[1].set_ylim(30, np.max(m5)[0])
axarr[1].set_title(feature_file)

axarr[1].legend()

plt.show()
