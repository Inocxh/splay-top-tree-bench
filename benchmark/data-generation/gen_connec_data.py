import sys
import subprocess

def main():
    if len(sys.argv) == 1:
        exit(f"Usage {sys.argv[0]} <test name> <sets> ")


    SEED = 1234
    test_name = sys.argv[1]
    sets = int(sys.argv[2])
    sizes = [1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000]
    #generate mixed workload
    for n in sizes:
        m = 8 * n
        for set in range(sets):
            f = open(f"../dataset/connectivity/mix/{test_name}_{n}_{set}.txt", "w")
            f.write(f"{test_name} {n} {m}\n")
            f.close()
            # Must open and close file, otherwise the subprocess writes the data too fast and breaks the format :-( 
            f = open(f"../dataset/connectivity/mix/{test_name}_{n}_{set}.txt", "a")
            subprocess.call(["bin/gen_queries", "--seed", f"{SEED}", "-n", f"{n}" ,"-q", f"{m}"],stdout=f);
    #generate only inserts and queries.
    for n in sizes:
        m = 8 * n
        for set in range(sets):
            f = open(f"../dataset/connectivity/query_only/{test_name}_{n}_{set}.txt", "w")
            f.write(f"{test_name} {n} {m}\n")
            f.close()
            # Must open and close file, otherwise the subprocess writes the data too fast and breaks the format :-( 
            f = open(f"../dataset/connectivity/query_only/{test_name}_{n}_{set}.txt", "a")
            subprocess.call(["bin/gen_queries", "--seed", f"{SEED}", "-p", "1.0", "-n", f"{n}" ,"-q", f"{m}"],stdout=f);

    #generate only inserts and deletes.
        for n in sizes:
            m = 8 * n
            for set in range(sets):
                f = open(f"../dataset/connectivity/update_only/{test_name}_{n}_{set}.txt", "w")
                f.write(f"{test_name} {n} {m}\n")
                f.close()
                # Must open and close file, otherwise the subprocess writes the data too fast and breaks the format :-( 
                f = open(f"../dataset/connectivity/update_only/{test_name}_{n}_{set}.txt", "a")
                subprocess.call(["bin/gen_queries", "--seed", f"{SEED}", "-p", "0.0", "-n", f"{n}" ,"-q", f"{m}"],stdout=f);


if __name__ == "__main__":
    main()