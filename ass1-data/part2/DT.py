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
        if isinstance(self.left, str) == True:
            print("\t", self.left)
        else:
            self.left.report(indent + "\t")
        print("{}{}= False:".format(indent, self.bestAtt))
        print(indent, end = "\t")
        if isinstance(self.right, str) == True:
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
    #print(class_list)
    #print(attr_list)
    training_list = []
    for line in training[2:]:
        if line != '\n':
            training_obj = line.split()
            class_name = training_obj[0]
            vals_list = training_obj[1:]
            vals = {}
            for i in range(16):
                vals[attr_list[i]] = vals_list[i]
            training_list.append((class_name, vals))
    #print(training_list)
    return (training_list, attr_list)


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
        return ("Category " + overall_class)
    if live_num == len(training_list):
        return "Category live, prob = 100%"
    elif live_num == 0:
        return "Category die, prob = 100%"
    if len(attr_list) == 0:
        
        if live_num >= len(training_list) - live_num:
            set_class = 'live'
        else:
            set_class = 'die'
        return ("Category " + set_class)      
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
        attr_list.remove(best_attr)
        left = BuildTree(best_true_set, attr_list, overall_class)
        right = BuildTree(best_false_set, attr_list, overall_class)
        node = Node(best_attr, left, right)
    return node
    

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
        
    

def main():
    """ Gets the job done """
    (training_list, attr_list) = training_process("hepatitis-training.dat")
    overall_class = get_overall_class(training_list, attr_list)
    node = BuildTree(training_list, attr_list, overall_class)
    node.report("\t")
    
    
    
main()