import sys
import subprocess

def main():
    if len(sys.argv) == 1:
        exit(f"Usage {sys.argv[0]} <test name> <sets> ")


    SEED = 1234
    test_name = sys.argv[1]
    sets = int(sys.argv[2])
    sizes = [500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000]
    for n in sizes:
        m = 8 * n
        for set in range(sets):
            
            f = open(f"./data/{test_name}/{test_name}_{n}_{set}.txt", "w")
            f.write(f"Randomly generated edge insertions\n")
            f.write(f"{test_name} {n} {m}\n")
            f.close()
            # Must open and close file, otherwise the subprocess writes the data too fast and breaks the format :-( 
            f = open(f"./dataset/{test_name}/{test_name}_{n}_{set}.txt", "a")
            subprocess.call(["gen_queries", "--seed", f"{SEED}", "-n", f"{n}" ,"-q", f"{m}"],stdout=f);
    


if __name__ == "__main__":
    main()