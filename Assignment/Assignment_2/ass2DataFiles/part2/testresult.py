def y(x):
    return (x**4)-2*(x**3)+x**2+1

def main():
    for i in range(20):
        x = -2 + i * 0.25
        print(x, y(x))
        
main()    