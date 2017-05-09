"""
Code for COMP307 assignment 1 Part 2: Decision Tree Learning Method
Author: Shi Tao, student number: 300409943
"""

class Node:
    """Represents a node in a binary tree."""
    def __init__(self, bestAtt, left, right):														
        self.bestAtt = bestAtt
        self.left = left
        self.right = right


    def report(self, indent):
        """Output the report"""
        print("{}= True:".format(self.bestAtt))
        print(indent, end = "\t")
        if isinstance(self.left, tuple) == True:
            print("\t", self.left)
        else:
            self.left.report(indent + "\t")
        print("{}{}= False:".format(indent, self.bestAtt))
        print(indent, end = "\t")
        if isinstance(self.right, tuple) == True:
            print("\t", self.right)
        else:
            self.right.report(indent + "\t")
    
    

def training_process(filename):
    """Process the training data"""
    trainingfile = open(filename)
    training = trainingfile.readlines()
    trainingfile.close()
    class_list = training[0].split()
    attr_list = training[1].split()
    training_list = []
    count = 0
    for line in training[2:]:
        if line != '\n':
            training_obj = line.split()
            class_name = training_obj[0]
            if class_name == 'live':
                count += 1
            vals_list = training_obj[1:]
            vals = {}
            for i in range(16):
                vals[attr_list[i]] = vals_list[i]
            training_list.append((class_name, vals))
    if count > len(training_list) / 2:
        baseline = 'live'
    else:
        baseline = 'die'
    print("Baseline class is:" + baseline)
    return (training_list, attr_list, baseline)


def get_overall_class(training_list, attr_list):
    """Get the most probable class overall"""
    live_num = 0
    for train in training_list:
        if train[0] == 'live':
            live_num += 1
    if live_num >= len(training_list) - live_num:
        overall_class = 'live'
    else:
        overall_class = 'die'
    return overall_class
    
  
def BuildTree(training_list, attr_list, overall_class):
    """Build the decision tree"""
    live_num = 0
    for train in training_list:
        if train[0] == 'live':
            live_num += 1
    if len(training_list) == 0:
        return (overall_class, 0)
    if live_num == len(training_list):
        return ("live", live_num)
    elif live_num == 0:
        return ("die", len(training_list))
    if len(attr_list) == 0:
        if live_num >= len(training_list) - live_num:
            set_class = 'live'
            majority = live_num
        else:
            set_class = 'die'
            majority = len(training_list) - live_num
        return (set_class, majority)
    else:
        best_attr = attr_list[0]
        best_true_set = []
        best_false_set = []
        for instance in training_list:
            if instance[1][best_attr] == "true":
                best_true_set.append(instance)
            else:
                best_false_set.append(instance)
        best_purity = get_purity(best_true_set, best_false_set)        
        for attr in attr_list[1:]:
            true_set = []
            false_set = []
            for instance in training_list:
                if instance[1][attr] == "true":
                    true_set.append(instance)
                else:
                    false_set.append(instance)
            purity = get_purity(true_set, false_set)
            if purity < best_purity:
                best_attr = attr
                best_true_set = true_set
                best_false_set = false_set     
                best_purity = purity
        #attr_list.remove(best_attr)
        new_list = []
        for attr in attr_list:
            if attr != best_attr:
                new_list.append(attr)
        left = BuildTree(best_true_set, new_list, overall_class)
        right = BuildTree(best_false_set, new_list, overall_class)
        return Node(best_attr, left, right)
    #return node
    


def get_purity(true_set, false_set):
    """Get the purity of true_set and false_set"""
    true_weight = len(true_set)
    live_in_true = 0
    false_weight = len(false_set)
    live_in_false = 0
    total = true_weight + false_weight
    for instance in true_set:
        if instance[0] == "live":
            live_in_true += 1
    if true_weight == 0:
        pp_true = 0
    else:
        pp_true = live_in_true * (true_weight - live_in_true) / (true_weight ** 2)
        
    for instance in false_set:
        if instance[0] == "live":
            live_in_false += 1
    if false_weight == 0:
        pp_false = 0
    else:
        pp_false = live_in_false * (false_weight - live_in_false) / (false_weight ** 2)
    purity = true_weight / total * pp_true + false_weight / total * pp_false
    return purity


def get_class(filename, root):
    """Get the class of test instance according to DT"""
    testfile = open(filename)
    test_data = testfile.readlines()
    testfile.close()
    class_list = test_data[0].split()
    attr_list = test_data[1].split()
    test_list = []
    for line in test_data[2:]:
        if line != '\n':
            test_obj = line.split()
            class_name = test_obj[0]
            vals_list = test_obj[1:]
            vals = {}
            for i in range(16):
                vals[attr_list[i]] = vals_list[i]
            test_list.append((class_name, vals))
    node = root
    correct = 0
    for instance in test_list:
        while isinstance(node, tuple) != True:
            if instance[1][node.bestAtt] == 'true':
                node = node.left
            elif instance[1][node.bestAtt] == 'false':
                node = node.right
        #print(instance, node[0])
        if instance[0] == node[0]:
            correct += 1
        node = root
    accuracy = correct / len(test_list) * 100
    print("Accuracy: {:.2f}%".format(accuracy))
    return accuracy
    
                
            

def main():
    """ Gets the job done """
    """
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    node.report("\t")
    get_class("hepatitis-training.dat", node)
    get_class("hepatitis-test.dat", node)
    """
    accuracy = 0
    print("run01")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run01.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run01.dat", node)
    print("run02")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run02.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run02.dat", node)
    print("run03")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run03.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run03.dat", node)    
    print("run04")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run04.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run04.dat", node)
    print("run05")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run05.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run05.dat", node)
    print("run06")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run06.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run06.dat", node)
    print("run07")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run07.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run07.dat", node)
    print("run08")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run08.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run08.dat", node)    
    print("run09")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run09.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run09.dat", node)  
    print("run10")
    (training_list, attr_list, baseline_class) = training_process("hepatitis-training-run10.dat")
    node = BuildTree(training_list, attr_list, baseline_class)
    accuracy += get_class("hepatitis-test-run10.dat", node)
    average_accuracy = accuracy / 10
    print("Average accuracy: {:.2f}%".format(average_accuracy))
    
    
    
main()