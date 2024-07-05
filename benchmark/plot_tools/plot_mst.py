import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker  

def main():

    pd.set_option('colheader_justify', 'center')
    
    print(f"results/mst/splay-top-tree.jsonl")
    splay_top_tree_df = pd.read_json(f"results/mst/splay-top-tree.jsonl", lines=True, orient="records")
    splay_top_tree_df = splay_top_tree_df.rename(columns={"median": "time_ns"})
    df = pd.concat([splay_top_tree_df['num_vertices'],splay_top_tree_df['num_edges'], splay_top_tree_df['name'], splay_top_tree_df['time_ns']], axis=1)
    

    sttrs_df = pd.read_json(f"results/mst/stt-rs.jsonl", lines=True)
    sttrs_df.index = [0] * len(sttrs_df)
    sttrs_df = sttrs_df[(sttrs_df['name'] == "MTR") | (sttrs_df['name'] == "Link-cut")] 
    sttrs_df = sttrs_df.rename(columns={"num_queries": "num_edges"})
    df = pd.concat([sttrs_df, df]).reset_index(drop=True)

    df['name'] = df['name'].replace('Link-cut', 'LC')
    df['name'] = df['name'].replace('Splay top tree full splay', 'TOP-f')
    df['name'] = df['name'].replace('Splay top tree semi splay', 'TOP-s')

    df['time_ns'] = df['time_ns'].div(10**9)
    
    df = df.pivot(index='num_vertices', columns='name', values='time_ns').reset_index()
    df = df.set_index('num_vertices')
    

    
    
    
    fig, ax = plt.subplots()
    styles = ["solid", "dotted", "dashed", (0, (1, 5)), "dashdot", (0, (1, 6))]

    for i, column in enumerate(df.columns):            
        df[column].plot.line(loglog=False, ax=ax, label=column, linestyle=styles[i], linewidth=1.6)
    ax.set_xlabel("Vertices")
    ax.set_ylabel("Time [s]")
    ax.legend()

    #plt.show()
    plt.savefig(f"plots/plot_mst.pdf", dpi=300, bbox_inches='tight')

        
if __name__ == "__main__":
    main()