import numpy as np

clients = 5
capacity = 10500 
rate = capacity * (4/10)
size = 1000
per_client_rate = rate / (clients * size)

packets = []

for slot in range(0, 500000):
    for source in range(1, 6):
        for packet in range(np.random.poisson(per_client_rate)):
            packets.append([slot, source, np.random.poisson(size)])
            

out = open("load.in", "w")

for packet in packets:
    out.write('{} {} {}\n'.format(packet[0], packet[1], packet[2]))
out.close()  
