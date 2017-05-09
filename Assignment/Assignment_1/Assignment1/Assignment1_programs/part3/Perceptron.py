"""
Code for COMP307 assignment 1 Part 3: Perceptron
Author: Shi Tao, student number: 300409943
"""


import random
random.seed(20140713)

class Feature:
    """Represents a feature as perceptron."""
    def __init__(self, row, col, sgn):														
        self.row = row
        self.col =col
        self.sgn = sgn
    
    def __str__(self):
        """ a string representation of this Feature"""
        return "(row: {0}, col: {1}, sgn: {2})".format(
        self.row, self.col, self.sgn)    
        
        
def get_featurelist():
    """Get the list of 50 random features"""
    print("Random feature is:")
    feature_list = []
    count = 0    
    while count < 50:        
        row = []
        col = []
        sgn = []
        for i in range(4):
            row.append(random.randint(0, 9))
            col.append(random.randint(0, 9))
            temp = random.randint(0, 1)
            if temp == 1:
                sgn.append('1')
            else:
                sgn.append('0')
        feature = Feature(row, col, sgn)
        print(feature)
        feature_list.append(feature)
        count += 1
    return feature_list


def get_w():
    """Get the list of w"""
    print("Initial weight set is:")
    w = []
    for i in range(51):
        w.append(random.uniform(0, 1))
    print(w)
    return w
        
    
def image_process(filename, feature_list):
    """Process the image data"""
    print("Training data is:")
    imagefile = open(filename)
    data = imagefile.readlines()
    imagefile.close()
    image_list = []
    count = 0
    while count < 100:
        image = []
        class_name = data[count * 5 + 1].split()[0][1:]
        if class_name == "Yes":
            y = 1
        else:
            y = -1
        if count < 99:
            imagedata = list(data[count * 5 + 3][:-1] + data[count * 5 + 4][:-1])
        else:
            imagedata = list(data[count * 5 + 3][:-1] + data[count * 5 + 4][:])
        for i in range(10):
            image.append(imagedata[(i * 10) : ((i * 10) + 10)])
        print(image)
        x = get_value(image, feature_list)
        count += 1
        image_list.append((x, y))
    return image_list


def get_value(image, feature_list):
    """Get the value of the feature on image"""
    x = [1]
    for feature in feature_list:
        total = 0
        for i in range(4):
            if image[feature.row[i]][feature.col[i]] == feature.sgn[i]:
                total += 1
        if total >= 3:
            x.append(1)
        else:
            x.append(0)
    return x


def get_result(image_list, w, m):
    """Get the result and print"""
    k = 0
    hits = 0
    while k > m or hits < 100:
        hits = 0
        for image in image_list:
            total = 0            
            for i in range(51):
                total += image[0][i] * w[i]
            if total > 0:
                label = 1
            else:
                label = -1
            if label == image[1]:
                hits += 1
            else:
                for i in range(51):
                    w[i] += image[0][i] * image[1]       
        k += 1
    print("After {} times learning, the matched number of image is {}:".format(k, hits))
    print("Final weight set is:")
    print(w)
    return w


def get_class(testfile, feature_list, w):
    """Get the class of testdata"""
    imagefile = open(testfile)
    data = imagefile.readlines()
    imagefile.close()
    image = []
    class_name = data[1].split()[0][1:]
    imagedata = list(data[3][:-1] + data[4][:])
    for i in range(10):
        image.append(imagedata[(i * 10) : ((i * 10) + 10)])
    print(image)
    x = get_value(image, feature_list)
    total = 0
    for i in range(51):
        total += x[i] * w[i]
    if total > 0:
        classification = "Yes"
    else:
        classification = "other"
    if class_name == classification:
        print("Correct!")
    else:
        print("Incorrect!")
            
            
            

def main():
    """ Gets the job done """
    feature_list = get_featurelist()
    image_list = image_process("image.data", feature_list)
    w = get_w()
    w = get_result(image_list, w, 1000)
    
    get_class("test0.data", feature_list, w)
    get_class("test1.data", feature_list, w)
    get_class("test2.data", feature_list, w)
    
    
    
main()