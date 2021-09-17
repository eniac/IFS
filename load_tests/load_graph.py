import matplotlib.pyplot as plt
import statistics


load = [3 * (i/10.0) * (1/5) for i in range(4, 27)]
private = list(map(lambda x: x / 3.5710, [0.0471510366475176,
            0.12607542297694735,
            0.3436347449409513,
            1.4995693371693093,
            4099.821217608652,
            20802.233932573]))
            
public = list(map(lambda x: x / 3.5710, [0,
            6.662713456682368e-06,
            7.784184285002816e-05,
            0.0003471886568700466,
            0.0008803540476630114,
            0.0018470033798257725,
            0.0034402730895251588,
            0.006006580739192203,
            0.009793730280763399,
            0.015487127373257853,
            0.023000907093361205,
            0.032986738831969654,
            0.04612333605968934,
            0.06394196142037366,
            0.08709402932450172,
            0.11762005007754874,
            0.16152060741245874,
            0.22674501174041767,
            0.33414454929710596,
            0.5325054489429745,
            1.1342452331902384,
            127.79403929302428,
            9642.177884695056]))
            
throughput = list(map(lambda x: x / 10500.0, [4197.758086,
               5247.231444,
               6301.973974,
               7351.901836,
               8065.009854,
               8206.288536,
               8350.66588,
               8492.589678,
               8635.921754,
               8781.139736,
               8924.318184,
               9064.810642,
               9209.35427,
               9352.58323,
               9497.04679,
               9639.694824,
               9783.101306,
               9927.157116,
               10068.220854,
               10214.031748,
               10355.192902,
               10497.985868,
               10499.936132]))







        


            
            

plt.rcParams.update({'font.size': 15})

fig, ax1 = plt.subplots()
color = 'tab:purple'
ax1.set_xlabel('Per Client Rate (Mbps)')
ax1.set_ylabel('Time spent in queue (ms)', color=color)  
ax1.plot(load[:len(private)], private, color=color)
plt.text(.445, 5, "private latency", color=color) 
ax1.plot(load, public, color='tab:blue')
plt.text(.95, 20, "indifferent latency", color='tab:blue') 
ax1.tick_params(axis='y', labelcolor=color)
#ax1.set_xlim(0, 1000)
#ax1.set_yscale('log')
ax1.set_ylim(0, 50)
ax2 = ax1.twinx()
color = 'k'
ax2.set_ylabel('Throughput (% of max)', color=color)
ax2.plot(load, throughput, color=color)
plt.text(.7, .76, "throughput", color=color)
ax2.tick_params(axis='y', labelcolor=color)
#ax2.set_ylim(0, 1)

fig.tight_layout()  # otherwise the right y-label is slightly clipped
fig.set_size_inches(8, 3.25)
plt.gcf().subplots_adjust(bottom=0.2)
plt.show()
#plt.savefig('pfs_unlimited_load2.eps', format='eps')
