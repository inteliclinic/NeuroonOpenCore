import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

full_staging_df = pd.read_csv('./functional_test_results/full_offline_staging.csv', header=None)

full_staging_df.loc[full_staging_df[0] == -1, 0] = np.NaN
full_staging_df = full_staging_df[~pd.isnull(full_staging_df[0])]

fig = plt.figure(figsize=(18,5))
plt.plot(full_staging_df.index, full_staging_df[0], 'k', linewidth=3)
plt.grid()
plt.ylim(-1,5)
plt.title('Full offline staging for TG_190616')

plt.savefig('./full_offline_staging.png')