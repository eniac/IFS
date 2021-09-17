import numpy as np
from collections import deque

clients = 5
capacity = 10500 
rate = capacity * (10/10)
size = 1000
per_client_rate = rate / clients
per_client_packet_rate = per_client_rate / size
private_rate = capacity * (7/10) / (clients * size)
public_rate_limit = (capacity * (7.5/10)) / clients
token_cap = public_rate_limit * 5
packets = []

private = [2,3,4]
public = [1,5]
tokens = [0,0]
public_queues = [deque([]),deque([])]

for slot in range(0, 500000):
    for source in private:
        for packet in range(np.random.poisson(private_rate)):
            packets.append([slot, source, np.random.poisson(size), slot])
    #print("TOKENS: " + str(tokens))
    #print("PACKETS: " + str(list(map(lambda x: len(x), public_queues))))
    for index, source in enumerate(public):
        for packet in range(np.random.poisson(per_client_packet_rate)):
            public_queues[index].append([slot, source, np.random.poisson(size), slot])
        while public_queues[index] and public_queues[index][0][2] <= tokens[index]:
            packet = public_queues[index].popleft()
            tokens[index] -= packet[2]
            packet[0] = slot
            #print(str(packet[0] - packet[3]))
            packets.append(packet)
        #print("Next packet is " + str(public_queues[index][0][2]) + " bytes")
        tokens[index] += public_rate_limit % token_cap

out = open("load.in", "w")

for packet in packets:
    out.write('{} {} {} {}\n'.format(packet[0], packet[1], packet[2], packet[3]))
out.close()  
