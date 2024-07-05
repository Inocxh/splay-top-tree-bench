import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker  

def main():
    pd.set_option('colheader_justify', 'center')

    splay_top_tree_df = pd.read_json(f"results/2-edge-connectivity/splay-top-tree.jsonl", lines=True, orient="records")
    splay_top_tree_df = splay_top_tree_df.rename(columns={"median": "time_ns"})

    df = pd.concat([splay_top_tree_df['name'], splay_top_tree_df['num_vertices'], splay_top_tree_df['num_edges'], splay_top_tree_df['time_ns']], axis=1)
    df['name'] = df['name'].replace(' splay top tree 2edge full splay ', 'TOP-f')
    df['name'] = df['name'].replace(' splay top tree 2edge semi splay ', 'TOP-s')
    df['time_ns'] = df['time_ns'].div(10 ** 9)
    
    df = df.reset_index().pivot(index='num_vertices', columns='name', values='time_ns').reset_index()
    df = df.set_index('num_vertices')
    fig, ax = plt.subplots()
    styles = ["dashed", "dotted"]

    for i, column in enumerate(df.columns):            
        df[column].plot.line(loglog=False, ax=ax, label=column, linestyle=styles[i], linewidth=1.6)
    ax.set_xlabel("Vertices")
    ax.set_ylabel("Time [s]")
    ax.legend()

    #plt.show()
    plt.savefig(f"plots/plot_2_edge.pdf", dpi=300, bbox_inches='tight')

    
if __name__ == "__main__":
    main()