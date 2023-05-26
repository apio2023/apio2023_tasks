import os
import json

PROGNAME = 'val.exe'

path = os.getcwd()
print('file path', path)
path = os.path.abspath(path+os.path.sep+'..')
print('parent path', path)

def test(dir, id):  
    print('subtask', id, dir)
    val_dir = path + os.path.sep + 'script' + os.path.sep + PROGNAME
    
    if id == 0:
        id = 8
    with open(dir,'r') as load_f:
        dict = json.load(load_f)
    
    for inf in dict['testcases']:
        test_dir = path + os.path.sep + 'tests' + os.path.sep + inf + ".in"
        print("{0} <{1} {2}".format(val_dir, test_dir, id))
        os.system("{0} <{1} {2}".format(val_dir, test_dir, id))

jsonlist = ['01', '02', '03', '04', '05', '06', '07', '08-full']
for id, jsonname in enumerate(jsonlist):
    test(path + os.path.sep + 'subtasks' + os.path.sep + jsonname + '.json', id + 1)