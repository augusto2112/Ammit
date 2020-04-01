from subprocess import run

clang = 'clang'
project_directory = '/home/augusto/Developer/llvm/switch-finder'
so_path = f'{project_directory}/cmake-build-debug/switch_finder/libSwitchFinder.so'
test_path = f'{project_directory}/test/'
source_path = f'{test_path}/test.c'
ll_path = f'{test_path}/test.ll'

run(f'{clang} {source_path} -S -O0 -disable-O0-optnone -Xclang -emit-llvm  -o {ll_path}', shell=True, check=True)

