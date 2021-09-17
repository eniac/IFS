import matplotlib.pyplot as plt
import statistics


load = [300 * (i/10.0) * (1/5) for i in range(4, 11)]
private = list(map(lambda x: x / 3.5710, [0.34562958142565864,
            0.5103957160951634,
            0.8167798025264992,
            1.4798407937282267,
            1.9916492295382309,
            1.9586955376907418,
            2.006985058400152]))
            
public = list(map(lambda x: x / 3.5710, [1.1902834779130999e-05,
            3.6168602797355885e-05,
            0.00010215159772225694,
            0.0006044763927462833,
            15436.214612704225,
            41803.53567787001,
            62366.65165901151]))
            
throughput = list(map(lambda x: x / 10500.0, [6087.408126,
               6511.506054,
		6933.436232,
		7349.201968,
		7559.32386,
		7555.496314,
		7557.649776]))

plt.rcParams.update({'font.size': 15})

fig, ax1 = plt.subplots()
color = 'tab:purple'
ax1.set_xlabel('Incoming Rate Per Indifferent Client (Mbps)')
ax1.set_ylabel('Time spent in queue (ms)', color=color)  
ax1.plot(load, private, color=color)
plt.text(22, .4, "private latency", color=color) 
ax1.plot(load, public, color='tab:blue')
ax1.axvline(x = 300 * (7.5/10.0) * (1/5), linestyle='--')
plt.text(21, .00005, "indifferent latency", color='tab:blue') 
plt.text(36.7, .0000014, "indifferent  rate limit", color='tab:blue') 
ax1.tick_params(axis='y', labelcolor=color)
ax1.set_xlim(20, 60)
ax1.set_yscale('log')
ax1.set_ylim(0, 500)
ax2 = ax1.twinx()
color = 'k'
ax2.set_ylabel('Throughput (% of max)', color=color)
ax2.plot(load, throughput, color=color)
ax2.set_ylim(.4, 1)
plt.text(50, .67, "throughput", color=color)
ax2.tick_params(axis='y', labelcolor=color)
#ax2.set_ylim(0, 1)

fig.tight_layout()  # otherwise the right y-label is slightly clipped
fig.set_size_inches(8, 3.25)
plt.gcf().subplots_adjust(bottom=0.2)
#plt.show()
plt.savefig('pfs_limited_load_fixed_private_log.eps', format='eps')
