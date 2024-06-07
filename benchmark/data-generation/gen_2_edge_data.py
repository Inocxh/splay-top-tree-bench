
import sys
import random
import os 
os.chdir(os.path.dirname(os.path.abspath(__file__)))

p_insert = 0.80
p_delete = 0.10 + p_insert
p_query = 0.10 + p_delete

def main():
    if len(sys.argv) == 1:
        exit(f"Usage {sys.argv[0]} <test name> <sets> ")

    test_name = sys.argv[1]
    sets = int(sys.argv[2])
    sizes = [500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000]

    random.seed(1234)

    for n in sizes:
        m = 8 * n
        for set in range(sets):
            queries = []
            edges = 0
            for i in range(m):
                u = random.randrange(n)
                v = random.randrange(n - 1)
                if u <= v:
                    v += 1
                r = random.random();
                if r <= p_insert:
                    edges += 1
                    queries.append(f"i {u} {v}\n")
                elif r <= p_delete and edges > 0 :
                    edges -= 1
                    queries.append(f"d {random.randint(0,edges)}\n")
                elif r <= p_query:
                    queries.append(f"q {u} {v}\n")
                

            f = open( f"../dataset/2-edge-connectivity/{test_name}_{n}_{set}.txt", "w")
            f.write(f"{test_name} {n} {m}\n")
            f.writelines(queries)
    


if __name__ == "__main__":
    main()