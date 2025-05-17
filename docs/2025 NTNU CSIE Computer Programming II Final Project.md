# 2025 NTNU CSIE Computer Programming II Final Project
## Topic: Twisted Fables
## 1. Introduction
![image](https://hackmd.io/_uploads/Skaf3N9R1x.png)
You can see the following sites to learn more about this game:
- Introduction: https://www.kickstarter.com/projects/diemensiongames/twisted-fables
- How To Play: https://youtu.be/D7QlQXT3U7k?si=z2O3qZ7d-Jdq8u3J
- **full document of this game in Chinese: https://hackmd.io/@ntnucsie-panda/BJer5UPKJl**
- Twisted Fables Breakdown videos https://youtube.com/playlist?list=PLm7bpXe-K-j-XQia2CBEp7L7HhZ4AxBTq&si=xyi0RSdPhCrlpUwu
- A sample play of of 1v1 https://youtu.be/9FZew2HOEEk?si=NB4ca93zarZr-XCw
## 2. Task
This time, I want you to develop Twisted Fables as a standalone version computer game which supports one user.
That is, you should also develop computer players. 
I know this is not an easy project, so this semester, you can form a team to complete this job. 
here is all thing you should done
1. 1v1 Game
2. at least 6 characters(you can choose any of them) 
3. TUI/GUI of this game
4. a bot that can play this game

and this is something you can do as bonus
1. 2v2 Game
2. Relic mode
3. the other characters
4. bot fighting competition on server we provide(1v1)
## 3. Demonstration
We will have a demonstration at the end of this semester. Each team should prepare the following items:
1. A brief description of your design.
2. Job assignment of each member.
3. Demo.
4. Anything special.

Each team will have **15 minutes** to demonstrate its work, including slide presentation and run its program. 
## 4. Scoring
Basic Game: 60
Presentation: 40
Bonus: 20
Any other intresting: 10
## 5. Important Date
Submission: 2025/6/16 8:00
Presentation: 2025/6/16

## 6. about competition
we will provide a api for you
this api included:
1. architecture.h
    - the game architecture you can use for connect server(server will send data by this format) 
    - this architecture tell you all state of game meaning
2. vector.h/vector.c
    - simple pack for size and a array
    - you should implement vector.c by your self if you need it
4. client.c/client.h
    - api for connecting server
    - for the start of bot, you should call `init_client()` to initalize the client
    - you can send data to server with function `send(void *data, size_t size)`
    - you can receive data of game by `receive(game *game_status)`
    - when game ending, call `destroy_client()` to free client
    - **you should send a 10 bytes data represent the character you are implement or not**

hint:
for a loop you should done the follow thing 
1. receive data in the begin of loop
2. after receive data, you should do something for each status
3. send the data to server

for join competition you have to implement the following feature
1. matches from Match Girl
2. posions from Snow White
3. any game states that you should show your hands

notice that **any wrong data** you send to server, server will think that you are cheating and you will lose the match

the card number about this game: https://hackmd.io/@ntnucsie-panda/S16mIpPFkg