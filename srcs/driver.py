from sys import argv, exit
from itertools import permutations, product
from pathlib import Path
from subprocess import run
from csv import writer, reader
from os import walk, path

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
                exe_path = Path(str(path).replace('.c', '.exe'))
                name, permutation = exe_path.stem.rsplit('_', 1)
                print(f"Running: {name}, permutation: {permutation}, optimization: {opt_level}, config: {config}")
                run(f'{compiler} {path} -O{opt_level} -o {exe_path}', shell=True, check=True)
                runtimes = []
                for i in range(1, number_of_experiments + 1):
                    output = run(f'{exe_path} {config}', shell=True, check=True, capture_output=True)
                    runtimes.append(output.stdout.decode('ascii'))
                csv_writer.writerow([name, config, permutation, opt_level] + runtimes)
                exe_path.unlink()
    for path in paths:
        path.unlink()



def read_config(directory):
    config_path = Path(path.join(directory, "config"))    
    if not config_path.exists():
        return []
    with open(config_path) as config:
        config_reader = reader(config)
        configs = [x for x in config_reader]
        return [' '.join([y.strip() for y in x]) for x in product(*configs)]



def process(directory, csv_writer):
    if "tmp" in str(directory): return
    print(f"Processing directory: {directory}")
    for directory, _, files in list(walk(directory)):
        configs = read_config(directory)
        for input_file in files:
            if input_file.endswith('.c'):
                paths = generate_permutations(Path(path.join(directory, input_file)), tmp)
                run_experiment(compiler, [1], tmp, number_of_experiments, csv_writer, paths, configs)

if __name__ == "__main__":
    if len(argv) < 3:
        print("Script needs at least one argument")
        exit()

    number_of_experiments = int(argv[1])
    compiler = argv[2]

    tmp = Path("./tmp").resolve()
    tmp.mkdir(parents=True, exist_ok=True)
    with open("csv.csv", 'w') as csv_file:
        csv_writer = writer(csv_file)
        process(Path("."), csv_writer)        
    tmp.rmdir()
