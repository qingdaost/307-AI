"""
Code for COMP307 assignment 2 Part 1: cope with the original data
"""

def data_process(filename):
    """Process the training data and test data"""
    filedata = open(filename)
    data = filedata.readlines()
    filedata.close()
    for line in data[:]:
        if line != '\n':
            obj = line.split()
            d0 = float(obj[0])
            d1 = float(obj[1])
            d2 = float(obj[2])   
            d3 = float(obj[3])
            obj_class = obj[4]
            if obj_class == 'Iris-setosa':
                d4 = 1
                d5 = 0
                d6 = 0
            elif obj_class == 'Iris-versicolor':
                d4 = 0
                d5 = 1
                d6 = 0
            elif obj_class == 'Iris-virginica':
                d4 = 0
                d5 = 0
                d6 = 1
            print('{};{};{};{};{};{};{}'.format(d0, d1, d2, d3, d4, d5, d6))
            
def main():
    """ Gets the job done """
    data_process("iris-training.txt")
    print("=" * 20)
    data_process("iris-test.txt")
    
    
main()