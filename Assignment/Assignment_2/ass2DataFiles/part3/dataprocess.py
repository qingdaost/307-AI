def data_process(filename):
    """Process the data"""
    trainingfile = open(filename)
    training = trainingfile.readlines()
    trainingfile.close()
    data_list = []
    benign = 0
    malignant = 0

    for line in training[:]:
        if line != '\n':
            train_obj = line.split(",")
            for i in range(1,10):
                if train_obj[i] == '?':
                    print(-1, end = " ")
                else:
                    print(train_obj[i], end = " ")
            if train_obj[10] == '2\n':
                train_obj[10] = 1
                benign += 1
            if train_obj[10] == '4\n':
                malignant += 1
                train_obj[10] = -1
            print(train_obj[10])
    print("benign:",benign)
    print("malignant:",malignant)


def main():
    """ Gets the job done """
    data_process("breast-cancer-wisconsin.data")
    
    
main()