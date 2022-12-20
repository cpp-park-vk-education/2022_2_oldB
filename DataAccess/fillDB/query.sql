CREATE TABLE users (
    id int GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    name varchar(40),
    surname varchar(40),
    login varchar(40) NOT NULL,
    password varchar(40) NOT NULL
);

\copy users(id, name, surname, login, password) from '/Users/karina/Desktop/chat/fill_db/users.csv' delimiter ',' csv;

CREATE TABLE rooms (
	id int GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    name varchar(40),
    port int NOT NULL
);

\copy rooms(id, name, port) from '/Users/karina/Desktop/chat/fill_db/rooms.csv' delimiter ',' csv;

CREATE TABLE messages (
	id int GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    text varchar(300) NOT NULL,
    user_id integer NOT NULL,
    room_id integer NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (room_id) REFERENCES rooms(id)
);

\copy messages(id, text, user_id, room_id) from '/Users/karina/Desktop/chat/fill_db/messages.csv' delimiter ',' csv;

CREATE TABLE user_rooms (
	id int GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    user_id integer NOT NULL,
    room_id integer NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (room_id) REFERENCES rooms(id)
);

\copy user_rooms(id, user_id, room_id) from '/Users/karina/Desktop/chat/fill_db/user_rooms.csv' delimiter ',' csv;


