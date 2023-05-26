import os
import json
import time

STD = 'cyberland.exe'

path = os.getcwd()
print('file path', path)
path = os.path.abspath(path+os.path.sep+'..')
print('parent path', path)

def test(dir, id):  
    print('subtask', id, dir)
    val_dir = path + os.path.sep + 'script' + os.path.sep + STD
    
    if id == 0:
        id = 8
    with open(dir,'r') as load_f:
        dict = json.load(load_f)
    
    for inf in dict['testcases']:
        in_dir = path + os.path.sep + 'tests' + os.path.sep + inf + ".in"
        out_dir = path + os.path.sep + 'tests' + os.path.sep + inf + ".out"
        print("{0} <{1} >{2}".format(val_dir, in_dir, out_dir))
        tic = time.time()
        os.system("{0} <{1} >{2}".format(val_dir, in_dir, out_dir))
        print("running time:",time.time() - tic)

jsonlist = ['01', '02', '03', '04', '05', '06', '07', '08-full']
for id, jsonname in enumerate(jsonlist):
    test(path + os.path.sep + 'subtasks' + os.path.sep + jsonname + '.json', id + 1)