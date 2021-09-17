import statistics
import numpy as np

f = open("load.out")
l = f.readlines()

l = list(map(lambda x: x.replace("value", ","), l))
l = list(map(lambda x: x.replace(" ", ""), l))
l = list(map(lambda x: x.replace("(", ""), l))
l = list(map(lambda x: x.replace(")", ""), l))
l = list(map(lambda x: x.replace("\n", ""), l))

rows = []
for row in l:
    r = {}
    for data in row.split(","):
         pair = data.split(":")
         if len(pair) == 2:
            r[pair[0]] = int(pair[1])
    if r.get("origSlot"):
        rows.append(r)

for row in rows:
    row["delay"] = row["tick"] - row["origSlot"]

privates = sorted(list(map(lambda x: x["delay"], filter(lambda x: x["class"] in [1,2,3] and x["isDummy"] == 0, rows))))
publics = sorted(list(map(lambda x: x["delay"], filter(lambda x: x["class"] in [5,6], rows))))

padded_throughput = sum(list(map(lambda x: int(x["size"] if x["privacy"] == 0 else 1518), filter(lambda x: x["isDummy"] == 0, rows))))
real_throughput = sum(list(map(lambda x: int(x["size"]), filter(lambda x: x["isDummy"] == 0, rows))))
dummy_throughput = sum(list(map(lambda x: 1518, filter(lambda x: x["isDummy"] == 1, rows))))

averages = [statistics.mean(list(map(lambda x: x["delay"], filter(lambda x: x["class"]==c and x["isDummy"] == 0, rows)))) for c in [1,2,3,5,6]]

throughputs = [sum(list(map(lambda x: int(x["size"]), filter(lambda x: x["class"]==c and x["isDummy"] == 0, rows)))) for c in [1,2,3,5,6]]

print(averages)
print(throughputs)

print(statistics.mean(privates))
print(statistics.mean(publics))
print("Throughput: " + str(real_throughput / 500000.0))
print("Padded Throughput: " + str(padded_throughput / 500000.0))
print("Dummy Throughput: " + str(dummy_throughput / 500000.0))
print(np.percentile(privates, 50))
print(np.percentile(privates, 99))
print(np.percentile(publics, 50))
print(np.percentile(publics, 99))




