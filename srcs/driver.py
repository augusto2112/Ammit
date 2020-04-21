from sys import argv, exit
from itertools import permutations, product
from pathlib import Path
from subprocess import run
from csv import writer, reader
from os import walk, path
from argparse import ArgumentParser
from statistics import median

def generate_permutations(file, directory):
    stem = file.stem
    with open(file) as current:
        lines = current.read()
        statements = []
        counters = []
        counter = 1
        while True:
            start_point = f"// START {counter}"
            end_point = f"// END {counter}"
            if lines.find(start_point) != -1:
                start = lines.find(start_point) + len(start_point)
                end = lines.find(end_point)
                statement = lines[start:end]
                lines = lines.replace(statement, '')

                statements.append(statement)
                counters.append(str(counter))
                counter += 1
            else:
                break

        paths = []
        for tup in permutations(zip(counters, statements)):
            permutation_order, whole_statement = zip(*tup)
            permutation_order = ''.join(permutation_order)
            whole_statement = ''.join(whole_statement)

            new_file_contents = lines.replace('// INSERT HERE', whole_statement)
            
            new_filename = directory.joinpath(f"{stem}_{permutation_order}.c")
            with open(new_filename, 'w') as new_file:
                new_file.write(new_file_contents)
            paths.append(new_filename)
    return paths

def run_experiment(compiler, opt_levels, directory, number_of_experiments, csv_writer, paths, configs):
    for config in configs:
        for opt_level in opt_levels:
            for path in paths:
                try:
                    exe_path = Path(str(path).replace('.c', '.exe'))
                    name, permutation = exe_path.stem.rsplit('_', 1)
                    print(f"Running: {name}, permutation: {permutation}, optimization: {opt_level}, config: {config}")
                    run(f'{compiler} {path} -O{opt_level} -o {exe_path}', shell=True, check=True)
                    runtimes = []
                    for i in range(1, number_of_experiments + 1):
                        output = run(f'{exe_path} {config}', shell=True, check=True, capture_output=True)
                        runtimes.append(float(output.stdout.decode('ascii')))
                    csv_writer.writerow([name, config, permutation, opt_level] + runtimes + [median(runtimes)])
                except Exception as e:
                    print(f"{name} failed with exception: {e}. Skipping")
                finally:
                    if exe_path.exists():
                        exe_path.unlink()
    for path in paths:
        path.unlink()

def read_config(directory):
    config_path = Path(path.join(directory, "config"))    
    if not config_path.exists():
        return []
    with open(config_path) as config:
        return [line for line in [line.split('#')[0].strip() for line in config] if line != '']

def process(directory, csv_writer,number_of_experiments, optimization):
    if "tmp" in str(directory): return
    print(f"Processing directory: {directory}")
    for directory, _, files in list(walk(directory)):
        configs = read_config(directory)
        for input_file in files:
            if input_file.endswith('.c'):
                paths = generate_permutations(Path(path.join(directory, input_file)), tmp)
                run_experiment(compiler, optimization, tmp, number_of_experiments, csv_writer, paths, configs)

if __name__ == "__main__":
    parser = ArgumentParser(description='Driver for the Ammit project benchmarks.')
    parser.add_argument('num_experiments', type=int, help='Number of experiments to run.')
    parser.add_argument('-O', dest='optimization', type=int, default=[1], nargs='*',
                        help='Optimization levels to compile the experiments. Defaults to 1')
    parser.add_argument('-compiler', dest='compiler', default='clang++', help='Compiler to use. Default is clang')
    parser.add_argument('-o', dest='output_file', default='results.csv', help='Ouput file to write. Default is "results.csv"')
    parser.add_argument('-d', dest='start_directory', default='.', 
                        help='Start directory for recursive search. Default is the current directory.')
    args = parser.parse_args()
    
    number_of_experiments = args.num_experiments
    compiler = args.compiler
    optimization = args.optimization
    output_file = args.output_file
    start_directory = args.start_directory

    tmp = Path("./tmp").resolve()
    tmp.mkdir(parents=True, exist_ok=True)
    with open(output_file, 'w') as csv_file:
        csv_writer = writer(csv_file)
        arg_rows = [f'Exp {i}' for i in range(1, number_of_experiments + 1)]
        csv_writer.writerow(["Name", "Arguments", "Permutation", "Optimization"] + arg_rows + ["Median"])
        process(Path(start_directory), csv_writer, number_of_experiments, optimization)        
    tmp.rmdir()
