import numpy
import matplotlib.pyplot as plot

# наш код с практики
def prob_estimate(S):
    N= len(S)
    P=numpy.array([0 for i in range(128)])
    for s in S:    
        P[ord(s)] += 1
    P=P/N
    return P

# второй варик
def prob_estimate2(S):
    total_symbols = len(S)
    symbol_counts = {}
    
    for symbol in S:
        if symbol in symbol_counts:
            symbol_counts[symbol] += 1
        else:
            symbol_counts[symbol] = 1
            
    probabilities = numpy.array(list(symbol_counts.values())) / total_symbols
    return probabilities
# код без учета длины
def entropy_basic(S):
    P= prob_estimate(S)
    P=numpy.array(list(filter(lambda x: x!=0,P)))
    H=-sum(numpy.log2(P)* P)
    return H

# код с учетом длины
def entropy(S,code_len):
    P= prob_estimate2(S)
    P=numpy.array(list(filter(lambda x: x!=0,P)))
    H=-sum(numpy.log2(P)* P*code_len)
    return H


# ТЕСТЫ
S = 'aaabbb'
print('Current string: ', S)
print(prob_estimate(S))

print('Current entropy: ', entropy_basic(S))

S = 'aaaaaaaaaaaaaaaaaaaaab'
print('Current string: ', S)
print(prob_estimate(S))

print('Current entropy: ', entropy_basic(S))
print('Current entropy2: ', entropy(S,1))
print('Current entropy2 (2 len): ', entropy(S,2))

######################################################################################################################
# Функция для фильтрации символов с юникодом выше 127
def filter_message(message):
    return bytes(filter(lambda x: x <= 127, message))

# Пример текста
text = b"Heloooo, i love pink! PINK is the best, loveeee nya)) Lol, i wanna a cupcake :)"
filtered_text = filter_message(text)

# Исследуем зависимость энтропии от длины кода символов
code_lengths = range(1, 5)  # Длина кода символа от 1 до 4 байт
entropies = [entropy(filtered_text, length) for length in code_lengths]

# Выводим результаты на график
plot.plot(code_lengths, entropies, marker='o')
plot.title('Зависимость энтропии от длины кода символа')
plot.xlabel('Длина кода символа (байты)')
plot.ylabel('Энтропия')
plot.xticks(code_lengths)
plot.grid()
plot.savefig('itog_test')
# plot.show()
