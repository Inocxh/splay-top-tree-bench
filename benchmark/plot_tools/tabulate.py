import json
import sys
import pandas as pd

def main():
    usage = ["mst", "mst_ogbl", "connec", "all"]

    if len(sys.argv) != 2 or (sys.argv[1] not in usage):
        exit(f"Usage {sys.argv[0]} <{"|".join(usage)}>")
    
    to_process = []
    if sys.argv[1] == "all":
        to_process = ["mst", "mst_ogbl", "connec"]
    else:
        to_process = [sys.argv[1]]

    for cat in to_process:
        splay_top_tree_df = pd.read_json(f"results/{cat}/splay-top-tree.jsonl", lines=True)
        sttc_df = pd.read_json(f"results/{cat}/stt-c.jsonl", lines=True)


        sttrs_df = pd.read_json(f"results/{cat}/stt-rs.jsonl", lines=True)

        df = pd.concat([splay_top_tree_df['num_vertices'], splay_top_tree_df['num_edges'], splay_top_tree_df['median'], sttc_df['median'], sttrs_df['median']], axis=1)
        with open(f"plots/table_{cat}.tex", 'w') as tf:
            tf.write(df.to_latex(header=["Vertices", "Edges", "Splay ($\\mu s$)", "STT-C ($\\mu s$)", "STT-RS ($\\mu s$)"], index=False, escape=False))

if __name__ == "__main__":
    main()