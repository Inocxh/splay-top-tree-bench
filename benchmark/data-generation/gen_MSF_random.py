import sys
import random

def main():
    if len(sys.argv) == 1:
        exit(f"Usage {sys.argv[0]} <test name> <sets> ")

    test_name = sys.argv[1]
    sets = int(sys.argv[2])
    sizes = [1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000]
    edge_weight = 1000
    for n in sizes:
        m = 8 * n
        for set in range(sets):
            queries = []
            for _ in range(m):
                u = random.randrange(n)
                v = random.randrange(n - 1)
                if u <= v:
                    v += 1
                w = random.randrange(edge_weight)
                queries.append(f"e {u} {v} {w}\n")

            f = open(f"./dataset/{test_name}/{test_name}_{n}_{set}.txt", "w")
            f.write(f"Randomly gemerated edge insertions\n")
            f.write(f"{test_name} {n} {m}\n")
            f.writelines(queries)
    


if __name__ == "__main__":
    main()