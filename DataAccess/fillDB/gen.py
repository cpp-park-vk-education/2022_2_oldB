from faker import Faker
from random import *

N = 100

user_id = []
message_id = []
room_id = []

def generate_user():
    global user_id
    faker = Faker()
    f = open('users.csv', 'w')
    user_id = [i for i in range(1, N+1)]
    for i in range(N):
        line = "{0},{1},{2},{3},{4}\n".format(user_id[i],
                                              faker.first_name(),
                                              faker.last_name(),
                                              faker.user_name(),
                                              faker.password())
        f.write(line)
    f.close()

def generate_room():
    global room_id
    faker = Faker()
    f = open('rooms.csv', 'w')
    room_id = [i for i in range(1, N+1)]
    port = 2000
    for i in range(N):
        line = "{0},{1},{2}\n".format(room_id[i],
                                  faker.word(),
                                  port + i + 1)
        f.write(line)
    f.close()

def generate_message():
    global message_id
    faker = Faker()
    f = open('messages.csv', 'w')
    message_id = [i for i in range(1, N+1)]
    for i in range(N):
        line = "{0},{1},{2},{3}\n".format(message_id[i],
                                      faker.sentence(),
                                      choice(user_id), 
                                      choice(room_id))
        f.write(line)
    f.close()

def generate_userroom():
    f = open('user_rooms.csv', 'w')
    for i in range(N):
        line = "{0},{1},{2}\n".format(i,
                                      choice(user_id), 
                                      choice(room_id))
        f.write(line)
    f.close()

generate_user()
generate_room()
generate_message()
generate_userroom()
