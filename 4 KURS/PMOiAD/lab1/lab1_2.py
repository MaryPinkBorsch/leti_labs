import numpy as np
import matplotlib.pyplot as plt


mu0 = [0, 2, 3]
mu1 = [3, 5,1]
sigma0= [2, 1, 2]
sigma1 = [1, 2, 1]

#mu = МО
#sigma = otklonenie ско


N = 1000 # число объектов класса
col = len(mu0) # количество столбцов-признаков – длина массива средних 
class0 = np.random.normal(mu0[0],sigma0[0],[N,1]) # инициализируем первый столбец (в Python нумерация от 0)
class1 = np.random.normal(mu1[0],sigma1[0],[N,1])

for i in range(1,col): # подумайте, почему нумерация с 1, а не с 0 
    v0 = np.random.normal(mu0[i],sigma0[i],[N,1])
    class0 = np.hstack((class0,v0))

    v1 = np.random.normal(mu1[i],sigma1[i],[N,1]) 
    class1 = np.hstack((class1,v1))

Y1 = np.empty((N, 1), dtype=bool) # пустой массив размерности Nx1 
Y1.fill(1) #заполнение пустого массива единицами
Y0 = np.empty((N, 1), dtype=bool) 
Y0.fill(0)

Y1 =np.ones((N, 1), dtype=bool) 
Y0 = np.zeros((N, 1), dtype=bool)

X = np.vstack((class0,class1))
Y = np.vstack((Y0,Y1)).ravel() #ravel позволяет сделать массив плоским – одномерным, размера (N,), это необходимо для дальнейшего использования в классификаторах


# перемешиваем данные
rng = np.random.default_rng()
arr = np.arange(2*N) #индексы для перемешивания 
rng.shuffle(arr)

X = X[arr] 
Y = Y[arr]


# разделяем данные на 2 подвыборки
trainCount = round(0.7*N*2) # не забываем округлить до целого 
Xtrain = X[0:trainCount]
Xtest = X[trainCount:N*2+1] 
Ytrain = Y[0:trainCount] 
Ytest = Y[trainCount:N*2+1]


# построение гистограмм распределения для всех признаков 

for i in range(0, col):
    plt.figure()
    plt.hist(class0[:, i], bins='auto', alpha=0.7, label='class0')
    plt.hist(class1[:, i], bins='auto', alpha=0.7, label='class1')
    plt.title(f'Гистограмма признака {i+1}')
    plt.xlabel(f'Признак {i+1}')
    plt.ylabel('Частота')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig('hist_'+str(i+1)+'.png')
    plt.show()

plt.figure()
plt.scatter(class0[:,0], class0[:,2], marker=".", alpha=0.7, label='class0')
plt.scatter(class1[:,0], class1[:,2], marker=".", alpha=0.7, label='class1')
plt.title('Диаграмма рассеяния: признак 1 vs признак 3')
plt.xlabel('Признак 1')
plt.ylabel('Признак 3')
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('scatter_1_3.png')
plt.show()


