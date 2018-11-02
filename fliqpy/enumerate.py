#libc = 0x00007fb8eab66000
libc_base = 0x00007f0000000000
offset = 3973120

rce = 0xf0274
dl = 0x17870

import random

d = dict()
for i in range(10000):
    r = random.randint(0, 0xfffffff) * 0x1000
    libc = libc_base + r
    ld = libc + offset

    x = libc + rce
    y = ld + dl

    s = bin(x^y)[2:]
    z = d.setdefault(s, 0)
    d[s] = z + 1


for k, v in sorted(d.items(), key=lambda x: -x[1]):
    print(str(k) + ": " + str(v))




