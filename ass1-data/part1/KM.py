"""
Code for COMP307 assignment 1 Part 1: K-Means Clustering
Author: Shi Tao, student number: 300409943
"""

def data_process(filename1, filename2):
    """Process the training data and test data"""
    trainingfile = open(filename1)
    training = trainingfile.readlines()
    trainingfile.close()
    data_list = []
    obj_class = "init"
    for line in training[:]:
        if line != '\n':
            train_obj = line.split()
            d0 = float(train_obj[0])
            d1 = float(train_obj[1])
            d2 = float(train_obj[2])
            d3 = float(train_obj[3])
            data_list.append([[d0, d1, d2, d3], obj_class])
    testfile = open(filename2)
    test = testfile.readlines()
    testfile.close()
    for line in test[:]:
        if line != '\n':
            test_obj = line.split()
            d0 = float(test_obj[0])
            d1 = float(test_obj[1])
            d2 = float(test_obj[2])
            d3 = float(test_obj[3])
            data_list.append([[d0, d1, d2, d3], obj_class])    
    return data_list

def get_range(data_list):
    """Get the range of component"""
    RL0 = []
    RL1 = []
    RL2 = []
    RL3 = []
    for instance in data_list[:]:
        d0 = instance[0][0]
        RL0.append(d0)
        d1 = instance[0][1]
        RL1.append(d1)      
        d2 = instance[0][2]
        RL2.append(d2)      
        d3 = instance[0][3]
        RL3.append(d3)     
    RL0.sort()
    RL1.sort()
    RL2.sort()
    RL3.sort()
    R0 = RL0[-1] - RL0[0]
    R1 = RL1[-1] - RL1[0]
    R2 = RL2[-1] - RL2[0]
    R3 = RL3[-1] - RL3[0]
    return (R0, R1, R2, R3)


def class_process(data_list, R0, R1, R2, R3, setosa_center, versicolor_center, virginica_center):
    """Finish the class process"""
    for instance in data_list:
        d_setosa = distance_measure(instance[0], setosa_center, R0, R1, R2, R3)
        d_versicolor = distance_measure(instance[0], versicolor_center, R0, R1, R2, R3)
        d_virginica = distance_measure(instance[0], virginica_center, R0, R1, R2, R3)
        if d_setosa == min(d_setosa, d_versicolor, d_virginica):
            instance[1] = 'Iris-setosa'
        elif d_versicolor == min(d_setosa, d_versicolor, d_virginica):
            instance[1] = 'Iris-versicolor'
        else:
            instance[1] = 'Iris-virginica'
        #print(instance)
    return data_list

def get_center(data_list):
    """Get the center of three classes"""
    setosa = []
    versicolor = []
    virginica = []
    for instance in data_list:
        if instance[1] == 'Iris-setosa':
            setosa.append(instance)
        elif instance[1] == 'Iris-versicolor':
            versicolor.append(instance)
        else:
            virginica.append(instance)
    setosa_center = get_mean(setosa)
    versicolor_center = get_mean(versicolor)
    virginica_center = get_mean(virginica)
    print("setosa_center:" , setosa_center)
    print("versicolor_center:", versicolor_center)
    print("versicolor_center:", virginica_center)    
    return (setosa_center, versicolor_center, virginica_center)
            

def get_mean(class_set):
    """Get the mean of the subset"""
    d1 = 0
    d2 = 0
    d3 = 0
    d4 = 0
    for instance in class_set:
        d1 += instance[0][0]
        d2 += instance[0][1]
        d3 += instance[0][2]
        d4 += instance[0][3]
    subset_center = [d1 / len(class_set),
                     d2 / len(class_set),
                     d3 / len(class_set),
                     d4 / len(class_set)]
    return subset_center       
        

  
def distance_measure(vec1, vec2, R0, R1, R2, R3):
    """Distance measure"""
    dist = ((vec1[0] - vec2[0]) ** 2 / (R0 ** 2) +
           (vec1[1] - vec2[1]) ** 2 / (R1 ** 2) +
           (vec1[2] - vec2[2]) ** 2 / (R2 ** 2) +
           (vec1[3] - vec2[3]) ** 2 / (R3 ** 2)) ** 0.5
    return dist


def main():
    """ Gets the job done """
    data_list = data_process("iris-training.txt", "iris-test.txt")
    (R0, R1, R2, R3) = get_range(data_list)
    setosa_center = [5.1,  3.5,  1.4,  0.2]   #[4.3,3.0,1.1,0.1]
    versicolor_center = [7.0,  3.2,  4.7,  1.4]   #[6.0,2.2,4.0,1.0]
    virginica_center = [6.3,  3.3,  6.0,  2.5]   #[7.7,3.8,6.7,2.2]
    data_list = class_process(data_list, R0, R1, R2, R3, setosa_center, versicolor_center, virginica_center)
    print("After 1 time:")
    (setosa_cen, versicolor_cen, virginica_cen) = get_center(data_list)
    n = 1
    while (setosa_cen, versicolor_cen, virginica_cen) != (setosa_center, versicolor_center, virginica_center):
        n += 1
        (setosa_center, versicolor_center, virginica_center) = (setosa_cen, versicolor_cen, virginica_cen)
        data_list = class_process(data_list, R0, R1, R2, R3, setosa_center, versicolor_center, virginica_center)
        print("After {} time:".format(n))
        (setosa_cen, versicolor_cen, virginica_cen) = get_center(data_list)
    print("The results of after {} times and after {} times are same, so the final center is:".format(n-1, n))
    print("setosa_center:" , setosa_center)
    print("versicolor_center:", versicolor_center)
    print("versicolor_center:", virginica_center)
    print("The class of instance is:")
    for instance in data_list:
        print(instance)

    
    
main()