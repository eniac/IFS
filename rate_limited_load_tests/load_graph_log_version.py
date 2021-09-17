import matplotlib.pyplot as plt
import statistics


load = [300 * (i/10.0) * (1/5) for i in range(4, 11)]
private = list(map(lambda x: x / 3.5710, [0.04715765554187348,
            0.12790490934282597,
            0.33820863618516117,
            1.4942551774145056,
            4265.000844311308,
            10999.709058397248,
	21516.429446471662]))
            
public = list(map(lambda x: x / 3.5710, [1.1891341676298654e-05,
            1.6172316988828734e-05,
            0.00012463265121433483,
            0.0003748717543998106,
            15642.328950000032,
            41726.18908962304,
	62662.51529298971]))
            
throughput = list(map(lambda x: x / 10500.0, [4202.692424,
               5251.961904,
		6293.94905,
		7349.989454,
		7991.624218,
		7991.838476,
		7991.802026]))

plt.rcParams.update({'font.size': 15})

fig, ax1 = plt.subplots()
color = 'tab:purple'
ax1.set_xlabel('Incoming Rate Per Client (Mbps)')
ax1.set_ylabel('Time spent in queue (ms)', color=color)  
ax1.plot(load, private, color=color)
plt.text(22, .4, "private latency", color=color) 
ax1.plot(load, public, color='tab:blue')
ax1.axvline(x = 300 * (7.5/10.0) * (1/5), linestyle='--')
plt.text(21, .0001, "indifferent latency", color='tab:blue') 
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
plt.text(50, .78, "throughput", color=color)
ax2.tick_params(axis='y', labelcolor=color)
#ax2.set_ylim(0, 1)

fig.tight_layout()  # otherwise the right y-label is slightly clipped
fig.set_size_inches(8, 3.25)
plt.gcf().subplots_adjust(bottom=0.2)
#plt.show()
plt.savefig('pfs_limited_load_log.eps', format='eps')
