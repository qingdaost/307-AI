"""
Code for COMP307 assignment 1 Part 1: Nearest Neighbour Method
Author: Shi Tao, student number: 300409943
"""

def data_process(filename1, filename2):
    """Process the training data and test data"""
    trainingfile = open(filename1)
    training = trainingfile.readlines()
    trainingfile.close()
    training_list = []
    RL0 = []
    RL1 = []
    RL2 = []
    RL3 = []
    for line in training[:]:
        if line != '\n':
            train_obj = line.split()
            d0 = float(train_obj[0])
            RL0.append(d0)
            d1 = float(train_obj[1])
            RL1.append(d1)      
            d2 = float(train_obj[2])
            RL2.append(d2)      
            d3 = float(train_obj[3])
            RL3.append(d3)     
            obj_class = train_obj[4]
            training_list.append(([d0, d1, d2, d3], obj_class))
    RL0.sort()
    RL1.sort()
    RL2.sort()
    RL3.sort()
    R0 = RL0[-1] - RL0[0]
    R1 = RL1[-1] - RL1[0]
    R2 = RL2[-1] - RL2[0]
    R3 = RL3[-1] - RL3[0]
    
    testfile = open(filename2)
    test = testfile.readlines()
    testfile.close()
    test_list = []
    for line in test[:]:
        if line != '\n':
            test_obj = line.split()
            d0 = float(test_obj[0])
            d1 = float(test_obj[1])
            d2 = float(test_obj[2])
            d3 = float(test_obj[3])
            test_class = test_obj[4]
            test_list.append(([d0, d1, d2, d3], test_class))       
    return (training_list, R0, R1, R2, R3, test_list)


def get_dist_list(training_list, test, R0, R1, R2, R3):
    """Get distance list of one test"""
    dist_list = []
    for training in training_list:
        dist = distance_measure(test[0], training[0], R0, R1, R2, R3)
        dist_list.append((dist, training[1]))
    return dist_list


def get_class_list(dist_list, k):
    """Achieve k nearest class list"""
    class_list = []
    for i in range(0, k):
        min_dist = dist_list[0][0]
        Iris_class = dist_list[0][1]
        index = 0
        for j in range(1, len(dist_list)):
            if dist_list[j][0] < min_dist:
                min_dist = dist_list[j][0]
                Iris_class = dist_list[j][1]
                index = j
        class_list.append(Iris_class)
        dist_list.pop(index)
    return class_list

def get_final_class(class_list):
    """Get the fianl class"""
    if len(class_list) == 1:
        final_class = class_list[0]
    else:
        a = class_list.count("Iris-setosa")
        b = class_list.count("Iris-versicolor")
        c = class_list.count("Iris-virginica")
        max_class = max(a, b, c)
        if a == max_class:
            final_class = "Iris-setosa"
        elif b == max_class:
            final_class = "Iris-versicolor"
        else:
            final_class = "Iris-virginica"
    return final_class
            

       
def distance_measure(vec1, vec2, R0, R1, R2, R3):
    """Distance measure"""
    dist = ((vec1[0] - vec2[0]) ** 2 / (R0 ** 2) +
           (vec1[1] - vec2[1]) ** 2 / (R1 ** 2) +
           (vec1[2] - vec2[2]) ** 2 / (R2 ** 2) +
           (vec1[3] - vec2[3]) ** 2 / (R3 ** 2)) ** 0.5
    return dist

def printresult(training_list, R0, R1, R2, R3, test_list, k):
    """Print the reselt of test data given k"""
    error = 0
    for test in test_list:
        #print(test, end = ";")
        dist_list = get_dist_list(training_list, test, R0, R1, R2, R3)
        class_list = get_class_list(dist_list, k)
        final_class = get_final_class(class_list)
        #print("predicted class:" + final_class, end = ";")
        if test[1] != final_class:
            error += 1
            print("incorrect")
        else:
            print("correct")
    accuracy = "{:.2f}%".format((1 - error / len(test_list)) * 100)
    print("k = " + str(k) + ", accuracy is " + accuracy)
    print("\n\n")
    
def simpleresult(training_list, R0, R1, R2, R3, test_list, k):
    """Print the reselt of test data given k"""
    error = 0
    for test in test_list:
        #print(test, end = ";")
        dist_list = get_dist_list(training_list, test, R0, R1, R2, R3)
        class_list = get_class_list(dist_list, k)
        final_class = get_final_class(class_list)
        #print("predicted class:" + final_class, end = ";")
        if test[1] != final_class:
            error += 1
            #print("incorrect")
        #else:
            #print("correct")
    accuracy = "{:.2f}%".format((1 - error / len(test_list)) * 100)
    print("k = " + str(k) + ", accuracy is " + accuracy)
  


def main():
    """ Gets the job done """
    (training_list, R0, R1, R2, R3, test_list) = data_process("iris-training.txt", "iris-test.txt")
    printresult(training_list, R0, R1, R2, R3, test_list, 1)
    
    printresult(training_list, R0, R1, R2, R3, test_list, 3)

    simpleresult(training_list, R0, R1, R2, R3, test_list, 5)


    
    
main()