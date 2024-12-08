import mytoken
import telebot
from mytoken import My_token
from telebot import types
botTimeWeb = My_token.botTimeWeb

@botTimeWeb.message_handler(commands=['start'])
def startBot(message):
    first_mess = f"<b>{message.from_user.first_name}</b>, приветик!\nДавай начнем тренировку?"
    markup = types.InlineKeyboardMarkup()
    button_yes = types.InlineKeyboardButton(text = 'Давай :)', callback_data='yes')
    markup.add(button_yes)
    botTimeWeb.send_message(message.chat.id, first_mess, parse_mode='html', reply_markup=markup)
  
@botTimeWeb.callback_query_handler(func=lambda call:True)
def response(function_call):
    if function_call.message:
        if function_call.data == "yes":
                        
            mess1 = "Стрейчинг для шеи:"
            mess2 = "Стрейчинг от боли в спине:"
            mess3 = "Общая растяжка 5 мин:"
            mess4 = "Общая растяжка 10 мин:"
            mess5 = "Общая растяжка 15 мин:"
            mess6 = "Кардио пилатес 10 мин:"
            mess7 = "Пилатес на руки 7 мин:"
            mess8 = "Пилатес на ноги 5 мин:"
            mess9 = "Пилатес на пресс 6 мин:"
            mess10 = "Пилатес на ягодицы 7 мин:"
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=s-7lyvblFNI&ab_channel=MadyMorrison"))
            botTimeWeb.send_message(function_call.message.chat.id, mess1, reply_markup=markup)
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=2eA2Koq6pTI&ab_channel=MadyMorrison"))
            botTimeWeb.send_message(function_call.message.chat.id, mess2, reply_markup=markup)
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=VcE3SFyHGAw&ab_channel=%D0%94%D0%B5%D0%BB%D0%B0%D0%B9%D0%A2%D0%B5%D0%BB%D0%BE"))
            botTimeWeb.send_message(function_call.message.chat.id, mess3, reply_markup=markup)
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=ihba9Lw0tv4&ab_channel=MadyMorrison"))
            botTimeWeb.send_message(function_call.message.chat.id, mess4, reply_markup=markup)
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=yqeirBfn2j4&ab_channel=MadyMorrison"))
            botTimeWeb.send_message(function_call.message.chat.id, mess5, reply_markup=markup)
            
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=AjWoaUBe80c&ab_channel=BaileyBrown"))
            botTimeWeb.send_message(function_call.message.chat.id, mess6, reply_markup=markup)
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=toOhNs85Mj8&ab_channel=BaileyBrown"))
            botTimeWeb.send_message(function_call.message.chat.id, mess7, reply_markup=markup)
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=ucW3ygyzgaI&ab_channel=BaileyBrown"))
            botTimeWeb.send_message(function_call.message.chat.id, mess8, reply_markup=markup)
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=N2yVhRKMApY&ab_channel=BaileyBrown"))
            botTimeWeb.send_message(function_call.message.chat.id, mess9, reply_markup=markup)
            
            markup = types.InlineKeyboardMarkup()
            markup.add(types.InlineKeyboardButton("Вперед!", url="https://www.youtube.com/watch?v=BnSxAiSd7lA&ab_channel=BaileyBrown"))
            botTimeWeb.send_message(function_call.message.chat.id, mess10, reply_markup=markup)
            
            
            
            
            botTimeWeb.answer_callback_query(function_call.id)

botTimeWeb.infinity_polling()