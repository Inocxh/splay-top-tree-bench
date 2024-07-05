import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker  

def main():
    pd.set_option('colheader_justify', 'center')

    print(f"results/connectivity/density/splay-top-tree.jsonl")
    splay_top_tree_df = pd.read_json(f"results/connectivity/density/splay-top-tree.jsonl", lines=True, orient="records")
    splay_top_tree_df = splay_top_tree_df.rename(columns={"median": "time_ns"})
    df = pd.concat([splay_top_tree_df['name'], splay_top_tree_df['num_vertices'], splay_top_tree_df['num_edges'], splay_top_tree_df['time_ns']], axis=1).reset_index(drop=True)
    
    sttrs_df = pd.read_json(f"results/connectivity/density/stt-rs.jsonl", lines=True)
    sttrs_df.index = [0] * len(sttrs_df)
    sttrs_df = sttrs_df[sttrs_df['name'] == "MTR"] 
    sttrs_df = sttrs_df.rename(columns={"num_queries": "num_edges"})
    sttrs_df = pd.concat([sttrs_df['name'], sttrs_df['num_vertices'], sttrs_df['num_edges'], sttrs_df['time_ns']], axis=1)
    df = pd.concat([sttrs_df, df]).reset_index(drop=True)

    sttc_df = pd.read_json(f"results/connectivity/density/stt-c.jsonl", lines=True)
    sttc_df = sttc_df.rename(columns={"median": "time_ns"})
    sttc_df = pd.concat([sttc_df['name'], sttc_df['num_vertices'], sttc_df['num_edges'], sttc_df['time_ns']], axis=1)
    df = pd.concat( [df, sttc_df]).reset_index(drop=True)

    # dtrees_df = pd.read_json(f"results/connectivity/density/d-trees.jsonl", lines=True)
    # dtrees_df = dtrees_df.rename(columns={"median": "time_ns"})
    # dtrees_df = pd.concat([dtrees_df['name'], dtrees_df['num_vertices'], dtrees_df['time_ns'], dtrees_df['num_edges']], axis=1)
    # df = pd.concat( [df, dtrees_df]).reset_index(drop=True)


    
    df['name'] = df['name'].replace('D-trees', 'D-Tree')
    df['name'] = df['name'].replace('stt-c', 'STT-c')
    df['name'] = df['name'].replace('Splay top tree full splay', 'TOP-f')
    df['name'] = df['name'].replace('Splay top tree semi splay', 'TOP-s')

    df['time_ns'] = df['time_ns'].div(10**3)


    df['avg_degree'] = df['num_edges'].div(df['num_vertices'])
    df['time_per_edge'] = df['time_ns'].div(df['num_edges'])
    print(df)
    df = df.pivot(index='avg_degree', columns='name', values='time_per_edge').reset_index()
    df = df.set_index('avg_degree')
    
    fig, ax = plt.subplots()
    styles = ["solid", "dotted", "dashed", (0, (1, 5)), "dashdot", (0, (1, 6))]

    for i, column in enumerate(df.columns):            
        df[column].plot.line(loglog=False, ax=ax, label=column, linestyle=styles[i], linewidth=1.6)
    ax.set_xlabel("Avg. Degree")
    ax.set_ylabel("Time per edge [us/edge]")
    ax.legend()

    plt.show()

        
if __name__ == "__main__":
    main()