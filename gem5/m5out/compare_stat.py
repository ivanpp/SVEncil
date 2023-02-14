import os

def read_stats(modelname:str):
    """
    Read the simulation stats from "modelname/stats.txt" file into a dict.
    return: 
        stats: dict{name: val} e.g. {"simSeconds": 0.000001}
    """
    filename = "stats.txt"
    stats = {}
    file = os.path.join(modelname, filename)
    with open(file, 'r') as f:
        lines = f.read().split('\n')
    for line in lines:
        elements = line.split()
        if len(elements) > 1:
            stats[elements[0]] = elements[1]
    return stats

def read_stats_with_comment(modelname: str):
    """
    Read the simulation stats from "modelname/stats.txt" file into a dict, with
    comments illustrate what stat it is.
    return: 
        stats: dict{name: (val, comments)}
        e.g. {"simSeconds": (0.001, "# Number of seconds simulated (Second)")}
    """
    filename = "stats.txt"
    stats = {}
    file = os.path.join(modelname, filename)
    with open(file, 'r') as f:
        lines = f.read().split('\n')
    for line in lines:
        elements = line.split()
        if len(elements) > 3:
            if elements[2] == "#":
                key = elements[0]
                val = elements[1]
                comment = ""
                for i in range(2, len(elements)):
                    comment = comment + ' ' + elements[i]
                stats[key] = (val, comment)
    return stats

def compare_dicts_base(dict1, dict2):
    """
    {name: (val0, val1)}
    """
    differences = {}
    for key in dict1:
        if key in dict2:
            if dict1[key] != dict2[key]:
                differences[key] = (dict1[key], dict2[key])
    return differences

def compare_dicts(dict1, dict2):
    """
    Compare two dicts with {name: (val, comments)}, store the differences with 
    common key but different value into a new dict
    return: 
        differences: dict{name: (val0, val1, comments)}
        e.g. {"simSeconds": (0.1, 0.2, "# Number of seconds simulated (Second)")}
    """
    differences = {}
    for key in dict1:
        if key in dict2:
            if dict1[key][0] != dict2[key][0]:
                differences[key] = (dict1[key][0], dict2[key][0], dict1[key][1])
    return differences

def compare_abs_diff(elem):
    """
    Compare list/tuple elements based on the abs of the first element
    [diff, *others] or (diff, *others)
    usage: as key of sorted()
    """
    return abs(elem[0])

def sorted_diff(differences: dict, reverse: bool = True):
    """
    Sort the differences stats stored in dict{name: (val0, val1, comments)} by
    abs percentage of diff in val0 and val1
    return:
        (diff, key, val0, val1, [comments])
    """
    diffs = []
    for key, item in differences.items():
        val0, val1, *others = item
        if val0 == "nan" or val1 == "nan": diff = 100 # treat nan as max diff
        else: diff = (float(val1) - float(val0)) / float(val0)
        if len(others) == 0: 
            diffs.append((diff, key, val0, val1)) # only val
        else: diffs.append((diff, key, val0, val1, others)) # with comment
    # sort with abs of diff(first element)
    diffs_sorted = sorted(diffs, key=compare_abs_diff, reverse=reverse)
    return diffs_sorted


def differences2file(differences, model1: str, model2: str, outfile: str):
    """
    Write the differences(dict) to txt file
    """
    with open(outfile, 'w') as f:
        f.write("model1: {}\n".format(model1))
        f.write("model2: {}\n".format(model2))
        f.write("\n")
        for key, values in differences.items():
            f.write(f'{key}: {values[0]} vs {values[1]} {values[2]}\n')

def sorted_diff2file(diffs: list, model1: str, model2: str, outfile: str):
    """
    Write sorted diff(list) to txt file
    """
    with open(outfile, 'w') as f:
        f.write("Value differences of comman stats in \'{}\' and \'{}\'\n".format(model1, model2))
        f.write("\n")
        f.write('{0: <7} {1:<71} {2:<16} {3:<16}\n'.format("diff", "arg", model1, model2))
        for item in diffs:
            # unpack
            diff, key, val0, val1, *comm = item
            if len(comm) == 0: comm = ' '
            f.write(f'{diff: <7.2f} {key: <71.71} {val0: <16} {val1: <16} {comm[0][0]}\n')

def compare_stats(model1: str, model2: str, outfile: str):
    stats1 = read_stats_with_comment(model1)
    stats2 = read_stats_with_comment(model2)
    differences = compare_dicts(stats1, stats2)
    diffs_sorted = sorted_diff(differences)
    sorted_diff2file(diffs_sorted, model1, model2, outfile)

if __name__ == '__main__':
    model1 = input("modelname1: ")
    model2 = input("modelname2: ")
    outfile = model1 + '_versus_' + model2 + '.txt'
    compare_stats(model1, model2, outfile)

# stats1 = read_stats_with_comment('sa_v1t')
# stats2 = read_stats_with_comment('sa_v2t')
# differences = compare_dicts(stats1, stats2)
# diffs_sorted = sorted_diff(differences)
# sorted_diff2file(diffs_sorted, 'm1', 'm2', 'test.txt')

