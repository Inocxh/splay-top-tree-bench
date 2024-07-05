import sys
import pandas as pd
import matplotlib.pyplot as plt


def main():
    usage1 = ["mix", "query_only", "updates_only"]
    usage2 = ["semi", "full"]
    usage3 = ["d-trees", "stt-c", "stt-rs", "all"]
        
    if len(sys.argv) != 4 or sys.argv[1] not in usage1 or sys.argv[2] not in usage2 or sys.argv[3] not in usage3:
        exit(f"Usage: {sys.argv[0]} <{'|'.join(usage1)}> <{'|'.join(usage2)}> <{'|'.join(usage3)}>")
    
    impls = []
    if sys.argv[3] == "all":
        usage3.remove("all")
        impls = usage3
    else:
        impls = [sys.argv[3]]

    pd.set_option('colheader_justify', 'center')
    
    splay_top_tree_df = pd.read_json(f"results/connectivity/{sys.argv[1]}/splay-top-tree.jsonl", lines=True, orient="records")
    splay_top_tree_df = splay_top_tree_df.rename(columns={"median": "time_ns", "num_edges": "num_queries"})
    splay_top_tree_df = pd.concat([splay_top_tree_df['name'], splay_top_tree_df['num_vertices'], splay_top_tree_df['num_queries'], splay_top_tree_df['time_ns'], ], axis=1)
    splay_top_tree_df = splay_top_tree_df[(splay_top_tree_df['name'] == f"Splay top tree {sys.argv[2]} splay")]
    splay_top_tree_df['throughput'] = splay_top_tree_df[['num_queries']].div(splay_top_tree_df['time_ns'], axis= 0).mul(10**9)
    
    splay_top_tree_df = splay_top_tree_df.reset_index(drop=True)
    
    df = splay_top_tree_df
    for impl in impls:
        
        impl_df = pd.read_json(f"results/connectivity/{sys.argv[1]}/{impl}.jsonl", lines=True, orient="records")
        impl_df = impl_df.rename(columns={"median": "time_ns", "num_edges": "num_queries"})
        impl_df = pd.concat([impl_df['name'], impl_df['num_vertices'], impl_df['num_queries'], impl_df['time_ns'], ], axis=1)
        impl_df = impl_df.reset_index(drop=True)

        for ds in impl_df["name"].unique():
            filt_df = impl_df[(impl_df['name'] == ds)].reset_index(drop=True)
            filt_df['throughput'] = filt_df[['num_queries']].div(filt_df['time_ns'], axis= 0).mul(10**9)
            df = pd.concat( [df, filt_df]).reset_index(drop=True)
            
        
    
    df = df.pivot(index='num_vertices', columns='name', values='throughput').reset_index()
    print(df)
    df = df.set_index('num_vertices')
    df = df.drop([1000, 2000, 5000, 10000, 20000, 50000, 100000])
    df.plot.line(loglog=False, xlabel="Vertices", ylabel="Throughput [ops / s]")
    plt.show()
    #plt.savefig(f"plots/plot_throughput.pdf", dpi=300)

if __name__ == "__main__":
    main()