import math

x = 96; y = 32; r = 30;

for i in range(30):
    px = r * math.cos(math.pi * (i / 180))
    py = r * math.sin(math.pi * (i / 180))
    print(int(px) + x, int(py) + y)
