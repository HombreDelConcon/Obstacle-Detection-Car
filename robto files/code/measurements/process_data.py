import math

platform_distance_length = 9.96
platform_distance_width = 4.91

us = """10.06
9.96
10.03
10.06
9.96
10.06
9.96
9.96
10.06
9.96
9.96
10.06
9.95
10.06
9.96
9.96
10.06
9.95
9.96
10.06
9.96
10.06
10.06
9.96
10.59"""

gyp = """4
4
4
4
4
4
5
6
5
4
3
4
4
4
4
5
4
4
5
5
5
5
5
4
5"""

tof = """6.90
7.00
7.10
6.70
7.20
7.00
6.70
7.00
7.20
7.10
7.10
7.20
6.80
7.20
6.60
6.90
6.80
7.00
6.90
6.90
7.00
6.60
7.10
6.90
7.20"""

tof2 = """14.30
14.20
14.60
14.50
14.00
14.40
14.40
14.30
14.30
14.10
14.30
14.50
14.40
14.40
14.50
14.20
14.80
14.30
14.60
14.30
14.40
14.60
14.40
14.40
14.30"""

def process_string(string:str, isLength:bool) -> list[int]:
    above_count = 0
    below_count = 0
    accurate = 0
    c = 4.91
    if isLength:
        c = 9.96

    s = string.split("\n")
    processed_vals =  [float(item)for item in s]
    
    mean = sum(processed_vals) / len(processed_vals)
    max_val = max(processed_vals)
    min_val = min(processed_vals)
    
    #Calc stdev
    temp_vals = [(x - mean)**2 for x in processed_vals]
    stdev = math.sqrt(sum(temp_vals) / len(temp_vals))

    for v in processed_vals:
        if v > c:
            above_count += 1
        elif v < c:
            below_count += 1
        else:
            accurate += 1

    print("Mean: %.2f \nMax: %.2f \nMin: %.2f \nStandard Deviation: %.2f \nSamples Above: %d \nSamples Below: %d \nPercent Accuracy: %.2f%%" % \
          (mean, max_val, min_val, stdev, above_count, below_count, accurate/len(processed_vals) * 100))

if __name__ == "__main__":
    process_string(tof2, True)