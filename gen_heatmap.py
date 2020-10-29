import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# Encodes inputs to integers for a group of runs
def encode_inputs(group):
    inp = "Input " + group["Arguments"].astype('category').cat.codes.astype(str)
    group["Input"] = inp

    return group

# Computes runtime/speedup for a given group of runs
def compute_data(group):
    sorted_group = group.sort_values(by=["Mean"])
    bench_name = sorted_group.iloc[0]["Name"]
    bench_input = sorted_group.iloc[0]["Input"]
    minval = sorted_group.iloc[0]['Mean']
    maxval = sorted_group.iloc[-1]['Mean']
    speedup = maxval/minval
    string = str(minval) + '(' + str(maxval) + ')'

    data = [[bench_name, bench_input, string, speedup]]
    df2 = pd.DataFrame({"Name":pd.Series([bench_name]),
                        "Input":pd.Series([bench_input]),
                        "Runtimes":[string],
                        "Speedup":pd.Series([speedup], dtype='float')})

    return df2

def gen_heatmap(input_csv, output_filename):
    df = pd.read_csv(input_csv)
    df = df[['Name', 'Permutation', 'Arguments', 'Mean']]
    df = df.round(decimals=2)
    df = df.groupby("Name", as_index=False).apply(encode_inputs)

    df = df.groupby(["Name", "Input"], as_index=False).apply(compute_data)
    runtimes = df.pivot(index="Name", columns="Input", values="Runtimes")
    speedups = df.pivot(index="Name", columns="Input", values="Speedup")

    plt.figure(figsize = (10,5))
    plot = sns.heatmap(speedups, annot=runtimes, fmt="s",
                   linewidths=.5, cmap="coolwarm")

    plt.xlabel('')
    plt.ylabel('Benchmark')

    plot.figure.tight_layout()
    plt.savefig(output_filename)

if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print("Usage: ", sys.argv[0], '<input_csv> <output_png>')
        sys.exit(1)

    input_csv = sys.argv[1]
    output_png = sys.argv[2]

    gen_heatmap(input_csv, output_png)
