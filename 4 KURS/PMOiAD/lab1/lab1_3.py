import numpy as np
import matplotlib.pyplot as plt

# если DataGenerator.py в той же папке
from DataGenerator import norm_dataset


mu0 = [0, 2, 3]
mu1 = [3, 5,1]
sigma0= [2, 1, 2]
sigma1 = [1, 2, 1]

#mu =  мат. ожидание
#sigma = otklonenie (ско)

N = 1000 # число объектов класса
col = len(mu0) # количество столбцов-признаков

import DataGenerator as dg
mu = [mu0, mu1]
sigma = [sigma0, sigma1]
X, Y, class0, class1 = dg.norm_dataset(mu,sigma,N)

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


