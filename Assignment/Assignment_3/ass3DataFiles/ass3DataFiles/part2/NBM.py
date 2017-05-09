"""
Code for COMP307 assignment 3 Part 2: Naive Bayes Method
Author: Shi Tao, student number: 300409943
"""

def data_process(labelledfile, unlabelledfile):
    """Process the labelled data and unlabelled data"""
    lfiledata = open(labelledfile)
    ldata = lfiledata.readlines()
    lfiledata.close()
    ufiledata = open(unlabelledfile)
    udata = ufiledata.readlines()
    ufiledata.close()
    ldata_list = []
    for line in ldata[:]:
        if line != '\n':
            obj = line.split()
            attributes = []
            for i in range(12):
                attributes.append(obj[i])
            ldata_list.append([attributes, obj[12]])
    udata_list = []
    for line in udata[:]:
        if line != '\n':
            obj = line.split()
            attributes = []
            for i in range(12):
                attributes.append(obj[i])
            udata_list.append(attributes)
    print(ldata_list)
    print(udata_list)
    return (ldata_list, udata_list)


def prior(data):
    """Calculate the P(c)"""
    spam = 0
    unspam = 0
    for instance in data:
        if instance[1] == '0':
            unspam += 1
        else:
            spam += 1
    print(unspam / 200, spam / 200)
    return (unspam / 200, spam / 200)


def likelihood(data, P0, P1):
    """Calculate probabilities (P(Fi|c) for each feature i"""
    occ = []
    pro = []
    for i in range(12):
        zz = 0
        zo = 0 
        oz = 0
        oo = 0
        for instance in data:
            if instance[0][i] == '0' and instance[1] == '0':
                zz += 1
            if instance[0][i] == '0' and instance[1] == '1':
                zo += 1
            if instance[0][i] == '1' and instance[1] == '0':
                oz += 1        
            if instance[0][i] == '1' and instance[1] == '1':
                oo += 1
        occ.append([[zz, zo], [oz, oo]])
        #print(zz, zo)
        #print(oz, oo)        
        zz = zz / (P0 * 200)
        zo = zo / (P1 * 200)
        oz = oz / (P0 * 200)
        oo = oo / (P1 * 200)
        pro.append([[zz, zo], [oz, oo]])
        print(zz, zo)
        print(oz, oo)
    return (occ, pro)


def classmail(data, occ, pro, P0, P1):
    """Classify the unlabelled mails given"""
    for instance in data:
        unspam = P0
        spam = P1        
        for i in range(12):
            #print(pro[i][int(instance[i])][0])
            #print(sum(occ[i][int(instance[i])]) / 200)
            unspam *= pro[i][int(instance[i])][0] / (sum(occ[i][int(instance[i])]) / 200)
            spam *= pro[i][int(instance[i])][1] / (sum(occ[i][int(instance[i])]) / 200)
        print(unspam, spam)
        if unspam >= spam:
            print(instance, 'unspam')
        else:
            print(instance, 'spam')
    
def classtest(data, classifier, P0, P1):
    """Classify the unlabelled mails given"""
    for instance in data:
        unspam = P0
        spam = P1        
        for i in range(12):
            unspam *= classifier[i][int(instance[0][i])][0]
            spam *= classifier[i][int(instance[0][i])][1]
        if unspam >= spam:
            print(instance, 'unspam')
        else:
            print(instance, 'spam')            


def main():
    """ Gets the job done """
    (ldata_list, udata_list) = data_process("spamLabelled.dat", "spamUnlabelled.dat")
    (P0, P1) = prior(ldata_list)
    (occ, pro) = likelihood(ldata_list, P0, P1)
    mailclass = classmail(udata_list, occ, pro, P0, P1)
    #testclass = classtest(ldata_list, pro, P0, P1)
    
main()