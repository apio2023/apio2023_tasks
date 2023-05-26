import os
import json
import time

PROGNAME = 'cyberland.exe'
SPJNAME = 'checker.exe'

path = os.getcwd()
print('file path', path)
path = os.path.abspath(path+os.path.sep+'..')
print('parent path', path)

def test(dir, id): 
    if (id != 7):
        return (0, 0) 
    print('subtask', id, dir)
    spjdir = path + os.path.sep + 'script' + os.path.sep + SPJNAME
    progdir = path + os.path.sep + 'script' + os.path.sep + PROGNAME
    with open(dir,'r') as load_f:
        dict = json.load(load_f)
    
    is_correct = True
    mx_time = 0.0
    for inf in dict['testcases']:
        in_dir = path + os.path.sep + 'tests' + os.path.sep + inf + ".in"
        out_dir = path + os.path.sep + 'tests' + os.path.sep + inf + ".out"
        usr_dir = path + os.path.sep + 'tests' + os.path.sep + inf + ".ans"
        res_dir = path + os.path.sep + 'tests' + os.path.sep + "spj.spj"
        rab_dir = path + os.path.sep + 'tests' + os.path.sep + "rab.spj"
        st = time.time()
        os.system("{0} <{1} >{2}".format(progdir, in_dir, usr_dir))
        ed = time.time()
        report = os.system("{0} {1} {2} {3} {4} >{5}".format(spjdir, in_dir, out_dir, usr_dir, res_dir, rab_dir))
        with open(res_dir, "r") as f:
            result = f.readline()
            print(result, ',testid: ', inf, ',time:', ed-st)
        if result == 'Correct.':
            if ed-st > mx_time:
                mx_time = ed-st
        else:
            is_correct = False
        os.system("del {0}".format(usr_dir))
    os.system("del {0}".format(res_dir))
    os.system("del {0}".format(rab_dir))
    
    if is_correct:
        return (dict['score'], mx_time)
    return (0,0)

jsonlist = ['01', '02', '03', '04', '05', '06', '07', '08-full']
scores, tls = [], []
for id, jsonname in enumerate(jsonlist):
    score, tl = test(path + os.path.sep + 'subtasks' + os.path.sep + jsonname + '.json', id + 1)
    scores.append(score)
    tls.append(tl)

sum = 0
for id in range(len(scores)):
    print('subtask:', id, ', score:', scores[id], ', max runtime:', tls[id])
    sum = sum + scores[id]
print('total score:', sum)