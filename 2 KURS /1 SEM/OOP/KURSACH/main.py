import mytoken
import telebot
from mytoken import My_token
from telebot import types
botTimeWeb = My_token.botTimeWeb





######################################################################################################
import telebot
from telebot import types 


# bot = telebot.TeleBot(My_token.botTimeWeb)
mess1 = "Стрейчинг для шеи 10 мин:"
mess2 = "Стрейчинг от боли в спине 10 мин:"
mess3 = "Общая растяжка 5 мин:"
mess4 = "Общая растяжка 10 мин:"
mess5 = "Общая растяжка 15 мин:"
mess6 = "Кардио пилатес 10 мин:"
mess7 = "Пилатес на руки 7 мин:"
mess8 = "Пилатес на ноги 5 мин:"
mess9 = "Пилатес на пресс 6 мин:"
mess10 = "Пилатес на ягодицы 7 мин:"
mess11 = "Пилатес на руки (версия с песиком) 5 мин:"
mess12 = "Растяжка против ПМС 15 мин:"
mess13 = "Пилатес на ноги 10 мин:"
mess14 = "Pump it Up! 75 мин:"


@botTimeWeb.message_handler(commands=['start'])
def start(message):
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn1 = types.KeyboardButton("Поздороваться")
    btn2 = types.KeyboardButton("Начать тренировку")
    markup.add(btn1, btn2)
    botTimeWeb.send_message(message.chat.id, text="Приветик, {0.first_name}! \nДавай начнем тренировку?".format(message.from_user), reply_markup=markup)
    
@botTimeWeb.message_handler(content_types=['text'])
def func(message):
    if(message.text == "Поздороваться"):
        botTimeWeb.send_message(message.chat.id, text="Привет! Готова к хорошей тренировке? Напиши да, когда будешь готова)\nЕсли будут вопросы, напиши ?")
    elif(message.text == "Начать тренировку" or message.text == "да" or message.text == "давай" or message.text == "Да"):
        markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
        btn1 = types.KeyboardButton("Пилатес")
        btn2 = types.KeyboardButton("Растяжка")
        back = types.KeyboardButton("Вернуться в главное меню")
        markup.add(btn1, btn2, back)
        botTimeWeb.send_message(message.chat.id, text="Выбирай тип тренировки и нажимай на нужную кнопку)", reply_markup=markup)
    
    elif(message.text == "Пилатес" or message.text == "пилатес"):
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=AjWoaUBe80c&ab_channel=BaileyBrown"))
        botTimeWeb.send_message(message.chat.id, mess6, reply_markup=markup)
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=toOhNs85Mj8&ab_channel=BaileyBrown"))
        botTimeWeb.send_message(message.chat.id, mess7, reply_markup=markup)
            
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=ucW3ygyzgaI&ab_channel=BaileyBrown"))
        botTimeWeb.send_message(message.chat.id, mess8, reply_markup=markup)
            
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=N2yVhRKMApY&ab_channel=BaileyBrown"))
        botTimeWeb.send_message(message.chat.id, mess9, reply_markup=markup)
            
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=BnSxAiSd7lA&ab_channel=BaileyBrown"))
        botTimeWeb.send_message(message.chat.id, mess10, reply_markup=markup)
        
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=rA52LE82L90"))
        botTimeWeb.send_message(message.chat.id, mess11, reply_markup=markup)
        
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=tUJiGmCG5Yc"))
        botTimeWeb.send_message(message.chat.id, mess13, reply_markup=markup)
        
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=EtuEmHfCKi4&t=702s"))
        botTimeWeb.send_message(message.chat.id, mess14, reply_markup=markup)
 
 
 
    
    elif (message.text == "Растяжка" or message.text == "растяжка"):
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=s-7lyvblFNI&ab_channel=MadyMorrison"))
        botTimeWeb.send_message(message.chat.id, mess1, reply_markup=markup)
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=2eA2Koq6pTI&ab_channel=MadyMorrison"))
        botTimeWeb.send_message(message.chat.id, mess2, reply_markup=markup)
            
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=VcE3SFyHGAw&ab_channel=%D0%94%D0%B5%D0%BB%D0%B0%D0%B9%D0%A2%D0%B5%D0%BB%D0%BE"))
        botTimeWeb.send_message(message.chat.id, mess3, reply_markup=markup)
            
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=ihba9Lw0tv4&ab_channel=MadyMorrison"))
        botTimeWeb.send_message(message.chat.id, mess4, reply_markup=markup)
            
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=yqeirBfn2j4&ab_channel=MadyMorrison"))
        botTimeWeb.send_message(message.chat.id, mess5, reply_markup=markup)
        
        markup = types.InlineKeyboardMarkup()
        markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=2X78NWuRfJU"))
        botTimeWeb.send_message(message.chat.id, mess12, reply_markup=markup)
 
    
    elif (message.text == "Вернуться в главное меню"):
        markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
        button1 = types.KeyboardButton("Пилатес")
        button2 = types.KeyboardButton("Растяжка")
        markup.add(button1, button2)
        botTimeWeb.send_message(message.chat.id, text="Вы вернулись в главное меню", reply_markup=markup)
    elif(message.text == "?"):
        botTimeWeb.send_message(message.chat.id, text="Если хочешь начать тренировку - напиши Да, если хочешь расслабиться и дать своим мышщам отдохнуть, напиши - растяжка, если хочешь укрепить и накачать мышщы, напиши - пилатес) ")
    else:
        botTimeWeb.send_message(message.chat.id, text="Такой команды нет в моем арсенале :(")

botTimeWeb.polling(none_stop=True)