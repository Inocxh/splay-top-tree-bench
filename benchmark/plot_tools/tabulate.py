import json
import sys
import pandas as pd

RUNTIME = "Running time ($ms$)"

def main():
    usage = ["mst", "mst-ogbl", "connec", "all"]

    if len(sys.argv) != 2 or (sys.argv[1] not in usage):
        exit(f"Usage {sys.argv[0]} <{'|'.join(usage)}>")
    
    to_process = []
    if sys.argv[1] == "all":
        to_process = ["mst", "mst-ogbl", "connec"]
    else:
        to_process = [sys.argv[1]]

    pd.set_option('colheader_justify', 'center')
    for cat in to_process:
        
        splay_top_tree_df = pd.read_json(f"results/{cat}/splay-top-tree.jsonl", lines=True, orient="records")
        splay_top_tree_df = splay_top_tree_df.rename(columns={"median": "time_ns"})
        df = pd.concat([splay_top_tree_df['num_vertices'],splay_top_tree_df['num_edges'], splay_top_tree_df['name'], splay_top_tree_df['time_ns']], axis=1)
        
        num_vertices = df['num_vertices'].unique()
        # if cat == "connec":
        #     sttc_df = pd.read_json(f"results/{cat}/stt-c.jsonl", lines=True)

        #     df = pd.concat( [df, sttc_df['time_ns']], axis=1)
        

        sttrs_df = pd.read_json(f"results/{cat}/stt-rs.jsonl", lines=True)
        sttrs_df.index = [0] * len(sttrs_df)

        df = pd.concat([sttrs_df, df]).reset_index(drop=True)
        df = df.pivot(index='name', columns='num_vertices', values='time_ns').div(1000*1000).reset_index()
        df = df.set_axis([[""] + [RUNTIME] * len(num_vertices),["Algorithm"] + num_vertices.tolist()], axis=1)
        with open(f"plots/table_{cat}.tex", 'w') as tf:
            tf.write(df.to_latex(index=False, escape=False, float_format="%.2f"))
        
if __name__ == "__main__":
    main()